/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_basic

#ifndef OGLPLUS_LOW_PROFILE
# define OGLPLUS_LOW_PROFILE 0 /*<
Compile-time option enabling the ['low-profile] mode.
In the low-profile mode some features (like object descriptions,
enumeration value names, some file and line info atached to exceptions,
etc. useful during development and debugging) are disabled, resulting
in both smaller binary executable sizes and lower run-time overhead
for release builds of applications using OGLplus.

Setting this option to a non-zero integer value enables the low-profile
mode, setting it to zero disables it. By default the low-profile mode
is disabled.

This option influences the default value of several other configuration
options. Their values can of course be set independently if required.
>*/
#endif

#ifndef OGLPLUS_LINK_LIBRARY
# define OGLPLUS_LINK_LIBRARY 0 /*<
Compile-time switch enabling linking of some parts of OGLplus from a binary library.
Certain parts of OGLplus (mostly complicated functions and functions where
static variables or long string literals are used) can be built into a separate
library that can be then linked to applications. If this option is set to
zero, then everything is inlined. If it is set to a nonzero integer value,
then some functions are just declared and must be built separatelly and
linked to the final executable.

The [^oglplus/lib.hpp] header file can be included into one of the translation units
when building the library or the final executable to include the implementations
of all such functions.

By default this option is set to 0 - everything is inlined.
>*/
#endif

#ifndef OGLPLUS_NO_SITE_CONFIG /*<
Disables the usage of the automatically-generated
[link oglplus.quickref.config.site site configuration header].
>*/
# include <oglplus/config/site.hpp>
#endif

//]

