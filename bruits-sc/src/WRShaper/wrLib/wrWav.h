#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char sGroupID[4]; // "RIFF"
    uint32_t dwFileLength; // total file size
    char sRiffType[4]; // "WAVE"
} WaveHeader_t;

#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_ALAW 0x0006
#define WAVE_FORMAT_MULAW 0x0007
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE

typedef struct {
    char sGroupID[4]; // "fmt "
    uint32_t dwChunkSize; // length of this chunk *after* this line
    uint16_t wFormatTag; // see WAVE_FORMAT_* defines
    uint16_t wChannels; // channels of audio
    uint32_t dwSamplesPerSec; // sample rate
    uint32_t dwAvgBytesPerSec; // sampleRate * blockAlign
    uint16_t wBlockAlign; // wChannels * (dwBitsPerSample / 8)
    uint16_t dwBitsPerSample; // 24
} WaveFormat_t;

typedef struct {
    char sGroupID[4];
    uint32_t dwChunkSize;
    uint32_t dwSampleLength; // optional? see chunksize
} WaveFact_t; // Required for non PCM data (ie floats)

typedef struct {
    char sGroupID[4]; // "data"
    uint32_t dwChunkSize; // length of the below array
    void* sampleData_p; // pointer to the audio data buffer
} WaveData_t;

typedef struct {
    WaveHeader_t* h;
    WaveFormat_t* f;
    WaveFact_t* fa;
    WaveData_t* d;
    FILE* file_read;
    FILE* file_write;
} WavFile_t;

WavFile_t* wav_load(FILE* file);
float* wav_malloc_float(WavFile_t* src, int* count);
void wav_save_as(FILE* dest, WavFile_t* src);
WavFile_t* wav_new(uint16_t channels, uint32_t samplerate, uint16_t bitdepth,
                   FILE* file);

WavFile_t* wav_append(WavFile_t* wf, void* audio, uint32_t sample_count);
void wav_close(WavFile_t* wf);
