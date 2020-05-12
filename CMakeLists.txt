cmake_minimum_required(VERSION 3.0)
project(sc-max)

# Fetch the correct verion of the max-api
message(STATUS "Updating Git Submodules")
execute_process(
	COMMAND				git submodule update --init --recursive
	WORKING_DIRECTORY	"${CMAKE_CURRENT_SOURCE_DIR}"
)

# global PATHS, cmake -DSC_PATH=../.. -DMAX_API_PATH=../..
set(SC_PATH 		    "" CACHE PATH "supercollider path")
set(C74_MAX_API_DIR "" CACHE PATH "c74 max api path")

# Misc setup and subroutines
include(${C74_MAX_API_DIR}/script/max-package.cmake)


# Generate a project for every folder in the "source/projects" folder
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach (project_dir ${PROJECT_DIRS})
	if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
		message("Generating: ${project_dir}")
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
	endif ()
endforeach ()
