/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_site_1

#define OGLPLUS_OPENAL_FOUND __UNSPECIFIED /*<
Indicates if __OpenAL__ is available and should be used.
>*/
#define OGLPLUS_PNG_FOUND __UNSPECIFIED /*<
Indicates if __LibPNG__ was found and should be used.
>*/
#define OGLPLUS_PANGO_CAIRO_FOUND __UNSPECIFIED /*<
Indicates if __Pango_Cairo__ was found and should be used.
>*/

#ifndef __OGLPLUS_LOW_PROFILE
#define __OGLPLUS_LOW_PROFILE __UNSPECIFIED /*<
The low-profile mode can be set through the configuration script.
>*/
#endif

//]
//[oglplus_config_site_2

#ifndef OGLPLUS_USE_GLCOREARB_H /*<
Indicates that [^GL/glcorearb.h] + the GL binary library
should be used.
>*/
# define OGLPLUS_USE_GLCOREARB_H __UNSPECIFIED
#endif
#ifndef OGLPLUS_USE_GL3_H /*<
Indicates that [^GL3/gl3.h] + the GL binary library
should be used.
>*/
# define OGLPLUS_USE_GL3_H __UNSPECIFIED
#endif
#ifndef OGLPLUS_USE_GLEW /*<
Indicates that __GLEW__ should be used.
>*/
# define OGLPLUS_USE_GLEW __UNSPECIFIED
#endif
#ifndef OGLPLUS_USE_GL3W /*<
Indicates that __GL3W__ should be used.
>*/
# define OGLPLUS_USE_GL3W __UNSPECIFIED
#endif

//]
//[oglplus_config_site_3

#ifndef OGLPLUS_USE_BOOST_CONFIG
# define OGLPLUS_USE_BOOST_CONFIG __UNSPECIFIED
#endif

//]
//[oglplus_config_site_4
#if !OGLPLUS_USE_BOOST_CONFIG

#ifndef OGLPLUS_NO_SCOPED_ENUMS /*<
Indicates that C++11 strongly typed enumerations are not supported.
>*/
# define OGLPLUS_NO_SCOPED_ENUMS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_VARIADIC_MACROS /*<
Indicates that preprocessor variadic macros are not supported.
>*/
# define OGLPLUS_NO_VARIADIC_MACROS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_VARIADIC_TEMPLATES /*<
Indicates that C++11 variadic templates are not properly supported.
>*/
# define OGLPLUS_NO_VARIADIC_TEMPLATES __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_UNIFIED_INITIALIZATION_SYNTAX /*<
Indicates that C++11 unified initialization syntax is not implemented.
>*/
# define OGLPLUS_NO_UNIFIED_INITIALIZATION_SYNTAX __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_INITIALIZER_LISTS /*<
Indicates that C++11 initializer lists are not supported.
>*/
# define OGLPLUS_NO_INITIALIZER_LISTS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_DEFAULTED_FUNCTIONS /*<
Indicates that C++11 [^default]ed functions are not supported.
>*/
# define OGLPLUS_NO_DEFAULTED_FUNCTIONS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_DELETED_FUNCTIONS /*<
Indicates that C++11 [^delete]d functions are not supported.
>*/
# define OGLPLUS_NO_DELETED_FUNCTIONS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_EXPLICIT_CONVERSION_OPERATORS /*<
Indicates that C++11 explicit conversion operators are not supported.
>*/
# define OGLPLUS_NO_EXPLICIT_CONVERSION_OPERATORS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS /*<
Indicates that C++11 default template arguments cannot be used.
>*/
# define OGLPLUS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_UNICODE_LITERALS /*<
Indicates that C++11 unicode string literals are not supported.
>*/
# define OGLPLUS_NO_UNICODE_LITERALS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_USER_DEFINED_LITERALS /*<
Indicates that C++11 user-defined literals are not supported.
>*/
# define OGLPLUS_NO_USER_DEFINED_LITERALS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_TEMPLATE_ALIASES /*<
Indicates that C++11 template aliases are not supported.
>*/
# define OGLPLUS_NO_TEMPLATE_ALIASES __UNSPECIFIED
#endif
//]
//[oglplus_config_site_5

#ifndef OGLPLUS_NO_CONSTEXPR /*<
Indicates that C++11 generalized constant expressions are not supported.
>*/
# define OGLPLUS_NO_CONSTEXPR __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_OVERRIDE /*<
Indicates that C++11 explicit virtual override is not supported.
>*/
# define OGLPLUS_NO_OVERRIDE __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_NOEXCEPT /*<
Indicates that C++11 [^noexcept] specifier is not supported.
>*/
# define OGLPLUS_NO_NOEXCEPT __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_LAMBDAS /*<
Indicates that C++11 lambdas are not supported.
>*/
# define OGLPLUS_NO_LAMBDAS __UNSPECIFIED
#endif

#endif //!OGLPLUS_USE_BOOST_CONFIG
//]
//[oglplus_config_site_6

#ifndef OGLPLUS_NO_INHERITED_CONSTRUCTORS /*<
Indicates that C++11 inherited constructors are not supported.
>*/
# define OGLPLUS_NO_INHERITED_CONSTRUCTORS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_CHRONO /*<
Indicates that the standard [^chrono] header is not available.
>*/
# define OGLPLUS_NO_CHRONO __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_THREADS /*<
Indicates that the C++11 threads implementation is not available.
>*/
# define OGLPLUS_NO_THREADS __UNSPECIFIED
#endif

#ifndef OGLPLUS_NO_SCOPED_ENUM_TEMPLATE_PARAMS /*<
Indicates that using C++11 scoped enumerations as template parameters
is not supported or not implemented correctly.
>*/
#define OGLPLUS_NO_SCOPED_ENUM_TEMPLATE_PARAMS __UNSPECIFIED
#endif

//]

