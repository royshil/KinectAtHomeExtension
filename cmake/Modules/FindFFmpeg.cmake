find_package(PkgConfig)


macro(FFMPEG_FIND varname shortname headername)

	pkg_check_modules(PC_${varname} ${shortname})

	find_path(${varname}_INCLUDE_DIR "${shortname}/${headername}" 
		HINTS ${PC_${varname}_INCLUDEDIR} ${PC_${varname}_INCLUDE_DIRS}
		NO_DEFAULT_PATH
		)
	
	if(${varname}_INCLUDE_DIR STREQUAL "${varname}_INCLUDE_DIR-NOTFOUND")
		message(STATUS "look for newer strcture")
		pkg_check_modules(PC_${varname} "lib${shortname}")

		find_path(${varname}_INCLUDE_DIR "lib${shortname}/${headername}"
			HINTS ${PC_${varname}_INCLUDEDIR} ${PC_${varname}_INCLUDE_DIRS}
			NO_DEFAULT_PATH
			)
	endif()

	find_library(${varname}_LIBRARIES names ${shortname}
		HINTS ${PC_${varname}_LIBDIR} ${PC_${varname}_LIBRARY_DIR})

	if(${varname}_LIBRARIES STREQUAL "${varname}_LIBRARIES-NOTFOUND")
		message(STATUS "look for newer structure for library")
		find_library(${varname}_LIBRARIES names lib${shortname}
			HINTS ${PC_${varname}_LIBDIR} ${PC_${varname}_LIBRARY_DIR})
	endif()

	if(NOT ${varname}_INCLUDE_DIR STREQUAL "${varname}_INCLUDE_DIR-NOTFOUND"
		AND NOT ${varname}_LIBRARIES STREQUAL ${varname}_LIBRARIES-NOTFOUND)

		message(STATUS "found ${shortname}: include ${${varname}_INCLUDE_DIR} lib ${${varname}_LIBRARIES}")
		SET(FFMPEG_${varname}_FOUND 1)
		SET(FFMPEG_${varname}_INCLUDE_DIRS ${${varname}_INCLUDE_DIR})
		SET(FFMPEG_${varname}_LIBS ${${varname}_LIBRARIES})
	else()
		message(STATUS "Can't find ${shortname}")
	endif()

endmacro(FFMPEG_FIND)

FFMPEG_FIND(LIBAVFORMAT avformat avformat.h)
FFMPEG_FIND(LIBAVDEVICE avdevice avdevice.h)
FFMPEG_FIND(LIBAVCODEC  avcodec  avcodec.h)
FFMPEG_FIND(LIBAVUTIL   avutil   avutil.h)
FFMPEG_FIND(LIBSWSCALE  swscale  swscale.h)

SET(FFMPEG_FOUND "NO")
IF   (FFMPEG_LIBAVFORMAT_FOUND AND 
	FFMPEG_LIBAVDEVICE_FOUND AND 
	FFMPEG_LIBAVCODEC_FOUND AND 
	FFMPEG_LIBAVUTIL_FOUND AND
	FFMPEG_LIBSWSCALE_FOUND
	)

    SET(FFMPEG_FOUND "YES")

    SET(FFMPEG_INCLUDE_DIRS ${FFMPEG_LIBAVFORMAT_INCLUDE_DIRS})

    SET(FFMPEG_LIBRARY_DIRS ${FFMPEG_LIBAVFORMAT_LIBRARY_DIRS})

    SET(FFMPEG_LIBRARIES
        ${FFMPEG_LIBAVFORMAT_LIBRARIES}
        ${FFMPEG_LIBAVDEVICE_LIBRARIES}
        ${FFMPEG_LIBAVCODEC_LIBRARIES}
        ${FFMPEG_LIBAVUTIL_LIBRARIES}
	${FFMPEG_LIBSWSCALE_LIBRARIES}
	)

ELSE ()

   MESSAGE(STATUS "Could not find FFMPEG")

ENDIF()
