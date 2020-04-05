#pragma once

// a wrapper over get/set functions into a stream - an 'interface' (?)
// uses a request/respond model to enable deferred access

// usage:
// the server should create a local wrStream_t
// server provides an 'init' function that returns wrStream_t*
// client accepts wrStream_t* and augments it with response & error fnptrs.

typedef enum { DIR_READ, DIR_WRITE } wrStream_DIR_t;

typedef int (*wrStream_OC_t)(void);
typedef int (*wrStream_RR_t)(wrStream_DIR_t direction, int location,
                             int size_in_bytes, uint8_t* data);
typedef void (*wrStream_ER_t)(int errorcode, char* msg);
typedef struct {
    wrStream_OC_t open;
    wrStream_OC_t close;
    wrStream_OC_t busy;
    wrStream_RR_t request;
    wrStream_RR_t response;
    wrStream_ER_t error;
} wrStream_t;

typedef struct {
    wrStream_DIR_t direction;
    int location;
    int size_in_bytes;
    uint8_t* data;
} wrStream_PACKET_t;
