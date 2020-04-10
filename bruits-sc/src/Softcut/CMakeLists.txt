include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-pretarget.cmake)

### what goes into here

add_subdirectory(softcut-lib)

add_library(${PROJECT_NAME} MODULE
    ${PROJECT_NAME}.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE softcut-lib/include)
target_link_libraries(${PROJECT_NAME} PRIVATE softcut)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/scplugin-posttarget.cmake)
