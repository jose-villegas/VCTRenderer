/**
 *  @file advanced/cloud_trace/textures.hpp
 *  @brief Declaration of texture classes
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_TEXTURES_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_TEXTURES_1119071146_HPP

#include "app_data.hpp"
#include "resources.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>
#include <oglplus/texture.hpp>
#include <oglplus/images/image.hpp>

namespace oglplus {
namespace cloud_trace {

struct CloudVolume
{
	images::Image image;

	CloudVolume(const AppData&);

	images::Image GenOrLoad(const AppData&);
	images::Image Generate(const AppData&);
	images::Image Load(const AppData&);

	void Save(const AppData&);
};

class CloudTexture : public Texture
{
public:
	const GLuint tex_unit;
	CloudTexture(const AppData&, const CloudVolume&, ResourceAllocator&);
	void Use(void);
};

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
