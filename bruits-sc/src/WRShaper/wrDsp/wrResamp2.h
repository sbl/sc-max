#pragma once

// wrap buffer zero to buf_len
float buffer_peek(float* buf, int buf_len, float phase);

// provided buf must have leading & trailing samples
float buffer_peek_unsafe(float* buf, float phase);

// TODO: benchmark against basic for loop around buffer_peek
float* buffer_peek_v(float* io, float* buf, int buf_len,
                     float phase // initial sample
                     ,
                     float* rate // step through buf with this array
                     ,
                     int size);
