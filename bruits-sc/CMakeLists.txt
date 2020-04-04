cmake_minimum_required(VERSION 3.5)
project("bruits")

set(BRUITS_BASE_PATH ${CMAKE_SOURCE_DIR}/..)

set(BRUITS_CMAKE_MODULES ${CMAKE_SOURCE_DIR}/cmake_modules)
set(SC_PATH "${BRUITS_BASE_PATH}/third_party/supercollider")

set(CMAKE_MODULE_PATH ${BRUITS_CMAKE_MODULES} ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX)
    message(WARNING "No install prefix provided, defaulting to $BUILD_DIR/install")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

message(STATUS "Install directory set to: ${CMAKE_INSTALL_PREFIX}")

#####################################################################
# testing

enable_testing()

#####################################################################
# subprojects

add_subdirectory(src/Griesinger)
add_subdirectory(src/MS20)
add_subdirectory(src/VCM)
add_subdirectory(src/Varshape)
add_subdirectory(src/Warsaw)
add_subdirectory(src/Wellen)
add_subdirectory(src/Zopf)
