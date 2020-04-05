#include "wrHead.h"
#include "wrMath.h" // iMAX24f, MAX24b, lim_i24_audio
#include <stdio.h>
#include <stdlib.h>

#include "wrConvert.h" // _s12_to_sf() _sf_to_s12()

rhead_t* RH_Init(void) {
    rhead_t* self = malloc(sizeof(rhead_t));
    if (!self) {
        printf("RH: malloc failed\n");
        return NULL;
    }

    self->record = lp1_init();
    self->feedback = lp1_init();
    self->monitor = lp1_init();
    // monitor needs to default to on
    lp1_set_dest(self->feedback, 1.0);
    lp1_set_out(self->feedback, 1.0);
    lp1_set_dest(self->monitor, 1.0);
    // slew times. can tune each individually?
    lp1_set_coeff(self->record, HEAD_SLEW_RECORD);
    lp1_set_coeff(self->feedback, HEAD_SLEW_FEEDBACK);
    lp1_set_coeff(self->monitor, HEAD_SLEW_MONITOR);

    return self;
}

void RH_DeInit(rhead_t* self) {
    lp1_deinit(self->monitor);
    lp1_deinit(self->feedback);
    lp1_deinit(self->record);
    free(self);
}

void RH_set_rw(rhead_t* self, tape_mode_t rmode) {
    lp1_set_dest(self->record, (float)(rmode != READONLY));
    lp1_set_dest(self->feedback, (float)(rmode <= OVERDUB));
    lp1_set_dest(self->monitor, (float)(rmode != OVERWRITE));
}

void RH_set_record_level(rhead_t* self, float level) {
    lp1_set_dest(self->record, level);
}

void RH_set_record_params(rhead_t* self, float feedback, float monitor) {
    lp1_set_dest(self->feedback, feedback);
    lp1_set_dest(self->monitor, monitor);
}

