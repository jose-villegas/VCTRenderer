/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_gl_apientry
#define GLAPIENTRY /*<
Definition of the [^GLAPIENTRY] calling convention used
by most of the GL API functions.
>*/
//]

//[oglplus_config_gl
#ifndef OGLPLUS_NO_GLFUNC_CHECKS /*<
Compile-time switch disabling checks of validity of pointers to GL functions.
Setting this preprocessor symbol to a nonzero value causes that
if the OpenGL functions are called through a pointer, then that pointer
is checked before it is used to call the function. If enabled and a pointer
to a GL function is nullptr then the __MissingFunction exception is thrown.
This check can safely be disabled if functions from the GL API are
not called through pointers.
By default this option is set to the same value as __OGLPLUS_LOW_PROFILE,
i.e. the function pointer checks are enabled, when not in low-profile mode,
and disabled otherwise. The check however requires variadic templates.
If variadic templates are not available, then the checks are disabled.
>*/
# define OGLPLUS_NO_GLFUNC_CHECKS __OGLPLUS_LOW_PROFILE
#endif

#ifndef OGLPLUS_NO_UNIFORM_TYPECHECK /*<
Compile-time switch entirely disabling typechecking of uniforms.
Setting this preprocessor symbol to a nonzero value causes
the Uniform variables are not typechecked.
By default this option is set to the same value as __OGLPLUS_LOW_PROFILE,
i.e. typechecking of uniforms is enabled when not in low-profile mode,
and disabled otherwise.
>*/
# define OGLPLUS_NO_UNIFORM_TYPECHECK __OGLPLUS_LOW_PROFILE
#endif

//]

