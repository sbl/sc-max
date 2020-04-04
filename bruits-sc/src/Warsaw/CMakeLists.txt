include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
  Warsaw.cpp

  ${BRUITS_BASE_PATH}/third_party/eurorack/plaits/resources.cc
  ${BRUITS_BASE_PATH}/third_party/eurorack/stmlib/dsp/units.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${BRUITS_BASE_PATH}/third_party/eurorack
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEST=1
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
