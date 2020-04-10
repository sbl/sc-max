include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
    ${PROJECT_NAME}.cpp

    softcut/FadeCurves.cpp
    softcut/SoftCutHead.cpp
    softcut/SoftCutVoice.cpp
    softcut/SubHead.cpp
    softcut/Svf.cpp
    softcut/TestBuffers.cpp
)

target_include_directories(${PROJECT_NAME}
  SYSTEM PRIVATE /usr/local/include
)

#target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
