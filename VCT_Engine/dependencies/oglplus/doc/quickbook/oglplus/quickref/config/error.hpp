/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_error

#ifndef OGLPLUS_ERROR_NO_FILE /*<
Disables the [^SourceFile] attribute of __Error.
Disabled in low-profile mode.
>*/
# define OGLPLUS_ERROR_NO_FILE __OGLPLUS_LOW_PROFILE
#endif

#ifndef OGLPLUS_ERROR_NO_LINE /*<
Disables the [^SourceLine] attribute of __Error.
Disabled in low-profile mode.
>*/
# define OGLPLUS_ERROR_NO_LINE __OGLPLUS_LOW_PROFILE
#endif

#ifndef OGLPLUS_ERROR_NO_FUNC /*<
Disables the [^SourceFunc] attribute of __Error.
Disabled in low-profile mode.
>*/
# define OGLPLUS_ERROR_NO_FUNC __OGLPLUS_LOW_PROFILE
#endif

#ifndef OGLPLUS_ERROR_NO_GL_LIB /*<
Disables the [^GLLib] attribute of __Error.
Enabled by default.
>*/
# define OGLPLUS_ERROR_NO_GL_LIB 0
#endif

#ifndef OGLPLUS_ERROR_NO_GL_FUNC /*<
Disables the [^GLFunc] attribute of __Error.
Enabled by default.
>*/
# define OGLPLUS_ERROR_NO_GL_FUNC 0
#endif

#ifndef OGLPLUS_ERROR_NO_GL_SYMBOL /*<
Disables the [^EnumParam], [^EnumParamName] and [^Index]
attributes of __Error.
Enabled by default.
>*/
# define OGLPLUS_ERROR_NO_GL_SYMBOL 0
#endif

//]

