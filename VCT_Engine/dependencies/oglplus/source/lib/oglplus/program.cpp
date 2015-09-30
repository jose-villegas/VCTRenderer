/**
 *  .file lib/oglplus/program.cpp
 *  .brief Program and ProgramPipeline-related functions
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
#include <oglplus/error/glfunc.hpp>
#include <oglplus/error/program.hpp>
#include <oglplus/error/prog_var.hpp>
#include <oglplus/error/limit.hpp>
#include <oglplus/object/desc.hpp>
#include <oglplus/data_type.hpp>
#include <oglplus/precision_type.hpp>
#include <oglplus/shader_type.hpp>
#include <oglplus/program_pipeline_stage.hpp>
#include <oglplus/face_mode.hpp>
#include <oglplus/program_interface.hpp>
#include <oglplus/primitive_type.hpp>
#include <oglplus/transform_feedback_mode.hpp>

#include "implement.ipp"

#include <oglplus/detail/program.hpp>
#include <oglplus/detail/glsl_source.hpp>
#include <oglplus/detail/info_log.hpp>
#include <oglplus/shader.hpp>
#include <oglplus/program.hpp>
#include <oglplus/program_resource.hpp>
#include <oglplus/program_pipeline.hpp>
#include "epilogue.ipp"
