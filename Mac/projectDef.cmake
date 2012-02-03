#/**********************************************************\ 
# Auto-generated Mac project definition file for the
# Kinect@Home Plugin project
#\**********************************************************/

# Mac template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in Mac/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Mac/[^.]*.cpp
    Mac/[^.]*.h
    Mac/[^.]*.cmake
	Mac/[^.]*.mm
    )

# use this to add preprocessor definitions
add_definitions(
    
)

find_library(OPENGL_FRAMEWORK OpenGL)
find_library(QUARTZ_CORE_FRAMEWORK QuartzCore)
find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
find_package(OpenNI)

if(NOT OPENNI_FOUND)
	message(STATUS "Can't find OpenNI!")
else()
	message(STATUS "OpenNI found in ${OpenNI_INCLUDE_DIRS}")
endif()

include_directories(${CORE_FOUNDATION_FRAMEWORK})
include_directories(${OpenNI_INCLUDE_DIRS})

set(OPENNI_XML_FILE "/Users/royshilkrot/Downloads/NITE-Bin-MacOSX-v1.4.1.2/Data/Sample-Tracking.xml")
set_source_files_properties(
	${OPENNI_XML_FILE}
	PROPERTIES
	MACOSX_PACKAGE_LOCATION "Resources"
)


SOURCE_GROUP(Mac FILES ${PLATFORM})

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
	${OPENNI_XML_FILE}
    )

set(PLIST "Mac/bundle_template/Info.plist")
set(STRINGS "Mac/bundle_template/InfoPlist.strings")
set(LOCALIZED "Mac/bundle_template/Localized.r")

add_mac_plugin(${PROJECT_NAME} ${PLIST} ${STRINGS} ${LOCALIZED} SOURCES)


# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
	${OPENGL_FRAMEWORK}
	${QUARTZ_CORE_FRAMEWORK}
	${FFMPEG_LIBSWSCALE_LIBS}
	${FFMPEG_LIBAVUTIL_LIBS}
	${X264_LIBS}
	${OpenNI_LIBRARIES}
    )
