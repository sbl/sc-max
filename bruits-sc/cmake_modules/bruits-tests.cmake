set(TEST_PROJECT ${PROJECT_NAME}_test)


# to speed up test compilation
add_library(main_test OBJECT 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/MainTest/main_test.cpp)

target_include_directories(main_test 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party)

# gather test files
file(GLOB TEST_SOURCES *_test.cpp)

add_executable(${TEST_PROJECT} ${TEST_SOURCES} $<TARGET_OBJECTS:main_test>)
target_include_directories(${TEST_PROJECT} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party)

add_test(NAME ${TEST_PROJECT} COMMAND ${TEST_PROJECT})
