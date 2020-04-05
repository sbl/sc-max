#pragma once

typedef struct clip_soft {
    float t; // threshold
    float g; // gain
    float c; // parabolic coefficient
    float o; // parabolic offset (aka max gain)
} clip_soft_t;

clip_soft_t* clip_soft_init_default(void);
clip_soft_t* clip_soft_init(float threshold, float gain);
void clip_soft_deinit(clip_soft_t* self);

void clip_soft_set_threshold(clip_soft_t* self, float threshold);
void clip_soft_set_gain(clip_soft_t* self, float gain);

float clip_soft_step(clip_soft_t* self, float in);
float* clip_soft_step_v(clip_soft_t* self, float* in, int size);
