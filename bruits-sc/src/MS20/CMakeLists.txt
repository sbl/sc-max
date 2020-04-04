include(${BRUITS_CMAKE_MODULES}/scplugin-pretarget.cmake)

### what goes into here

add_library(${PROJECT_NAME} MODULE
  MS20.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/includes
)

include(${BRUITS_CMAKE_MODULES}/scplugin-posttarget.cmake)
