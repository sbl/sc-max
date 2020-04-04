project(quantizer)

add_library(quantizer SHARED
  quantizer~.cc
  quantizer_scales.h
  br_quantizer.cc
  br_quantizer.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "quantizer~")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
if (${APPLE})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pd_darwin")
elseif (${UNIX})
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pd_linux")
endif ()
