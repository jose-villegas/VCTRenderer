/**
 *  @file advanced/cloud_trace/saver.cpp
 *  @brief Implements the saver class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "saver.hpp"
#include "raytracer.hpp"

#include <oglplus/framebuffer.hpp>

#include <string>
#include <vector>
#include <fstream>

namespace oglplus {
namespace cloud_trace {

Saver::Saver(const AppData&)
{
}

void Saver::SaveFrame(
	const AppData& app_data,
	RaytracerTarget& rt_target,
	unsigned face
)
{
	assert(face < 6);

	gl.Flush();

	std::vector<char> pixels(app_data.render_width * app_data.render_height * 4);

	gl.Finish();

	if(app_data.save_raytrace_data)
	{
		rt_target.fbo.Bind(FramebufferTarget::Read);
	}
	else
	{
		dfb.Bind(FramebufferTarget::Read);
	}

	gl.ReadPixels(
		0, 0,
		app_data.render_width,
		app_data.render_height,
		PixelDataFormat::RGBA,
		PixelDataType::UnsignedByte,
		pixels.data()
	);
	std::string path = app_data.output_prefix;
	path.append(app_data.output_face_id[face]);
	path.append(".");
	path.append(app_data.output_suffix);

	std::ofstream output(path.c_str());
	output.write(pixels.data(), pixels.size());
}

} // namespace cloud_trace
} // namespace oglplus
