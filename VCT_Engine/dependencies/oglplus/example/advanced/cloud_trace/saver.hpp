/**
 *  @file advanced/cloud_trace/saver.hpp
 *  @brief Declares the saver class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_SAVER_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_SAVER_1119071146_HPP

#include "app_data.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <oglplus/context.hpp>
#include <oglplus/framebuffer.hpp>

namespace oglplus {
namespace cloud_trace {

struct RaytracerTarget;

class Saver
{
private:
	Context gl;
	DefaultFramebuffer dfb;
public:
	Saver(const AppData&);

	void SaveFrame(const AppData&, RaytracerTarget&, unsigned);
};

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
