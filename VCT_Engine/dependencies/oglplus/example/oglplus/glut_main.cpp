/*
 *  .file example/oglplus/glut_main.cpp
 *  Implements GLUT-based program main function for running examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#if OGLPLUS_FREEGLUT_FOUND
# include <GL/freeglut.h>
#elif defined(__APPLE__) && __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include <cstddef>
#include <cstring>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include <oglplus/error/basic.hpp>

#include <oglplus/query.hpp>

#include <oglplus/os/steady_clock.hpp>

#include "example.hpp"
#include "example_main.hpp"

namespace oglplus {

class SingleExample
{
private:
	static SingleExample*& SingleInstance(void)
	{
		static SingleExample* wrapper = nullptr;
		return wrapper;
	}

	os::steady_clock _os_clock;
	ExampleClock _clock;
	double _fps_time, _prim_count;
	unsigned long _frame_no;
	GLuint _width, _height;
	std::unique_ptr<Example> _example;
	std::unique_ptr<Query> _primitive_query;

	const char* _screenshot_path;

	SingleExample(const SingleExample&);
public:

	Example* operator ->(void)
	{
		assert(SingleInstance());
		return SingleInstance()->_example.get();
	}

	SingleExample(
		GLuint width,
		GLuint height,
		const ExampleParams& params,
		const char* screenshot_path
	): _fps_time(0.0)
	 , _prim_count(0.0)
	 , _frame_no(0)
	 , _width(width)
	 , _height(height)
	 , _example(makeExample(params))
	 , _primitive_query(new Query())
	 , _screenshot_path(screenshot_path)
	{
		assert(!SingleInstance());
		SingleInstance() = this;

		assert(_example);
		_example->Reshape(width, height);
		_example->MouseMove(width/2, height/2, width, height);
		_os_clock.reset();

		if(_screenshot_path)
			_clock.Update(_example->HeatUpTime());
	}

	~SingleExample(void)
	{
		assert(SingleInstance());
		SingleInstance() = nullptr;
	}

	void Quit(void)
	{
#if OGLPLUS_FREEGLUT_FOUND
		glutLeaveMainLoop();
#else
		exit(0);
#endif
	}

	void Close(void)
	{
		_primitive_query.reset();
		_example.reset();
	}

	static void CloseFunc(void)
	{
		assert(SingleInstance());
		SingleInstance()->Close();
	}

	void Display(void)
	{
		_clock.Update(_os_clock.seconds());
		double frame_time = _clock.Now().Seconds();
		_frame_no++;

		GLuint primitives_per_frame = 0;
		if(_primitive_query)
		{
			try
			{
				auto query_exec = _primitive_query->Execute(
					Query::Target::PrimitivesGenerated,
					primitives_per_frame
				);
				_example->Render(_clock);
				glutSwapBuffers();
			}
			catch(Error&)
			{
				_primitive_query.reset();
			}
		}
		else
		{
			_example->Render(_clock);
			glutSwapBuffers();
		}
		_prim_count += double(primitives_per_frame);

		const double fps_interval = 10.0;
		const double this_interval = frame_time - _fps_time;
		if(this_interval >= fps_interval)
		{
			_fps_time = frame_time;
			std::cout.width(5);
			std::cout.precision(3);
			std::cout << _frame_no << " frames in "
				<< std::fixed << this_interval << " seconds = "
				<< std::fixed << _frame_no/this_interval << " FPS; "
				<< std::scientific << _prim_count/this_interval << " PPS; "
				<< std::scientific << _prim_count/_frame_no << " PPF; "
				<< std::endl;
			_frame_no = 0;
			_prim_count = 0.0;
		}

		if(!_example->Continue(_clock))
		{
			Quit();
		}
	}

	static void DisplayFunc(void)
	{
		assert(SingleInstance());
		SingleInstance()->Display();
	}

	void Screenshot(void)
	{
		_example->Render(_clock);
		if(_clock.Now().Seconds() >= _example->ScreenshotTime())
		{
			glFinish();
			std::vector<char> pixels(_width * _height * 3);
			glReadPixels(
				0, 0,
				_width,
				_height,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				pixels.data()
			);
			std::ofstream output(_screenshot_path);
			output.write(pixels.data(), pixels.size());
			Quit();
		}
		glutSwapBuffers();
		_clock.Advance(_example->FrameTime());
	}

	static void ScreenshotFunc(void)
	{
		assert(SingleInstance());
		SingleInstance()->Screenshot();
	}

	void Reshape(int width, int height)
	{
		_width = width;
		_height= height;
		_example->Reshape(width, height);
	}

	static void ReshapeFunc(int width, int height)
	{
		assert(SingleInstance());
		SingleInstance()->Reshape(width, height);
	}

	void Motion(int x, int y)
	{
		_example->MouseMove(x, _height-y, _width, _height);
	}

	static void MotionFunc(int x, int y)
	{
		assert(SingleInstance());
		SingleInstance()->Motion(x, y);
	}

	void KeyPress(unsigned char /*k*/)
	{
		// TODO
	}

	static void KeyboardFunc(unsigned char k, int, int)
	{
		if(k == 0x1B) // Escape
		{
			assert(SingleInstance());
			SingleInstance()->Quit();
		}
		else
		{
			assert(SingleInstance());
			SingleInstance()->KeyPress(k);
		}
	}
};

} // namespace oglplus

