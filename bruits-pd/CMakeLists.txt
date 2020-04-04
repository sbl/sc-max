# cmake config adapted from https://github.com/TheTechnobear/Mi4Pd

cmake_minimum_required(VERSION 2.8.11)

project(externs)

set(PROJECT_NAME externs)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

if (APPLE)
    set(EXECUTABLE_NAME Mi4Pd)
elseif (UNIX)
    set(EXECUTABLE_NAME mi4pd)
endif (APPLE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -funroll-loops -fomit-frame-pointer")


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")

if (APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTEST")
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTEST")
endif (APPLE)


set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
if (NOT APPLE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/:$ORIGIN/../lib")
else()
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib;@loader_path")
  set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR TRUE)
endif()

include_directories(
        "${PROJECT_SOURCE_DIR}/../third_party/pd"
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/externs)

############
add_subdirectory(src/quantizer)
