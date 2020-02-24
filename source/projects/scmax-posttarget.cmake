#set(C74_CXX_STANDARD 98)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    
    "${C74_INCLUDES}"
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${SC_PATH}/include/common
	${SC_PATH}/include/plugin_interface
)

include(${C74_MAX_API_DIR}/script/max-posttarget.cmake)