cmake_minimum_required(VERSION 3.7)
project(softcut)

set(CMAKE_CXX_STANDARD 14)

set(SRC
        src/Voice.cpp
        src/ReadWriteHead.cpp
        src/SubHead.cpp
        src/FadeCurves.cpp
        src/Svf.cpp)


add_library(${PROJECT_NAME} STATIC ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
  include
  src

  SYSTEM PUBLIC /usr/local/include
  )
