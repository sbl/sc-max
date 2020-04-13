cmake_minimum_required(VERSION 3.7)
project(dsp_kit)

add_executable(test-noise
        lcg.c
        pink.c
        pink.h
        test-noise.c)

target_link_libraries(test-noise sndfile)