int glut_example_main(int argc, char ** argv)
{
	GLuint width = 800, height = 600;
	glutInit(&argc, argv);
	glutInitDisplayMode(
#if defined(__APPLE__) && __APPLE__
		GLUT_3_2_CORE_PROFILE |
#endif
		GLUT_DOUBLE |
		GLUT_RGBA |
		GLUT_DEPTH |
		GLUT_STENCIL
	);
#if OGLPLUS_FREEGLUT_FOUND
	glutInitContextVersion(
		OGLPLUS_GL_VERSION_MAJOR,
		OGLPLUS_GL_VERSION_MINOR
	);
#endif
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OGLplus example");

	const char* screenshot_path = nullptr;

	for(int a=1; a!=argc;)
	{
		int aoffs = 0;
		if(std::strcmp(argv[a], "--fullscreen") == 0)
		{
			glutFullScreen();
			aoffs = 1;
		}

		if(std::strcmp(argv[a], "--screenshot") == 0)
		{
			if((a+1) < argc)
			{
				screenshot_path = argv[a+1];
				aoffs = 2;
			}
			else
			{
				screenshot_path = "screenshot.rgb";
				aoffs = 1;
			}
		}

		if(aoffs > 0)
		{
			for(int ao=a+aoffs; ao!=argc; ++ao)
			{
				argv[ao-aoffs] = argv[ao];
			}
			argc -= aoffs;
		}
		else ++a;
	}

	oglplus::GLAPIInitializer api_init;

	// The parameters for this example
	oglplus::ExampleParams params(argc, argv);
	setupExample(params);
	params.Check();

	using oglplus::SingleExample;
	// The main window/rendering context
	if(screenshot_path)
	{
		glutDisplayFunc(&SingleExample::ScreenshotFunc);
		glutIdleFunc(&SingleExample::ScreenshotFunc);
	}
	else
	{
		glutDisplayFunc(&SingleExample::DisplayFunc);
		glutIdleFunc(&SingleExample::DisplayFunc);
	}
	glutReshapeFunc(&SingleExample::ReshapeFunc);

	glutMotionFunc(&SingleExample::MotionFunc);
	glutPassiveMotionFunc(&SingleExample::MotionFunc);

	glutKeyboardFunc(&SingleExample::KeyboardFunc);
#if OGLPLUS_FREEGLUT_FOUND
	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);
	glutCloseFunc(&SingleExample::CloseFunc);
#endif

	// create the example
	SingleExample example(width, height, params, screenshot_path);

	// start the example main loop
	glutMainLoop();
	return 0;
}

int main(int argc, char* argv[])
{
	return oglplus::example_main(glut_example_main, argc, argv);
}

