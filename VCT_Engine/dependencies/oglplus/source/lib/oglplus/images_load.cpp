/**
 *  .file lib/oglplus/images_load.cpp
 *  .brief Bitmap image loading functions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "prologue.ipp"
#include <oglplus/string/def.hpp>
#include <oglplus/string/empty.hpp>
#include <oglplus/string/utf8.hpp>
#include <oglplus/data_type.hpp>
#include <oglplus/pixel_data.hpp>
#include <oglplus/images/image.hpp>

#include "implement.ipp"

#include <oglplus/images/xpm.hpp>
#if OGLPLUS_PNG_FOUND
#include <oglplus/images/png.hpp>
#endif
#include <oglplus/images/load.hpp>
#include "epilogue.ipp"
