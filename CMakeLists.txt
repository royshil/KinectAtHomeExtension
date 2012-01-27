#/**********************************************************\ 
# 
# Auto-generated CMakeLists.txt for the Kinect@Home Plugin project
#
#\**********************************************************/

# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

Project(${PLUGIN_NAME})

file (GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
	cmake/Modules/*.cmake
    )

include_directories(${PLUGIN_INCLUDE_DIRS})

# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
        GENERATED 1
    )

SOURCE_GROUP(Generated FILES
    ${GENERATED}
    )

SET( SOURCES
    ${GENERAL}
    ${GENERATED}
    )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

find_library(OPENGL OpenGL)

find_package(FFmpeg)
if(${FFMPEG_LIBSWSCALE_FOUND})
	message(STATUS "found swscale at ${FFMPEG_LIBSWSCALE_INCLUDE_DIRS} and ${FFMPEG_LIBAVUTIL_INCLUDE_DIRS}")
	include_directories(${FFMPEG_LIBSWSCALE_INCLUDE_DIRS})
	include_directories(${FFMPEG_LIBAVUTIL_INCLUDE_DIRS})
else(${FFMPEG_LIBSWSCALE_FOUND})
	message(STATUS "can't find SWSCALE!!")
endif(${FFMPEG_LIBSWSCALE_FOUND})

find_package(x264)
if(${X264_FOUND})
	include_directories(${X264_INCLUDE_DIRS})
endif()

# This will include Win/projectDef.cmake, X11/projectDef.cmake, Mac/projectDef 
# depending on the platform
include_platform()
