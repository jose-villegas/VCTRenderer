/**
 *  .file lib/oglplus/prog_var.cpp
 *  .brief GLSL and Shader and shader variable-related functions
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
#include <oglplus/error/glfunc.hpp>
#include <oglplus/error/prog_var.hpp>
#include <oglplus/error/limit.hpp>
#include <oglplus/data_type.hpp>
#include <oglplus/shader_type.hpp>
#include <oglplus/object/type.hpp>

#include "implement.ipp"

#include <oglplus/prog_var/typecheck.hpp>
#include <oglplus/uniform.hpp>
#include <oglplus/uniform_block.hpp>
#include <oglplus/uniform_subroutines.hpp>
#include <oglplus/vertex_attrib.hpp>
#include <oglplus/frag_data.hpp>
#include <oglplus/shader_storage_block.hpp>
#include "epilogue.ipp"
