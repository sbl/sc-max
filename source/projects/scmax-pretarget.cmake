cmake_minimum_required(VERSION 3.0)

string(REGEX REPLACE "(.*)/" "" THIS_FOLDER_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_FOLDER_NAME})

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-api/script/max-pretarget.cmake)


include_directories(
	"${C74_INCLUDES}"
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}/../../supercollider/include/common
	${CMAKE_CURRENT_SOURCE_DIR}/../../supercollider/include/plugin_interface
)

if(APPLE)
	set(EXTRA_LIBS
	)
else()
	if(WIN64)
		set(EXTRA_LIBS
		)
	else ()
		set(EXTRA_LIBS
		)
	endif ()
endif()
