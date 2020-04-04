cmake_minimum_required(VERSION 3.0)
project(bruits-max)


set(BRUITS_BASE_PATH ${CMAKE_SOURCE_DIR}/..)

string(REGEX REPLACE "(.*)/" "" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")


if (${CMAKE_GENERATOR} MATCHES "Xcode")
 		if (${XCODE_VERSION} VERSION_LESS 9)
  			message(STATUS "Xcode 9 or higher is required. Please install from the Mac App Store.")
			return ()
		endif ()
endif ()


# Fetch the correct version of the min-api
message(STATUS "Updating Git Submodules")
execute_process(
	COMMAND				git submodule update --init --recursive
	WORKING_DIRECTORY	"${CMAKE_CURRENT_SOURCE_DIR}"
)

# Misc setup and subroutines
include(${BRUITS_BASE_PATH}/third_party/min-api/script/min-package.cmake)


# Add unit tests for the API
# enable_testing()


add_subdirectory(${BRUITS_BASE_PATH}/third_party/min-api
  ${CMAKE_CURRENT_BINARY_DIR}/min-api
  )

# common libs
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/bruits)


# Generate a project for every folder in the "source/projects" folder
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach (project_dir ${PROJECT_DIRS})
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
    message("Generating: ${project_dir}")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
  endif ()
endforeach ()
