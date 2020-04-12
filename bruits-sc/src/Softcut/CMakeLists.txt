include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
    ${PROJECT_NAME}.cpp

    softcut-lib/src/FadeCurves.cpp
    softcut-lib/src/ReadWriteHead.cpp
    softcut-lib/src/SubHead.cpp
    softcut-lib/src/Svf.cpp
    softcut-lib/src/Voice.cpp

    softcut-lib/include/softcut/FadeCurves.h
    softcut-lib/include/softcut/Interpolate.h
    softcut-lib/include/softcut/ReadWriteHead.h
    softcut-lib/include/softcut/Resampler.h
    softcut-lib/include/softcut/SoftClip.h
    softcut-lib/include/softcut/Softcut.h
    softcut-lib/include/softcut/SubHead.h
    softcut-lib/include/softcut/Svf.h
    softcut-lib/include/softcut/Types.h
    softcut-lib/include/softcut/Voice.h
)

target_include_directories(${PROJECT_NAME}
  PRIVATE softcut-lib/include
  SYSTEM PRIVATE /usr/local/include
)

#target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
