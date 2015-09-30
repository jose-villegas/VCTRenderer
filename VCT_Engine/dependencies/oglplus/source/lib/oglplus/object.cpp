/**
 *  .file lib/oglplus/object.cpp
 *  .brief GL object-related functions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "prologue.ipp"

#include <oglplus/string/def.hpp>
#include <oglplus/string/empty.hpp>
#include <oglplus/string/utf8.hpp>
#include <oglplus/error/basic.hpp>
#include <oglplus/error/glfunc.hpp>
#include <oglplus/error/limit.hpp>
#include <oglplus/error/object.hpp>
#include <oglplus/error/framebuffer.hpp>
#include <oglplus/compare_function.hpp>
#include <oglplus/data_type.hpp>
#include <oglplus/pixel_data.hpp>
#include <oglplus/access_specifier.hpp>
#include <oglplus/texture_target.hpp>
#include <oglplus/texture_filter.hpp>
#include <oglplus/texture_compare.hpp>
#include <oglplus/texture_swizzle.hpp>
#include <oglplus/texture_wrap.hpp>
#include <oglplus/buffer_target.hpp>
#include <oglplus/buffer_usage.hpp>
#include <oglplus/buffer_map_access.hpp>
#include <oglplus/buffer_storage_bit.hpp>
#include <oglplus/framebuffer_target.hpp>
#include <oglplus/framebuffer_attachment.hpp>
#include <oglplus/renderbuffer_target.hpp>
#include <oglplus/transform_feedback_target.hpp>
#include <oglplus/transform_feedback_mode.hpp>
#include <oglplus/transform_feedback_type.hpp>
#include <oglplus/color_buffer.hpp>

#include "implement.ipp"

#include <oglplus/object/desc.hpp>
#include <oglplus/texture.hpp>
#include <oglplus/sampler.hpp>
#include <oglplus/buffer.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/renderbuffer.hpp>
#include <oglplus/transform_feedback.hpp>

#if GL_EXT_direct_state_access

#include <oglplus/dsa/ext/buffer.hpp>
#include <oglplus/dsa/ext/framebuffer.hpp>
#include <oglplus/dsa/ext/renderbuffer.hpp>
#include <oglplus/dsa/ext/texture.hpp>

#endif

#include "epilogue.ipp"
