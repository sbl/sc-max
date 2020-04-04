include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
    Wellen.cpp

    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/poly_slope_generator.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/poly_slope_generator.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/ramp_extractor.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/ramp_extractor.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/ramp_generator.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/ramp_shaper.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/ratio.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/resources.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/tides2/resources.h

    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/stmlib.h
    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/dsp/units.cc
    ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/dsp/units.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${BRUITS_BASE_PATH}/third_party/eurorack
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEST=1
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
