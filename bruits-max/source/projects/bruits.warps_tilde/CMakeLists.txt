# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.

cmake_minimum_required(VERSION 3.0)

set(C74_MIN_API_DIR ${BRUITS_BASE_PATH}/third_party/min-api)
set(EURORACK_DIR ${BRUITS_BASE_PATH}/third_party/eurorack)

include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)


#############################################################
# MAX EXTERNAL
#############################################################


set(SOURCE_FILES
	${PROJECT_NAME}.cpp

    ${EURORACK_DIR}/stmlib/dsp/units.h
    ${EURORACK_DIR}/stmlib/dsp/units.cc
    ${EURORACK_DIR}/stmlib/utils/random.h
    ${EURORACK_DIR}/stmlib/utils/random.cc

    ${EURORACK_DIR}/warps/resources.h
    ${EURORACK_DIR}/warps/resources.cc
    ${EURORACK_DIR}/warps/dsp/filter_bank.h
    ${EURORACK_DIR}/warps/dsp/filter_bank.cc
    ${EURORACK_DIR}/warps/dsp/modulator.h
    ${EURORACK_DIR}/warps/dsp/modulator.cc
    ${EURORACK_DIR}/warps/dsp/oscillator.h
    ${EURORACK_DIR}/warps/dsp/oscillator.cc
    ${EURORACK_DIR}/warps/dsp/vocoder.h
    ${EURORACK_DIR}/warps/dsp/vocoder.cc
)


add_library(
	${PROJECT_NAME}
	MODULE
	${SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE

	${C74_INCLUDES}
    ${BRUITS_BASE_PATH}/third_party/eurorack
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    TEST=1
)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)


#############################################################
# UNIT TEST
#############################################################

#include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
