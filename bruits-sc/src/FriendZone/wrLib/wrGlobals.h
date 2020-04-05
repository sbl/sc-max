#pragma once

// ints
#define MAX24b2 0x00FFFFFF
#define MAX24b 0x007FFFFF
#define HALF24b 0x003FFFFF
#define MIN24b 0xFF800000
#define MAX16f 0x00007FFF

// floats
#define HALF24f ((float)HALF24b)
#define MAX24f ((float)MAX24b)
#define MAX24f2 ((float)MAX24b2)
#define iMAX24f ((float)(1.0 / MAX24f))
#define MIN24f ((float)MIN24b)
#define iMAX16f ((float)(1.0 / MAX16f))

#define nFloor ((float)0.00024) // -72dB
#define nFloorSub1 ((float)(1.0f - nFloor)) // -0.01dB?
#define nMAX24f ((float)0xFF800000)

#define WR_PI ((float)3.14159265358979323846264)
