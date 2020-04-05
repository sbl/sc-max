include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here


add_library(${PROJECT_NAME} MODULE
    ${PROJECT_NAME}.cpp

    wrDsp/wrFuncGen.c
    wrDsp/wrVtl.c
    wrLib/wrMath.c
)


target_include_directories(${PROJECT_NAME} PRIVATE wrDSP wrLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
