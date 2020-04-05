#pragma once

#include <stdint.h>
#include <stdlib.h>

#define EXTRACT_HIST_LENGTH 2 // >= 1 (no delay)

typedef enum etrig {
    tr_none = 0,
    tr_p_negative = 1,
    tr_p_same = 2,
    tr_p_positive = 3,
    tr_hold = 4,
    tr_n_negative = 5 // as for 1-3 w/ b3 masked negative
    ,
    tr_n_same = 6,
    tr_n_positive = 7
} etrig_t;

typedef struct event_extract {
    // parameters
    int8_t hyst_time;
    float tr_abs_level;
    float tr_rel_level;
    float cv_thresh_same;
    float cv_thresh_opp;

    // state
    int8_t hyst; // hysteresis
    float in_history[EXTRACT_HIST_LENGTH]; // input history
    int8_t gate_state; // +/-1 if gate is high/low, else 0
} event_extract_t;

// variable speed
event_extract_t* Extract_init(void);
void Extract_deinit(event_extract_t* e);
etrig_t Extract_cv_trigger(event_extract_t* e, float in);

typedef struct debounce {
    uint8_t state;
    uint16_t count;
    uint16_t count_to;
} Debounce_t;

Debounce_t* Debounce_init(uint16_t debounce_count);
void Debounce_deinit(Debounce_t* db);
uint8_t Debounce_step(Debounce_t* db, uint8_t in);

typedef struct debounce_a {
    uint8_t state;
    uint16_t count;
    uint16_t count_to_up;
    uint16_t count_to_down;
} Debounce_a_t;

Debounce_a_t* Debounce_a_init(uint16_t debounce_count_up,
                              uint16_t debounce_count_down);
void Debounce_a_deinit(Debounce_a_t* db);
uint8_t Debounce_a_step(Debounce_a_t* db, uint8_t in);
