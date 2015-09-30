/**
 *  .file lib/oglplus/prologue.hpp
 *  .brief Common include file for the library source files
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifdef None
# pragma push_macro("None")
# undef None
# define OGLPLUS_None_WAS_DEFINED
#endif

#define OGLPLUS_LINK_LIBRARY 1

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>
#include <oglplus/config/basic.hpp>
