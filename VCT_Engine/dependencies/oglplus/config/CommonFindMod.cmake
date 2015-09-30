#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
macro(oglplus_common_find_module PREFIX PC_NAME HEADER LIBRARY)
	unset(${PREFIX}_FOUND)
	unset(${PREFIX}_DEFINITIONS)
	unset(${PREFIX}_INCLUDE_DIRS)
	unset(${PREFIX}_LIBRARY_DIRS)
	unset(${PREFIX}_LIBRARIES)

	if(PKG_CONFIG_FOUND)
		pkg_check_modules(PC_${PREFIX} QUIET ${PC_NAME})
		if(PC_${PREFIX}_FOUND)
			find_library(
				${PREFIX}_FULL_LIB_PATH NAMES ${PC_${PREFIX}_LIBRARIES}
				PATHS ${PC_${PREFIX}_LIBRARY_DIRS}
				NO_DEFAULT_PATH
			)
			if("${${PREFIX}_FULL_LIB_PATH}" MATCHES "${CMAKE_STATIC_LIBRARY_SUFFIX}$")
				set(${PREFIX}_DEFINITIONS "${PC_${PREFIX}_STATIC_DEFINITIONS}")
				set(${PREFIX}_INCLUDE_DIRS "${PC_${PREFIX}_STATIC_INCLUDE_DIRS}")
				set(${PREFIX}_LIBRARY_DIRS "${PC_${PREFIX}_STATIC_LIBRARY_DIRS}")
				set(${PREFIX}_LIBRARIES "${PC_${PREFIX}_STATIC_LIBRARIES}")
				foreach(TMP_LIBRARY ${PC_${PREFIX}_STATIC_LIBRARIES})
					find_library(
						${TMP_LIBRARY}_FULL_LIB_PATH NAMES ${TMP_LIBRARY}
						HINTS ${PC_${PREFIX}_STATIC_LIBRARY_DIRS}
					)
					set(${PREFIX}_LIBRARIES ${${PREFIX}_LIBRARIES} ${${TMP_LIBRARY}_FULL_LIB_PATH})
					unset(TMP_FULL_LIB_PATH)
				endforeach()
			else()
				set(${PREFIX}_DEFINITIONS "${PC_${PREFIX}_DEFINITIONS}")
				set(${PREFIX}_INCLUDE_DIRS "${PC_${PREFIX}_INCLUDE_DIRS}")
				set(${PREFIX}_LIBRARY_DIRS "${PC_${PREFIX}_LIBRARY_DIRS}")
				foreach(TMP_LIBRARY ${PC_${PREFIX}_LIBRARIES})
					find_library(
						${TMP_LIBRARY}_FULL_LIB_PATH NAMES ${TMP_LIBRARY}
						HINTS ${LIBRARY_SEARCH_PATHS} ${PC_${PREFIX}_LIBRARY_DIRS}
					)
					set(${PREFIX}_LIBRARIES ${${PREFIX}_LIBRARIES} ${${TMP_LIBRARY}_FULL_LIB_PATH})
					unset(TMP_FULL_LIB_PATH)
				endforeach()
			endif()
			unset(${PREFIX}_FULL_LIB_PATH)
		endif()
	endif()

	if(NOT(${PREFIX}_INCLUDE_DIRS))
		unset(${PREFIX}_INCLUDE_DIRS)
		find_path(
			${PREFIX}_INCLUDE_DIRS ${HEADER}
			PATHS ${HEADER_SEARCH_PATHS}
			NO_DEFAULT_PATH
		)
	endif()
	if(NOT(${PREFIX}_INCLUDE_DIRS))
		unset(${PREFIX}_INCLUDE_DIRS)
		find_path(${PREFIX}_INCLUDE_DIRS ${HEADER})
	endif()

	if(NOT ${PREFIX}_LIBRARIES)
		find_library(
			${PREFIX}_LIBRARIES NAMES ${LIBRARY}
			PATHS ${LIBRARY_SEARCH_PATHS}
			NO_DEFAULT_PATH
		)
	endif()
	if(NOT ${PREFIX}_LIBRARIES)
		find_library(${PREFIX}_LIBRARIES NAMES ${LIBRARY})
	endif()

	set(${PREFIX}_FOUND 0)
	if((${PREFIX}_INCLUDE_DIRS) AND (${PREFIX}_LIBRARIES))

		if(EXISTS ${PROJECT_SOURCE_DIR}/config/ext_lib/test_${PREFIX}.cpp)
			if(NOT ${PREFIX}_LIBRARIES)
				set(${PREFIX}_LIBRARIES "")
			endif()
			configure_file(
				${PROJECT_SOURCE_DIR}/config/ext_lib/test_${PREFIX}.cpp
				${PROJECT_BINARY_DIR}/ext_lib/test_${PREFIX}.cpp
			)
			try_run(
				RUNS_WITH_${PREFIX} COMPILES_WITH_${PREFIX}
				"${PROJECT_BINARY_DIR}/ext_lib"
				"${PROJECT_BINARY_DIR}/ext_lib/test_${PREFIX}.cpp"
				COMPILE_DEFINITIONS
					"${OGLPLUS_CPP_STD_COMPILER_SWITCH} ${${PREFIX}_DEFINITIONS}"
				CMAKE_FLAGS
					"-DINCLUDE_DIRECTORIES:STRING=${${PREFIX}_INCLUDE_DIRS} "
					"-DLIBRARY_DIRECTORIES:STRING=${${PREFIX}_LIBRARY_DIRS} "
					"-DLINK_DIRECTORIES:STRING=${${PREFIX}_LIBRARY_DIRS} "
					"-DLINK_LIBRARIES:STRING=${${PREFIX}_LIBRARIES} "
			)
			if(COMPILES_WITH_${PREFIX})
				if(RUNS_WITH_${PREFIX} EQUAL 0)
					set(${PREFIX}_FOUND 1)
					message(STATUS
						"Found ${PREFIX}: "
						"${${PREFIX}_INCLUDE_DIRS} "
						"${${PREFIX}_LIBRARIES}"
					)
				else()
					message(STATUS "Could NOT execute with ${PREFIX}")
				endif()
			else()
				message(STATUS "Could NOT compile or link with ${PREFIX}")
			endif()
			unset(RUNS_WITH_${PREFIX})
			unset(COMPILES_WITH_${PREFIX})
		else()
			set(${PREFIX}_FOUND 1)
			message(STATUS "Found ${PREFIX}: ${${PREFIX}_INCLUDE_DIRS} ${${PREFIX}_LIBRARIES}")
		endif()
	else()
		message(STATUS "Could NOT find ${PREFIX}")
	endif()
endmacro()
