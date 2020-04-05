include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
    ${PROJECT_NAME}.cpp
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
