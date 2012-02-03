# - Try to find OpenNI
# Once done, this will define
#
#  OpenNI_FOUND - system has OpenNI
#  OpenNI_INCLUDE_DIRS - the OpenNI include directories
#  OpenNI_LIBRARIES - link these to use OpenNI 

#include(LibFindMacros)

# Dependencies

# Use pkg-config to get hints about paths
find_package(PkgConfig)
pkg_check_modules(OpenNI_PKGCONF OpenNI)

# Include dir
find_path(OpenNI_INCLUDE_DIR
  NAMES ni/XnOpenNI.h
  HINTS ${OpenNI_PKGCONF_INCLUDE_DIRS}
)

if(OpenNI_INCLUDE_DIR STREQUAL "OpenNI_INCLUDE_DIR-NOTFOUND")
	message(STATUS "Looking for OpenNI in default dirs")
	find_path(OpenNI_INCLUDE_DIR NAMES ni/XnOpenNI.h )
endif()

# Finally the library itself
find_library(OpenNI_LIBRARY
  NAMES OpenNI
  PATHS ${OpenNI_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
#set(OpenNI_PROCESS_INCLUDES OpenNI_INCLUDE_DIR OpenNI_INCLUDE_DIRS)
#set(OpenNI_PROCESS_LIBS OpenNI_LIBRARY OpenNI_LIBRARIES)
#libfind_process(OpenNI)

set(OpenNI_LIBRARIES ${OpenNI_LIBRARY} )
set(OpenNI_LIBS ${OpenNI_LIBRARY} )
set(OpenNI_INCLUDE_DIRS ${OpenNI_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(OpenNI  DEFAULT_MSG
                                  OpenNI_LIBRARY OpenNI_INCLUDE_DIR)

mark_as_advanced(OpenNI_INCLUDE_DIR OpenNI_LIBRARY )
