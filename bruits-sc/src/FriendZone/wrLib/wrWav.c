#include "wrWav.h"

#include <stdlib.h> // malloc
#include <string.h> // memcpy

static void wav_pretty_print(WavFile_t* w);
static void wav_updatelength(WavFile_t* wf, uint32_t byte_count);

/////////////////////////////////////////////////////////
// WARNING! LEAKS MEMORY FOR EVERY WAV FILE /////////////
// NEED TO CLEANUP ALL OF THE malloc'd DATA /////////////
/////////////////////////////////////////////////////////

WavFile_t* wav_load(FILE* file) {
    // read from file & create the appropriate structures
    WavFile_t* wav = malloc(sizeof(WavFile_t));
    wav->h = malloc(sizeof(WaveHeader_t));
    wav->f = malloc(sizeof(WaveFormat_t));
    wav->fa = malloc(sizeof(WaveFact_t));
    wav->d = malloc(sizeof(WaveData_t));
    wav->file_read = file;
    wav->file_write = NULL;

    // now just memcpy from src into structs?
    // do each struct separately
    fread(wav->h, sizeof(WaveHeader_t), 1, wav->file_read);
    fread(wav->f, sizeof(WaveFormat_t), 1, wav->file_read);
    if (wav->f->dwChunkSize != 16) {
        printf("TODO fseek to appropriate place.\n");
    }
    printf("TODO should really be a 'fact' chunk here?\n");
    // need to str_cmp to check what we're reading
    // only the data metadata
    fread(wav->d, 1, 8, wav->file_read);
    // now the data (relies on wav->d metadata)
    printf("FIXME wav samples are malloc'd! should just stream with fread\n");
    wav->d->sampleData_p = malloc(wav->d->dwChunkSize);
    fread(wav->d->sampleData_p, 1, wav->d->dwChunkSize, wav->file_read);
    wav_pretty_print(wav);
    return wav;
}

float* wav_malloc_float(WavFile_t* src, int* count) {
    int c = *count;
    *count = src->d->dwChunkSize / src->f->wBlockAlign;
    if (c > 0 && *count > c) {
        *count = c;
    } // limit to count
    float* f = malloc(sizeof(float) * *count);
    float* f2 = f;
    int16_t* origin = (int16_t*)src->d->sampleData_p; // assume 16bit integer
    for (int i = 0; i < *count; i++) {
        *f2++ = (1.0 / 0x7FFF) * (float)*origin;
        origin += src->f->wChannels;
    }
    return f;
}

static void wav_pretty_print(WavFile_t* w) {
    fprintf(stderr, "Header\tGroupID\t%.*s\n\tFileLength\t%i\n\tRiffType\t%.*s\n",
            4, w->h->sGroupID, w->h->dwFileLength, 4, w->h->sRiffType);
    fprintf(stderr,
            "Format\tGroupID\t%.*s\n\tChunkSize\t%i\n\tFormatTag\t%i\n\tChannels\t%"
            "i\n\tSamples/Sec\t%i\n\tBytes/"
            "Sec\t%i\n\tBlockAlign\t%i\n\tBits/Samp\t%i\n",
            4, w->f->sGroupID, w->f->dwChunkSize, w->f->wFormatTag, w->f->wChannels,
            w->f->dwSamplesPerSec, w->f->dwAvgBytesPerSec, w->f->wBlockAlign,
            w->f->dwBitsPerSample);
    if (w->f->wFormatTag != 0x0001) { // non-PCM data
        fprintf(stderr,
                "Fact\tGroupID\t%.*s\n\tChunkSize\t%u\n\tSampleLength\t%i\n", 4,
                w->fa->sGroupID, w->fa->dwChunkSize, w->fa->dwSampleLength);
    }
    fprintf(stderr, "Data\tGroupID\t%.*s\n\tChunkSize\t%u\n", 4, w->d->sGroupID,
            w->d->dwChunkSize);
}

void wav_save_as(FILE* dest, WavFile_t* src) {
    // write a wave file to a different file than the original
    src->file_write = dest;
    fwrite(src->h, sizeof(WaveHeader_t), 1, src->file_write);
    fwrite(src->f, sizeof(WaveFormat_t), 1, src->file_write);
    fwrite(src->d, 8, 1, src->file_write);
    wav_append(src, src->d->sampleData_p,
               src->d->dwChunkSize / src->f->wBlockAlign);
}

WavFile_t* wav_new(uint16_t channels, uint32_t samplerate, uint16_t bitdepth,
                   FILE* file_p) {
    WavFile_t* wav = malloc(sizeof(WavFile_t));
    wav->h = malloc(sizeof(WaveHeader_t));
    wav->f = malloc(sizeof(WaveFormat_t));
    wav->fa = malloc(sizeof(WaveFact_t));
    wav->d = malloc(sizeof(WaveData_t));
    wav->file_read = file_p;
    wav->file_write = file_p;

    memcpy(wav->h->sGroupID, "RIFF", 4);
    memcpy(wav->h->sRiffType, "WAVE", 4);

    memcpy(wav->f->sGroupID, "fmt ", 4);
    wav->f->dwChunkSize = sizeof(WaveFormat_t) - 8;
    wav->f->wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    wav->f->wChannels = channels;
    wav->f->dwSamplesPerSec = samplerate;
    wav->f->dwBitsPerSample = bitdepth;
    wav->f->wBlockAlign = wav->f->wChannels * (wav->f->dwBitsPerSample >> 3);
    wav->f->dwAvgBytesPerSec = wav->f->dwSamplesPerSec * wav->f->wBlockAlign;
    // TODO
    // if formatTag not PCM, need to add fact chunk

    // data block is empty
    memcpy(wav->d->sGroupID, "data", 4);
    wav->d->dwChunkSize = 0;
    wav->h->dwFileLength = 4 + sizeof(WaveFormat_t) + 8;

    fwrite(wav->h, sizeof(WaveHeader_t), 1, wav->file_write);
    fwrite(wav->f, sizeof(WaveFormat_t), 1, wav->file_write);
    fwrite(wav->d, 8, 1, wav->file_write);
    return wav;
}

WavFile_t* wav_append(WavFile_t* wf, void* audio, uint32_t sample_count) {
    uint32_t byte_count = sample_count * wf->f->wBlockAlign;
    fwrite(audio, byte_count, 1, wf->file_write);
    wav_updatelength(wf, byte_count);
    return wf;
}

void wav_close(WavFile_t* wf) {
    fclose(wf->file_read);
    fclose(wf->file_write);
    // free() the inner & outer struct
}

static void wav_updatelength(WavFile_t* wf, uint32_t byte_count) {
    // save current FILE* location
    long int tell = ftell(wf->file_write);

    wf->d->dwChunkSize += byte_count;
    wf->h->dwFileLength += byte_count;
    // TODO how to abstract this based on struct layout?
    fseek(wf->file_write, sizeof(WaveHeader_t) + sizeof(WaveFormat_t) + 4,
          SEEK_SET);
    fwrite(&(wf->d->dwChunkSize), sizeof(uint32_t), 1, wf->file_write);
    fseek(wf->file_write, 4, SEEK_SET);
    fwrite(&(wf->h->dwFileLength), sizeof(uint32_t), 1, wf->file_write);

    // restore FILE* location
    fseek(wf->file_write, tell, SEEK_SET);
}
