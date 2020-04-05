#pragma once

typedef struct {
    float* buffer;
    float max_time;
    float max_samps;
    float rate;
    float feedback;
    float tap_fb;
    float tap_read;
    float tap_write;
} delay_t;

delay_t* delay_init(float max_time, float time);
void delay_deinit(delay_t* self);
void delay_set_ms(delay_t* self, float time);
void delay_set_time_percent(delay_t* self, float percent);
void delay_set_rate(delay_t* self, float rate);
void delay_set_read_phase(delay_t* self, float percent);
float delay_get_ms(delay_t* self);
void delay_set_feedback(delay_t* self, float feedback);
float delay_get_feedback(delay_t* self);
float delay_step(delay_t* self, float in);
float* delay_step_v(delay_t* self, float* buffer, int b_size);
