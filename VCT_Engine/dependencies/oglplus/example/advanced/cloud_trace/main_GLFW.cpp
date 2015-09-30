/**
 *  @example advanced/cloud_trace/main_GLFW.cpp
 *  @brief GLFW-based implementation of the main function.
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "main_common.hpp"
#include "app_data.hpp"
#include "resources.hpp"
#include "renderer.hpp"
#include "raytracer.hpp"
#include "copier.hpp"
#include "saver.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <GL/glfw.h>

#include <stdexcept>
#include <iostream>

namespace oglplus {
namespace cloud_trace {

class GLFWInitializer
{
public:
	GLFWInitializer(void)
	{
		if(!glfwInit())
		{
			throw std::runtime_error("GLFW initialization error");
		}
	}

	~GLFWInitializer(void)
	{
		glfwTerminate();
	}
};

void render_loop(AppData& app_data)
{
	ResourceAllocator alloc;

	RaytracerTarget raytrace_tgt(app_data, alloc);

	RaytracerData raytrace_data(app_data);
	RaytracerResources raytrace_res(app_data, raytrace_data, alloc);

	Raytracer raytracer(app_data, raytrace_res);
	RaytraceCopier::Params copy_params;
	RaytraceCopier copier(app_data, raytrace_tgt);

	Renderer renderer(app_data, raytrace_tgt.tex_unit);
	Saver saver(app_data);

	unsigned face = 0;
	unsigned tile = 0;
	const unsigned tiles = app_data.tiles();


	while(true)
	{
		if(app_data.skip_face[face])
		{
			++face;
			continue;
		}

		if(app_data.verbosity > 0)
		{
			app_data.logstr()
				<< "Rendering cube face "
				<< face
				<< std::endl;
		}

		raytracer.Use(app_data);

		if(tile == 0)
		{
			raytrace_tgt.Clear(app_data);
			raytracer.InitFrame(app_data, face);
			renderer.InitFrame(app_data, face);
		}

		if(tile < tiles)
		{
			raytracer.BeginWork(app_data);
			raytracer.Raytrace(app_data, tile);
			raytracer.EndWork(app_data);

			copier.Copy(app_data, copy_params, raytracer, tile);

			renderer.Use(app_data);
			renderer.Render(app_data);

			glfwSwapBuffers();

			tile++;
		}
		else if(face < 6)
		{
			glfwSwapBuffers();
			saver.SaveFrame(app_data, raytrace_tgt, face);
			if(face < 5)
			{
				tile = 0;
				face++;
			}
			else break;
		}
		glfwPollEvents();

		int new_x, new_y;
		glfwGetWindowSize(&new_x, &new_y);
		if(new_x > 0)
		{
			app_data.render_width = unsigned(new_x);
		}
		if(new_y > 0)
		{
			app_data.render_height = unsigned(new_y);
		}

		if(glfwGetKey(GLFW_KEY_ESC))
		{
			glfwCloseWindow();
			break;
		}
		if(!glfwGetWindowParam(GLFW_OPENED))
		{
			break;
		}
	}
}

int main_GLFW(AppData& app_data)
{
	GLFWInitializer glfw_initializer;

	if(!glfwOpenWindow(
		app_data.render_width,
		app_data.render_height,
		8, 8, 8, 8,
		32, 8,
		GLFW_WINDOW
	)) throw std::runtime_error("Error creating GLFW window");
	else
	{
		glfwSetWindowTitle("CloudTrace: OGLplus cloud ray-tracer");
		glfwPollEvents();

		GLAPIInitializer api_init;

		render_loop(app_data);
	}
	return 0;
}

} // namespace cloud_trace
} // namespace oglplus

int main (int argc, char ** argv)
{
	using oglplus::cloud_trace::main_GLFW;
	using oglplus::cloud_trace::AppData;

	AppData app_data;
	if(app_data.ParseArgs(argc, argv))
	{
		app_data.LogInfo();
		return do_run_main(main_GLFW, app_data);
	}
	else return 0;
}

