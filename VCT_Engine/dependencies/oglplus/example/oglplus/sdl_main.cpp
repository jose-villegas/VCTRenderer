/*
 *  .file example/oglplus/sdl_main.cpp
 *  Implements SDL-based program main function for running examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <SDL/SDL.h>

#include <oglplus/os/steady_clock.hpp>

#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <cassert>

#include "example.hpp"
#include "example_main.hpp"

namespace oglplus {

class SDLInitializer
{
public:
	SDLInitializer(const char* title, GLuint width, GLuint height)
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
			throw std::runtime_error(SDL_GetError());

		if(!SDL_SetVideoMode(
			width, height, 32,
			SDL_HWSURFACE |
			SDL_GL_DOUBLEBUFFER |
			SDL_OPENGL
		))
		{
			std::runtime_error e(SDL_GetError());
			SDL_Quit();
			throw e;
		}
		SDL_WM_SetCaption(title, nullptr);
	}

	~SDLInitializer(void)
	{
		SDL_Quit();
	}
};

void make_screenshot(
	std::unique_ptr<Example>& example,
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
		if(s < t) SDL_GL_SwapBuffers();
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
	SDL_GL_SwapBuffers();
}

void run_loop(
	std::unique_ptr<Example>& example,
	GLuint width,
	GLuint height
)
{
	os::steady_clock os_clock;
	ExampleClock clock;
	bool done = false;

	while(!done)
	{
		clock.Update(os_clock.seconds());
		if(!example->Continue(clock)) break;
		example->Render(clock);

		SDL_GL_SwapBuffers();

		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_MOUSEMOTION)
			{
				example->MouseMove(
					event.motion.x,
					height-
					event.motion.y,
					width,
					height
				);
			}
			else if(event.type == SDL_KEYUP)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE)
					done = true;
			}
			else if(event.type == SDL_VIDEORESIZE)
			{
				width = event.resize.w;
				height = event.resize.h;
				example->Reshape(width, height);
			}
			else if(event.type == SDL_QUIT)
			{
				done = true;
			}
		}
	}
}

void run_example(
	const char* screenshot_path,
	int argc,
	char ** argv
)
{
	const GLuint width = 800, height = 600;

	SDLInitializer sdl_initializer("OGLplus example", width, height);
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
			width,
			height,
			screenshot_path
		);
	}
	else run_loop(example, width, height);
}

} // namespace oglplus

int sdl_example_main (int argc, char ** argv)
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
	return oglplus::example_main(sdl_example_main, argc, argv);
}

