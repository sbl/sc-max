#include "wrPoly.h"

#include <stdio.h> // printf
#include <stdlib.h>

// private declarations
void _free_voice(poly_alloc_t* self, uint8_t voice);
int8_t _assign_voice(poly_alloc_t* self, int16_t note);
int8_t _steal_voice(poly_alloc_t* self, int16_t note);

// public definitions
poly_alloc_t* poly_init(uint8_t voice_count) {
    poly_alloc_t* self = malloc(sizeof(poly_alloc_t));
    if (!self) {
        printf("poly malloc failed\n");
        return NULL;
    }

    self->voice_count = voice_count;

    self->notes = malloc(sizeof(int16_t) * voice_count);
    self->busy_count = 0;
    self->busy_list = malloc(sizeof(uint8_t) * voice_count);

    self->free_count = voice_count;
    self->free_first = 0;
    self->free_queue = malloc(sizeof(uint8_t) * voice_count);
    for (uint8_t i = 0; i < voice_count; i++) {
        self->notes[i] = 0x8000;
        self->busy_list[i] = 255;
        self->free_queue[i] = 255;
    }
    return self;
}

void poly_deinit(poly_alloc_t* self) {
    free(self->free_queue);
    self->free_queue = NULL;
    free(self->busy_list);
    self->busy_list = NULL;
    free(self->notes);
    self->notes = NULL;
    free(self);
    self = NULL;
}

int8_t poly_assign_note(poly_alloc_t* self, int16_t note) {
    // note on
    if (note == 0x8000) {
        return -1;
    } // invalid pitch

    // in future, search for note w same pitch

    // assign next free voice if there is one
    if (self->free_count) {
        return _assign_voice(self, note);
    } else {
        return _steal_voice(self, note);
    }
}

int8_t poly_kill_note(poly_alloc_t* self, int16_t note) {
    if (note == 0x8000) {
        return -1;
    } // invalid pitch

    // search for match in self->notes
    for (uint8_t i = 0; i < self->voice_count; i++) {
        if (note == self->notes[i]) { // found, so kill this
            _free_voice(self, i);
            return i;
        }
    }
    return -1;
}

void _free_voice(poly_alloc_t* self, uint8_t voice) {
    // forget pitch
    self->notes[voice] = 0x8000;

    // rm note from busy list (maintains age sorting)
    // matches first occurance, not oldest, for duplicate pitches
    uint8_t i = 0;
    while (voice != self->busy_list[i]) {
        i++;
    }
    for (; i < (--self->busy_count); i++) {
        self->busy_list[i] = self->busy_list[i + 1];
    }

    // enqueue note into free queue
    // add check for full-queue (shouldn't happen)
    uint8_t ix = (self->free_first + self->free_count++) % self->voice_count;
    self->free_queue[ix] = voice;
}

int8_t _assign_voice(poly_alloc_t* self, int16_t note) {
    int8_t voice;

    // dequeue voice from free
    voice = self->free_first++;
    if (self->free_first >= self->voice_count) {
        self->free_first = 0;
    }
    self->free_count--;

    // add busy voice to end of list
    self->busy_list[self->busy_count++] = voice;

    // save pitch
    self->notes[voice] = note;

    // return voice
    return voice;
}

int8_t _steal_voice(poly_alloc_t* self, int16_t note) {
    int8_t voice;

    // steal front of busy list
    voice = self->busy_list[0];

    // shift list forward (ouch..)
    // want to do a queue, but it varies in length?
    for (uint8_t i = 0; i < (self->voice_count - 1); i++) {
        self->busy_list[i] = self->busy_list[i + 1];
    }
    self->busy_list[self->voice_count - 1] = voice;

    // save pitch
    self->notes[voice] = note;

    // return voice
    return voice;
}
