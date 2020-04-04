cmake_minimum_required(VERSION 3.0)

set(C74_MIN_API_DIR ${BRUITS_BASE_PATH}/third_party/min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)


#############################################################
# MAX EXTERNAL
#############################################################


include_directories(
	"${C74_INCLUDES}"
)

set(SOURCE_FILES ${PROJECT_NAME}.cpp)

add_library(
	${PROJECT_NAME}
	MODULE
	${SOURCE_FILES}
)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)


#############################################################
# UNIT TEST
#############################################################

#include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
