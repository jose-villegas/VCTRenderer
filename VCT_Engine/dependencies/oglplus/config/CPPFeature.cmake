#  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# we need C++11
if(${CMAKE_COMPILER_IS_GNUCXX})
	set(OGLPLUS_CPP_STD_COMPILER_SWITCH -std=c++11)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(OGLPLUS_CPP_STD_COMPILER_SWITCH -std=c++11)
endif()
# TODO add support for other compilers

add_definitions(${OGLPLUS_CPP_STD_COMPILER_SWITCH})

function(cpp_feature_detection FEATURE_NAME)
	if(NOT DEFINED OGLPLUS_NO_${FEATURE_NAME})
		configure_file(
			${PROJECT_SOURCE_DIR}/config/cpp/has_${FEATURE_NAME}.cpp
			${PROJECT_BINARY_DIR}/cpp/has_${FEATURE_NAME}.cpp
		)

		try_compile(
			OGLPLUS_HAS_${FEATURE_NAME}
			${PROJECT_BINARY_DIR}/cpp
			${PROJECT_BINARY_DIR}/cpp/has_${FEATURE_NAME}.cpp
			COMPILE_DEFINITIONS ${OGLPLUS_CPP_STD_COMPILER_SWITCH}
		)
		message(STATUS
			"Detecting support for c++ feature '${FEATURE_NAME}': "
			"${OGLPLUS_HAS_${FEATURE_NAME}}"
		)
		if(OGLPLUS_HAS_${FEATURE_NAME})
			set(OGLPLUS_NO_${FEATURE_NAME} 0 CACHE INTERNAL "")
			set(HAS_CPP_${FEATURE_NAME} TRUE PARENT_SCOPE)
		else()
			set(OGLPLUS_NO_${FEATURE_NAME} 1 CACHE INTERNAL "")
		endif()
		unset(OGLPLUS_HAS_${FEATURE_NAME})
	elseif(NOT ${OGLPLUS_NO_${FEATURE_NAME}})
		set(HAS_CPP_${FEATURE_NAME} TRUE PARENT_SCOPE)
	endif()
endfunction()

function(require_cpp_feature FEATURE_NAME)
	cpp_feature_detection(${FEATURE_NAME})

	if(${OGLPLUS_NO_${FEATURE_NAME}})
		message(FATAL_ERROR
			"C++ feature '${FEATURE_NAME}' is required "
			"but not supported by the currently used compiler!"
		)
	endif()

endfunction()

require_cpp_feature(SCOPED_ENUMS)

cpp_feature_detection(SCOPED_ENUM_TEMPLATE_PARAMS)
cpp_feature_detection(VARIADIC_MACROS)
cpp_feature_detection(VARIADIC_TEMPLATES)
cpp_feature_detection(UNIFIED_INITIALIZATION_SYNTAX)
cpp_feature_detection(INITIALIZER_LISTS)
cpp_feature_detection(DEFAULTED_FUNCTIONS)
cpp_feature_detection(DELETED_FUNCTIONS)
cpp_feature_detection(EXPLICIT_CONVERSION_OPERATORS)
cpp_feature_detection(FUNCTION_TEMPLATE_DEFAULT_ARGS)
cpp_feature_detection(UNICODE_LITERALS)
cpp_feature_detection(USER_DEFINED_LITERALS)
cpp_feature_detection(INHERITED_CONSTRUCTORS)
cpp_feature_detection(GENERALIZED_ATTRIBUTES)
cpp_feature_detection(TEMPLATE_ALIASES)
cpp_feature_detection(CONSTEXPR)
cpp_feature_detection(NOEXCEPT)
cpp_feature_detection(OVERRIDE)
cpp_feature_detection(LAMBDAS)
cpp_feature_detection(CHRONO)
cpp_feature_detection(THREADS)

