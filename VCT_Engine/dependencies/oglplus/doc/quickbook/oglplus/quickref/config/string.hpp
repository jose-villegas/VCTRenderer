/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_string

#ifndef OGLPLUS_NO_UTF8_CHECKS /*<
Compile-time switch disabling UTF-8 validity checks in various functions.
Setting this preprocessor symbol to a nonzero value causes that
the __String constructors skip their UTF-8 validity checks.
By default this option is set to the same value as __OGLPLUS_LOW_PROFILE,
i.e. the UTF-8 validity checks are enabled, when not in low-profile mode
and disabled otherwise.
>*/
# define OGLPLUS_NO_UTF8_CHECKS __OGLPLUS_LOW_PROFILE
#endif

//]

