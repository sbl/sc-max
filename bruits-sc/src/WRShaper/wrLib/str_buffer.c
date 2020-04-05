#include "str_buffer.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef S_SPLINT_S
typedef int8_t char;
typedef uint16_t int;
#endif

str_buffer_t* str_buffer_init(int len) {
    str_buffer_t* self = malloc(sizeof(str_buffer_t));
    if (!self) {
        printf("str_buffer malloc failed\n");
        return NULL;
    }

    self->length = len;
    self->contents = malloc(sizeof(char) * len + 1);
    if (!self->contents) {
        printf("str_buffer contents!\n");
        return NULL;
    }

    for (int i = 0; i < (len + 1); i++) {
        self->contents[i] = '\0';
    }

    // empty when read == write ix
    self->ix_read = 0;
    self->ix_write = 0;
    self->count = 0;

    return self;
}

void str_buffer_deinit(str_buffer_t* buf) {
    free(buf->contents);
    buf->contents = NULL;
    free(buf);
    buf = NULL;
}

void str_buffer_enqueue(str_buffer_t* buf, char* s) {
    while (*s != '\0') { // until we reach a NULL
        buf->contents[buf->ix_write++] = *s++;
        if (buf->ix_write >= buf->length) {
            buf->ix_write = 0;
        }
        buf->count++;
    }
}

char* str_buffer_dequeue(str_buffer_t* buf, uint16_t size) {
    char* ret_str = &buf->contents[buf->ix_read];
    buf->count -= size;
    buf->ix_read += size;
    if (buf->ix_read >= buf->length) {
        buf->ix_read = 0;
    }
    return ret_str;
}

uint16_t str_buffer_len(str_buffer_t* buf) {
    uint16_t ret_len = buf->count;
    if ((ret_len + buf->ix_read) > buf->length) {
        ret_len = buf->length - buf->ix_read;
    }
    return ret_len;
}

uint8_t str_buffer_empty(str_buffer_t* buf) {
    return (!buf->count);
}
