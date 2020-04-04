# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.

cmake_minimum_required(VERSION 3.0)

set(C74_MIN_API_DIR ${BRUITS_BASE_PATH}/third_party/min-api)
set(EURORACK_DIR ${BRUITS_BASE_PATH}/third_party/eurorack)

include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)


#############################################################
# MAX EXTERNAL
#############################################################



set(PLAITS_FILES
    ${EURORACK_DIR}/stmlib/dsp/atan.h
    ${EURORACK_DIR}/stmlib/dsp/units.h
    ${EURORACK_DIR}/stmlib/utils/random.h
    ${EURORACK_DIR}/stmlib/utils/random.cc
    ${EURORACK_DIR}/stmlib/dsp/atan.cc
    ${EURORACK_DIR}/stmlib/dsp/units.cc

    ${EURORACK_DIR}/plaits/dsp/engine/additive_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/additive_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/bass_drum_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/bass_drum_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/chord_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/chord_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/fm_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/fm_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/grain_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/grain_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/hi_hat_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/hi_hat_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/modal_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/modal_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/noise_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/noise_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/particle_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/particle_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/snare_drum_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/snare_drum_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/speech_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/speech_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/string_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/string_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/swarm_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/swarm_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/virtual_analog_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/virtual_analog_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/waveshaping_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/waveshaping_engine.h
    ${EURORACK_DIR}/plaits/dsp/engine/wavetable_engine.cc
    ${EURORACK_DIR}/plaits/dsp/engine/wavetable_engine.h
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/modal_voice.cc
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/modal_voice.h
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/resonator.cc
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/resonator.h
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/string.cc
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/string.h
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/string_voice.cc
    ${EURORACK_DIR}/plaits/dsp/physical_modelling/string_voice.h
    ${EURORACK_DIR}/plaits/dsp/speech/lpc_speech_synth.cc
    ${EURORACK_DIR}/plaits/dsp/speech/lpc_speech_synth.h
    ${EURORACK_DIR}/plaits/dsp/speech/lpc_speech_synth_controller.cc
    ${EURORACK_DIR}/plaits/dsp/speech/lpc_speech_synth_controller.h
    ${EURORACK_DIR}/plaits/dsp/speech/lpc_speech_synth_phonemes.cc
    ${EURORACK_DIR}/plaits/dsp/speech/lpc_speech_synth_words.cc
    ${EURORACK_DIR}/plaits/dsp/speech/lpc_speech_synth_words.h
    ${EURORACK_DIR}/plaits/dsp/speech/naive_speech_synth.cc
    ${EURORACK_DIR}/plaits/dsp/speech/naive_speech_synth.h
    ${EURORACK_DIR}/plaits/dsp/speech/sam_speech_synth.cc
    ${EURORACK_DIR}/plaits/dsp/speech/sam_speech_synth.h
    ${EURORACK_DIR}/plaits/dsp/voice.cc
    ${EURORACK_DIR}/plaits/dsp/voice.h
    ${EURORACK_DIR}/plaits/resources.cc
    ${EURORACK_DIR}/plaits/resources.h

    speex/resample.c
    )


set(SOURCE_FILES
	${PROJECT_NAME}.cpp
  ${PLAITS_FILES}
)


add_library(
	${PROJECT_NAME}
	MODULE
	${SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE

    ${BRUITS_BASE_PATH}/third_party/eurorack
	"${C74_INCLUDES}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/rack"
    "${CMAKE_CURRENT_SOURCE_DIR}/dsp"
    "${CMAKE_CURRENT_SOURCE_DIR}/pffft"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEST=1
    HAVE_CONFIG_H=1
)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)


#############################################################
# UNIT TEST
#############################################################

#include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
