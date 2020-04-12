include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
    ${PROJECT_NAME}.cpp

    softcut/FadeCurves.cpp
    softcut/SoftCutHead.cpp
    softcut/SoftCutVoice.cpp
    softcut/SubHead.cpp
    softcut/Svf.cpp

    softcut/FadeCurves.h
    softcut/Interpolate.h
    softcut/LowpassBrickwall.h
    softcut/Resampler.h
    softcut/SoftClip.h
    softcut/SoftCut.h
    softcut/SoftCutHead.h
    softcut/SoftCutVoice.h
    softcut/SubHead.h
    softcut/Svf.h
    softcut/TestBuffers.h
    softcut/Types.h
)

target_include_directories(${PROJECT_NAME}
  SYSTEM PRIVATE /usr/local/include
)

#target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
