#include "wrEvent.h"

#include <stdio.h>

event_extract_t* Extract_init(void) {
    event_extract_t* e = malloc(sizeof(event_extract_t));
    if (!e) {
        printf("Event malloc failed\n");
        return NULL;
    }

    // parameter defaults
    e->hyst_time = 10; // >= EXTRACT_HIST_LENGTH for debouncing

    // these constants are divided by EXTRACT_HIST_LENGTH
    // trigger detection
    e->tr_abs_level = 0.25; // trigger level re:0v
    e->tr_rel_level = 0.2; // trigger level re:cv-level
    // trigger||cv thresholds
    e->cv_thresh_same = 0.2; // cv-level re:0v (same dir as trigger)
    e->cv_thresh_opp = 0.15; // cv-level re:0v (opposite dir)

    // state
    e->hyst = 0;
    for (uint8_t i = 0; i < EXTRACT_HIST_LENGTH; i++) {
        e->in_history[i] = 0.0;
    }
    e->gate_state = 0;

    return e;
}

void Extract_deinit(event_extract_t* e) {
    free(e);
    e = NULL;
}

etrig_t Extract_cv_trigger(event_extract_t* e, float in) {
    etrig_t retval = tr_none;

    if (e->hyst == 0) {
        if (e->gate_state == 0) {
            if (in > (e->in_history[0] + e->tr_rel_level)
                && in >= e->tr_abs_level) {
                // trigger high
                // replace these conditionals with some integer math
                // shift the cv, scale, then cast, then lookup in tr_t
                if (e->in_history[0] > e->cv_thresh_same) {
                    retval = tr_p_positive;
                } else if (e->in_history[0] < -(e->cv_thresh_opp)) {
                    retval = tr_p_negative;
                } else {
                    retval = tr_p_same;
                }
                e->gate_state = 1;
                e->hyst = e->hyst_time;
            } else if (in < (e->in_history[0] - e->tr_rel_level)
                       && in <= -(e->tr_abs_level)) {
                // trigger low
                if (e->in_history[0] > e->cv_thresh_opp) {
                    retval = tr_n_positive;
                } else if (e->in_history[0] < -(e->cv_thresh_same)) {
                    retval = tr_n_negative;
                } else {
                    retval = tr_n_same;
                }
                e->gate_state = -1;
                e->hyst = e->hyst_time;
            } else {
                // no trigger -> gate-state unchanged
                retval = tr_none;
            }
        } else { // gate_state is HIGH
            if (in < e->tr_abs_level && in > -(e->tr_abs_level)) {
                retval = tr_none;
                e->gate_state = 0; // release
                e->hyst = e->hyst_time;
            } else {
                retval = tr_hold; // still holding!
            }
        }
    } else {
        e->hyst--;
        if (e->hyst < 0) {
            e->hyst = 0;
        }
        retval = tr_hold;
    }

    // circular buffer
    for (uint8_t i = 0; i < EXTRACT_HIST_LENGTH - 1; i++) {
        e->in_history[i] = e->in_history[i + 1];
    }
    e->in_history[EXTRACT_HIST_LENGTH - 1] = in;

    return retval;
}

Debounce_t* Debounce_init(uint16_t debounce_count) {
    Debounce_t* db = malloc(sizeof(Debounce_t));
    if (!db) {
        printf("Debounce malloc failed\n");
        return NULL;
    }
    db->state = 0;
    db->count = 0;
    db->count_to = debounce_count;
    return db;
}

void Debounce_deinit(Debounce_t* db) {
    free(db);
    db = NULL;
}

uint8_t Debounce_step(Debounce_t* db, uint8_t in) {
    if (in != db->state) { // change
        if (++db->count >= db->count_to) {
            db->state = in;
            db->count = 0;
        }
    } else {
        db->count = 0;
    }
    return db->state;
}

// should inherit from debounce
Debounce_a_t* Debounce_a_init(uint16_t debounce_count_up,
                              uint16_t debounce_count_down) {
    Debounce_a_t* db = malloc(sizeof(Debounce_a_t));
    if (!db) {
        printf("Debounce_a malloc failed\n");
        return NULL;
    }
    db->state = 0;
    db->count = 0;
    db->count_to_up = debounce_count_up;
    db->count_to_down = debounce_count_down;
    return db;
}

void Debounce_a_deinit(Debounce_a_t* db) {
    free(db);
    db = NULL;
}

uint8_t Debounce_a_step(Debounce_a_t* db, uint8_t in) {
    if (in != db->state) { // change
        if (db->state) {
            if (++db->count >= db->count_to_down) {
                db->state = in;
                db->count = 0;
            }
        } else {
            if (++db->count >= db->count_to_up) {
                db->state = in;
                db->count = 0;
            }
        }
    } else {
        db->count = 0;
    }
    return db->state;
}
