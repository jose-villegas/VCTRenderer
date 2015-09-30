/**
 *  .file lib/oglplus/shapes_utils.cpp
 *  .brief Shape utility functions
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
#include <oglplus/error/object.hpp>
#include <oglplus/error/prog_var.hpp>
#include <oglplus/error/limit.hpp>
#include <oglplus/vertex_array.hpp>
#include <oglplus/vertex_attrib.hpp>
#include <oglplus/buffer.hpp>
#include <oglplus/program.hpp>
#include <oglplus/context.hpp>
#include <oglplus/face_mode.hpp>
#include <oglplus/data_type.hpp>
#include <oglplus/primitive_type.hpp>

#include "implement.ipp"

#include <oglplus/shapes/draw.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/analyzer.hpp>
#include <oglplus/shapes/analyzer_data.hpp>
#include "epilogue.ipp"
