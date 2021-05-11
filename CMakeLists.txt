cmake_minimum_required(VERSION 3.8) # First version with CMP0067
project(adv-mancala VERSION 0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
cmake_policy(SET CMP0067 NEW) # Force CMake to observe CXX_STANDARD in checks

##
## HAVE is for symbols and headers
## FOUND is for packages
##

# Give some default build types to use
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo" "Coverage")
endif()

# This expects you to have modules. Uncomment if you do
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# CTest if this is the top-level project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
endif()

include(CheckIncludeFileCXX)
include(CheckIncludeFile)
include(CheckCXXCompilerFlag)

function (add_compile_option_if_available ARG_NAME)
	string(REGEX REPLACE "^-|\ " "" ARG_STRIPPED "${ARG_NAME}")
	string(REGEX REPLACE "-" "_" ARG_STRIP_UNDER "${ARG_STRIPPED}")
	string(TOUPPER "HAVE_${ARG_STRIP_UNDER}" HAVE_ARG_NAME)

	check_cxx_compiler_flag("${ARG_NAME}" ${HAVE_ARG_NAME})
	if (${HAVE_ARG_NAME})
		add_compile_options("${ARG_NAME}")
	endif()
endfunction() # add_compile_option_if_available

# Put any cache options here


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# Enable most compiler warnings if a debug build
	if (MSVC)
		add_compile_options("/W4" "/WX")
	else()
		add_compile_options("-Wall" "-Wextra" "-pedantic")
	endif()
elseif (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	# Coverage should only be run on linux

	# Adds gcov options
	add_compile_options("-g" "-fprofile-arcs" "-ftest-coverage")
	link_libraries(gcov)

	# Force use of absolute paths
	add_compile_option_if_available("-fprofile-abs-path")

	# Disable compiler optimizations
	check_cxx_compiler_flag("-fkeep-inline-functions" HAVE_KEEP_INLINE)
	check_cxx_compiler_flag("-fkeep-static-functions" HAVE_KEEP_STATIC)
	
	add_compile_options("-O0" "$<$<BOOL:${HAVE_KEEP_INLINE}>:-fkeep-inline-functions>" "$<$<BOOL:${HAVE_KEEP_STATIC}>:-fkeep-static-functions>")
endif()

# Add the binary directory to includes to be safe
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_library(MATH_FOUND m)

#[[ Don't have cmakeconfig.h
if ($<VERSION_GREATER_EQUAL:"${CMAKE_VERSION}","3.12">)
	add_compile_definitions(HAVE_CMAKE_CONFIG_H)
else()
	add_compile_options("-DHAVE_CMAKE_CONFIG_H")
endif()

configure_file("${PROJECT_SOURCE_DIR}/cmakeconfig.h.in" "${PROJECT_BINARY_DIR}/cmakeconfig.h" ESCAPE_QUOTES @ONLY)
#]]

# Assumes your directory tree is as follows
# CMakeLists.txt
# test
# + CMakeLists.txt with build info
# src
# + CMakeLists.txt with test build info and other test stuff
add_subdirectory(src)


# Only enable testing if this is the top-level project or it's a test build
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/" AND (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR adv_mancala_BUILD_TESTING) AND BUILD_TESTING)
	add_subdirectory(test)
endif()