IO_block_t* RH_rw_process(rhead_t* self, IO_block_t* headbuf, HEAD_Action_t action,
                          int32_t** access, int count, int* dirty,
                          int write_offset) {
    uint16_t i;
    float* input_v = headbuf->audio;
    float* return_v = headbuf->audio;
    int32_t** tape_r = access;

    if (action == HEAD_Fadeout // if xfading, act on 1st
        || action == HEAD_Active) { // if !xfade, act on only head
        lp1_step_internal(self->record);
        lp1_step_internal(self->feedback);
        lp1_step_internal(self->monitor);
    }

    // playback mode if record is staying at 0.0
    if (lp1_get_dest(self->record) == 0.0 && lp1_get_out(self->record) == 0.0) {
        if (action == HEAD_Fadeout) {
            float fade_step = 1.0 / (float)(count - 1);
            float fade_out = 1.0;
            for (i = 0; i < (count); i++) {
                fade_out -= fade_step;
                *(write_offset + *tape_r) = **tape_r; // 'write' action is playback
                // 'read' action
                *return_v++ = iMAX24f * (float)(**tape_r++ << BIT_HEADROOM)
                    * lp1_get_out(self->monitor) * fade_out;
            }
        } else if (action == HEAD_Fadein) {
            float fade_step = 1.0 / (float)(count - 1);
            float fade_in = 0.0;
            for (i = 0; i < (count); i++) {
                fade_in += fade_step;
                *(write_offset + *tape_r) = **tape_r; // 'write' action is playback
                // 'read' action
                *return_v++ = iMAX24f * (float)(**tape_r++ << BIT_HEADROOM)
                    * lp1_get_out(self->monitor) * fade_in;
            }
        } else {
            for (i = 0; i < (count); i++) {
                *(write_offset + *tape_r) = **tape_r; // 'write' action is playback
                // 'read' action
                *return_v++ = iMAX24f * (float)(**tape_r++ << BIT_HEADROOM)
                    * lp1_get_out(self->monitor);
            }
        }
    } else {
        if (action == HEAD_Fadeout) {
            float fade_step = 1.0 / (float)(count - 1);
            float fade_out = 1.0;

            for (i = 0; i < (count); i++) {
                fade_out -= fade_step; // fade out
                // 'write' action
                if ((*(write_offset + *tape_r) == INVALID_SAMP)) {
                    *(write_offset + *tape_r) =
                        lim_i24_audio((int32_t)
                                      // INPUT
                                      (lp1_get_out(self->record) * fade_out
                                           * (*input_v++) * F_TO_TAPE_SCALE
                                       // SCALE FEEDBACK TOWARD PLAYBACK
                                       + ((lp1_get_out(self->feedback) - 1.0)
                                              * lp1_get_out(self->record)
                                          + 1.0
                                          // FEEDBACK
                                          ) * (float)(**tape_r)));
                } else {
                    *(write_offset + *tape_r) =
                        lim_i24_audio((int32_t)
                                      // INPUT
                                      (lp1_get_out(self->record) * fade_out
                                       * (*input_v++) * F_TO_TAPE_SCALE)
                                      // FEEDBACK (already copied)
                                      + *(write_offset + *tape_r));
                }
                // 'read' action
                *return_v++ = iMAX24f * (float)(**tape_r++ << BIT_HEADROOM)
                    * lp1_get_out(self->monitor)
                    // fade_in starts to affect as self->record fades out
                    * (1.0 + (fade_out - 1.0) * (1.0 - lp1_get_out(self->record)));
            }
        }

        else if (action == HEAD_Fadein) {
            float fade_step = 1.0 / (float)(count - 1);
            float fade_in = 0.0;

            for (i = 0; i < (count); i++) {
                fade_in += fade_step; // linear ramp, start step above zero
                // 'write' action
                if ((*(write_offset + *tape_r) == INVALID_SAMP)) {
                    *(write_offset + *tape_r) =
                        lim_i24_audio((int32_t)
                                      // INPUT * FADEIN
                                      (lp1_get_out(self->record) * fade_in
                                           * (*input_v++) * F_TO_TAPE_SCALE
                                       // SCALED FEEDBACK TOWARD PLAYBACK
                                       + ((lp1_get_out(self->feedback) - 1.0)
                                              * lp1_get_out(self->record)
                                          + 1.0)
                                           // FEEDBACK (already copied)
                                           * (float)(**tape_r)));
                } else {
                    *(write_offset + *tape_r) =
                        lim_i24_audio((int32_t)(lp1_get_out(self->record) * fade_in
                                                * (*input_v++) * F_TO_TAPE_SCALE)
                                      + *(write_offset + *tape_r));
                }
                // 'read' action
                *return_v++ = iMAX24f * (float)(**tape_r++ << BIT_HEADROOM)
                    * lp1_get_out(self->monitor)
                    // fade_in starts to affect as self->record fades out
                    * (1.0 + (fade_in - 1.0) * (1.0 - lp1_get_out(self->record)));
            }
        }

        else if (action == HEAD_Active) {
            for (i = 0; i < (count); i++) {
                // 'write' action
                if ((*(write_offset + *tape_r) == INVALID_SAMP)) {
                    *(write_offset + *tape_r) = lim_i24_audio(
                        (int32_t)
                        // INPUT
                        (lp1_get_out(self->record) * (*input_v++) * F_TO_TAPE_SCALE
                         // SCALE FEEDBACK TOWARD PLAYBACK
                         + ((lp1_get_out(self->feedback) - 1.0)
                                * lp1_get_out(self->record)
                            + 1.0)
                             // FEEDBACK
                             * (float)(**tape_r)));
                } else {
                    *(write_offset + *tape_r) = lim_i24_audio(
                        (int32_t)
                        // INPUT
                        (lp1_get_out(self->record) * (*input_v++) * F_TO_TAPE_SCALE)
                        // FEEDBACK (already copied)
                        + *(write_offset + *tape_r));
                }
                // 'read' action
                *return_v++ = iMAX24f * (float)(**tape_r++ << BIT_HEADROOM)
                    * lp1_get_out(self->monitor);
            }
        } // else {} inactive. won't happen

        // MARK DIRTY FLAG
        *dirty = 1;
    }
    return headbuf;
}

float RH_rw_process_cv(rhead_t* self, float input, uint8_t action // HEAD_Action_t
                       ,
                       int16_t* cv_read, int* dirty, int write_offset) {
    float rec_level = lp1_get_out(self->record);
    float fb_level = lp1_get_out(self->feedback);
    float mon_level = lp1_get_out(self->monitor);

    float ontape = _s12_to_sf(*cv_read);
    if (action) {
        *(cv_read + write_offset) =
            _sf_to_s12(ontape * fb_level + input * rec_level);
        *dirty = 1;
    } else {
        mon_level = 1.0;
    }

    return ontape * mon_level;
}
