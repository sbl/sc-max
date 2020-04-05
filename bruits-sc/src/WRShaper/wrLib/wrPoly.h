#pragma once

#include <stdint.h>

/* polyphonic note allocator
 * assumes all voices have same release time
 * up to 128 voices
 * pitch values are int16_t only
 * first attempts to assign to oldest, inactive voice
 * else steals longest held note
 * notes are assumed free upon beginning release stage
 *
 * once a note enters release it's pitch is forgotten
 * todo: note's maintain memory, for duplicate notes to
 *       trigger in same channel in case still sustaining.
 */

typedef struct poly_alloc {
    uint8_t voice_count;

    int16_t* notes;
    uint8_t busy_count;
    uint8_t* busy_list;

    uint8_t free_count;
    int16_t free_first;
    uint8_t* free_queue;
} poly_alloc_t;

poly_alloc_t* poly_init(uint8_t voice_count);
void poly_deinit(poly_alloc_t* self);
int8_t poly_assign_note(poly_alloc_t* self, int16_t note);
int8_t poly_kill_note(poly_alloc_t* self, int16_t note);
