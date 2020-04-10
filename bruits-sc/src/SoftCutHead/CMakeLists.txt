include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
    ${PROJECT_NAME}.cpp
    interp.h
    SoftCutHeadLogic.h
    SoftCutHeadLogic.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
