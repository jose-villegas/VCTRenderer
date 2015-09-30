/*
 *  .file example/oglplus/glfw3_main.cpp
 *  Implements GLFW-3-based program main function for running examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <GLFW/glfw3.h>

#include <oglplus/os/steady_clock.hpp>

#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>

#include "example.hpp"
#include "example_main.hpp"

namespace oglplus {

class GLFWInitializer
{
public:
	GLFWInitializer(void)
	{
		if(!glfwInit())
		{
			throw std::runtime_error("GLFW initialization error");
		}

		// TODO: this caused problems for some users
		// have a look what GLFW actually does here
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OGLPLUS_GL_VERSION_MAJOR);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OGLPLUS_GL_VERSION_MINOR);
	}

	~GLFWInitializer(void)
	{
		glfwTerminate();
	}
};

void make_screenshot(
	std::unique_ptr<Example>& example,
	GLFWwindow* window,
	GLuint width,
	GLuint height,
	const char* screenshot_path
)
{
	double s = example->HeatUpTime();
	double t = example->ScreenshotTime();
	double dt = example->FrameTime();

	ExampleClock clock(s);

	while(true)
	{
		s += dt;
		clock.Update(s);
		example->Render(clock);
		if(s < t) glfwSwapBuffers(window);
		else break;
	}
	glFinish();

	std::vector<char> pixels(width * height * 3);
	glReadPixels(
		0, 0,
		width,
		height,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		pixels.data()
	);
	std::ofstream output(screenshot_path);
	output.write(pixels.data(), pixels.size());
	glfwSwapBuffers(window);
}

void run_loop(
	std::unique_ptr<Example>& example,
	GLFWwindow* window,
	GLuint width,
	GLuint height
)
{
	GLuint mouse_x = width / 2;
	GLuint mouse_y = height / 2;
	os::steady_clock os_clock;
	ExampleClock clock;
	while(true)
	{
		clock.Update(os_clock.seconds());
		if(!example->Continue(clock)) break;
		example->Render(clock);

		glfwSwapBuffers(window);
		glfwPollEvents();

		int new_x, new_y;
		double tmp_x, tmp_y;
		glfwGetWindowSize(window, &new_x, &new_y);
		if((int(width) != new_x) || (int(height) != new_y))
		{
			if(new_x <= 0) new_x = 1;
			if(new_y <= 0) new_y = 1;
			width = GLuint(new_x);
			height = GLuint(new_y);
			example->Reshape(width, height);
		}

		glfwGetCursorPos(window, &tmp_x, &tmp_y);
		new_x = std::floor(tmp_x);
		new_y = std::floor(tmp_y);
		if((int(mouse_x) != new_x) || (int(mouse_y) != new_y))
		{
			if(new_x <= 0) new_x = 1;
			if(new_y <= 0) new_y = 1;
			mouse_x = GLuint(new_x);
			mouse_y = GLuint(new_y);
			example->MouseMove(
				mouse_x,
				height-
				mouse_y,
				width,
				height
			);
		}

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
			break;
		}
		if(glfwWindowShouldClose(window))
		{
			break;
		}
	}
}

void run_example(
	const char* screenshot_path,
	int argc,
	char** argv
)
{
	GLFWInitializer glfw_initializer;

	const GLuint width = 800, height = 600;

	GLFWwindow* window = glfwCreateWindow(
		width,
		height,
		"OGLplus example",
		NULL,
		NULL
	);
	if(!window)
	{
		throw std::runtime_error("Error creating GLFW3 window");
	}
	else
	{
		glfwMakeContextCurrent(window);
		GLAPIInitializer api_init;

		ExampleParams params(argc, argv);
		setupExample(params);
		params.Check();

		std::unique_ptr<Example> example(makeExample(params));

		example->Reshape(width, height);
		example->MouseMove(width/2, height/2, width, height);

		if(screenshot_path)
		{
			make_screenshot(
				example,
				window,
				width,
				height,
				screenshot_path
			);
		}
		else run_loop(example, window, width, height);
	}
}

} // namespace oglplus

int glfw_example_main (int argc, char ** argv)
{
	const char* screenshot_path = 0;
	if((argc >= 3) && (std::strcmp(argv[1], "--screenshot") == 0))
	{
		screenshot_path = argv[2];
		for(int a=3; a<argc; ++a)
			argv[a-2] = argv[a];
		argc -= 2;
	}

	oglplus::run_example(screenshot_path, argc, argv);
	std::cout << "Done" << std::endl;
	return 0;
}

int main (int argc, char ** argv)
{
	return oglplus::example_main(glfw_example_main, argc, argv);
}

