include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

add_library(${PROJECT_NAME} MODULE
    Zopf.cpp

    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/dsp/units.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/dsp/units.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/dsp/units.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/utils/random.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/utils/random.cc

    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/additive_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/bass_drum_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/chord_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/fm_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/grain_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/hi_hat_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/modal_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/noise_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/particle_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/snare_drum_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/speech_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/string_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/swarm_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/virtual_analog_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/waveshaping_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/engine/wavetable_engine.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/physical_modelling/modal_voice.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/physical_modelling/resonator.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/physical_modelling/string.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/physical_modelling/string_voice.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/speech/lpc_speech_synth.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/speech/lpc_speech_synth_controller.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/speech/lpc_speech_synth_phonemes.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/speech/lpc_speech_synth_words.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/speech/naive_speech_synth.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/speech/sam_speech_synth.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/voice.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/dsp/voice.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/resources.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${BRUITS_BASE_PATH}/third_party/eurorack
    ${CMAKE_SOURCE_DIR}/includes
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEST=1
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
