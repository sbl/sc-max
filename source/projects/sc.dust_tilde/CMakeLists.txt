include(${CMAKE_CURRENT_SOURCE_DIR}/../scmax-pretarget.cmake)

add_library(
	${PROJECT_NAME}
	MODULE
	${PROJECT_NAME}.cpp
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../scmax-posttarget.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-api/script/max-posttarget.cmake)
