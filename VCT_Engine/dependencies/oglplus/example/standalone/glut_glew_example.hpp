/*
 *  @file standalone/glut_glew_example.hpp
 *  @brief Implements GLUT/GLEW-based program main function for running examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_STANDALONE_GLUT_GLEW_EXAMPLE_1203161253_HPP
#define OGLPLUS_STANDALONE_GLUT_GLEW_EXAMPLE_1203161253_HPP

#include <GL/glew.h>

#include <oglplus/config/gl.hpp>

#if OGLPLUS_USE_FREEGLUT
# include <GL/freeglut.h>
#else
# include <GL/glut.h>
#endif

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <chrono>

#include <oglplus/error/program.hpp>
#include <oglplus/error/limit.hpp>

namespace oglplus {

class StandaloneExample
{
private:
	std::chrono::time_point<std::chrono::system_clock> _start;
	std::chrono::time_point<std::chrono::system_clock> _now;
	double _frame_time, _frame_duration;
	unsigned long _frame_number;

	GLuint _width, _height;

	GLuint _prev_mouse_x, _prev_mouse_y;
	GLuint _curr_mouse_x, _curr_mouse_y;

	GLuint _curr_key;
public:
	void HandleUpdate(void)
	{
		static const double period =
			double(std::chrono::system_clock::period::num)/
			double(std::chrono::system_clock::period::den);
		auto now = std::chrono::system_clock::now();
		_frame_duration = (now - _now).count() * period;
		_now = now;
		_frame_time = (_now - _start).count() * period;
		++_frame_number;
	}

	void HandleResize(GLuint width, GLuint height)
	{
		_width = width;
		_height = height;
	}

	void HandleMouseMove(GLuint mouse_x, GLuint mouse_y)
	{
		_prev_mouse_x = _curr_mouse_x;
		_prev_mouse_y = _curr_mouse_y;;
		_curr_mouse_x = mouse_x;
		_curr_mouse_y = mouse_y;
	}

	void HandleKeyPress(GLuint key)
	{
		_curr_key = key;
	}


	double FrameTime(void) const { return _frame_time; }
	double FrameDuration(void) const { return _frame_duration; }
	unsigned long FrameNumber(void) const { return _frame_number; }

	double CurrentFPS(void) const { return 1.0 / _frame_duration; }
	double AverageFPS(void) const { return _frame_number / _frame_time; }

	GLuint Width(void) const { return _width; }
	GLuint Height(void) const { return _height; }
	double Aspect(void) const { return double(_width)/double(_height); }

	GLuint MouseX(void) const { return _curr_mouse_x; }
	GLuint MouseY(void) const { return _curr_mouse_y; }

	int MouseDiffX(void) const
	{
		return int(_curr_mouse_x - _prev_mouse_x);
	}

	int MouseDiffY(void) const
	{
		return int(_curr_mouse_y - _prev_mouse_y);
	}

	double NormMouseX(void) const
	{
		return double(2*_curr_mouse_x)/_width - 1.0;
	}

	double NormMouseY(void) const
	{
		return double(2*_curr_mouse_y)/_height - 1.0;
	}

	double NormMouseDiffX(void) const
	{
		return double(2*MouseDiffX())/_width;
	}

	double NormMouseDiffY(void) const
	{
		return double(2*MouseDiffY())/_height;
	}

	GLuint Key(void) const { return _curr_key; }

private:
	StandaloneExample(const StandaloneExample&);
public:

	StandaloneExample(void)
	 : _width(800)
	 , _height(800)
	{ }

	virtual ~StandaloneExample(void)
	{ }

	void Startup(GLuint width, GLuint height)
	{
		HandleResize(width, height);
		Reshape();

		HandleMouseMove(_width/2, _height/2);
		HandleMouseMove(_width/2, _height/2);
		PassiveMotion();

		_start = std::chrono::system_clock::now();
		_frame_number = 0;
		HandleUpdate();
	}

	virtual void Render(void) = 0;

	virtual void Reshape(void) = 0;

	virtual void Motion(void)
	{
	}

	virtual void PassiveMotion(void)
	{
	}

	virtual void KeyPress(void)
	{
	}
};

template <typename Example>
class SingleExampleTpl
{
private:
	static Example*& SingleInstance(void)
	{
		static Example* _ptr = nullptr;
		return _ptr;
	}
public:
	SingleExampleTpl(int argc, const char** argv)
	{
		assert(!SingleInstance());
		SingleInstance() = new Example(argc, argv);
	}

	~SingleExampleTpl(void)
	{
		delete SingleInstance();
	}

	void Startup(GLuint width, GLuint height)
	{
		assert(SingleInstance());
		SingleInstance()->Startup(width, height);
	}

	static void CloseFunc(void)
	{
		assert(SingleInstance());
		delete SingleInstance();
		SingleInstance() = nullptr;
	}

	static void DisplayFunc(void)
	{
		assert(SingleInstance());
		SingleInstance()->HandleUpdate();
		SingleInstance()->Render();
		glutSwapBuffers();
	}

	static void ReshapeFunc(int width, int height)
	{
		assert(SingleInstance());
		SingleInstance()->HandleResize(width, height);
		SingleInstance()->Reshape();
	}

	static void MotionFunc(int x, int y)
	{
		assert(SingleInstance());
		SingleInstance()->HandleMouseMove(x, y);
		SingleInstance()->Motion();
	}

	static void PassiveMotionFunc(int x, int y)
	{
		assert(SingleInstance());
		SingleInstance()->HandleMouseMove(x, y);
		SingleInstance()->PassiveMotion();
	}

	static void KeyboardFunc(unsigned char k, int, int)
	{
		if(k == 0x1B) // Escape
		{
#if OGLPLUS_USE_FREEGLUT
			glutLeaveMainLoop();
#else
			exit(0);
#endif
		}
		else
		{
			assert(SingleInstance());
			SingleInstance()->HandleKeyPress(k);
			SingleInstance()->KeyPress();
		}
	}
};

class GlutInit
{
protected:
	GlutInit(
		GLint xpos,
		GLint ypos,
		GLuint width,
		GLuint height,
		const char* window_title,
		int &argc,
		char* argv[]
	)
	{
		glutInit(&argc, argv);

		glutInitDisplayMode(
			GLUT_DOUBLE |
			GLUT_RGBA |
			GLUT_DEPTH |
			GLUT_STENCIL
		);

		glutInitWindowPosition(xpos, ypos);
		glutInitWindowSize(GLint(width), GLint(height));

		glutCreateWindow(window_title);
	}
};

class GlewInit
{
protected:
	GlewInit(void)
	{
		if(glewInit() != GLEW_OK)
		{
			throw std::runtime_error(
				"GLEW initialization error"
			);
		}
		glGetError();
	}
};

template <typename Example>
class GlutGlewExampleApp
 : public GlutInit
 , public GlewInit
{
private:
	typedef SingleExampleTpl<Example> SingleExample;
	SingleExample example;

	GlutGlewExampleApp(const GlutGlewExampleApp&);
public:
	GlutGlewExampleApp(
		GLint xpos,
		GLint ypos,
		GLuint width,
		GLuint height,
		const char* window_title,
		int &argc,
		char* argv[]
	): GlutInit(
		xpos,
		ypos,
		width,
		height,
		window_title,
		argc,
		argv
	), GlewInit()
	 , example(argc, const_cast<const char**>(argv))
	{
		glutReshapeFunc(&SingleExample::ReshapeFunc);

		glutMotionFunc(&SingleExample::MotionFunc);
		glutPassiveMotionFunc(&SingleExample::PassiveMotionFunc);

		glutKeyboardFunc(&SingleExample::KeyboardFunc);

		glutDisplayFunc(&SingleExample::DisplayFunc);
		glutIdleFunc(&SingleExample::DisplayFunc);

		example.Startup(width, height);
	}

	void Run(int /*argc*/, const char** /*argv*/)
	{
#if OGLPLUS_USE_FREEGLUT
		glutSetOption(
			GLUT_ACTION_ON_WINDOW_CLOSE,
			GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);
		glutCloseFunc(&SingleExample::CloseFunc);
#endif
		glutMainLoop();
	}
};

template <typename Example>
int GlutGlewMain(const char* title, int argc, char* argv[])
{
	try
	{
		std::cout << "Started" << std::endl;
		{
			GLuint width = 800, height = 600;
			GlutGlewExampleApp<Example> app(
				100, 100,
				width, height,
				title,
				argc, argv
			);
			app.Run(argc, const_cast<const char**>(argv));
		}
		std::cout << "Finished" << std::endl;
		return 0;
	}
	catch(oglplus::ProgramBuildError& pbe)
	{
		std::cerr
			<< "Program build error (in "
			<< pbe.GLFunc()
			<< ", "
			<< pbe.ObjectTypeName()
			<< ": ("
			<< pbe.ObjectName()
			<< ") '"
			<< pbe.ObjectDesc()
			<< "'): "
			<< pbe.what()
			<< ": "
			<< pbe.Log()
			<< std::endl;
	}
	catch(oglplus::LimitError& le)
	{
		std::cerr
			<< "Limit error: ("
			<< le.Value()
			<< ") exceeds ("
			<< le.EnumParamName()
			<< " == "
			<< le.Limit()
			<< ") ["
			<< le.SourceFile()
			<< ":"
			<< le.SourceLine()
			<< "] "
			<< std::endl;
	}
	catch(oglplus::ObjectError& obe)
	{
		std::cerr
			<< "Object error (in "
			<< obe.GLFunc()
			<< ", "
			<< obe.ObjectTypeName()
			<< ": ("
			<< obe.ObjectName()
			<< ") '"
			<< obe.ObjectDesc()
			<< "') ["
			<< obe.SourceFile()
			<< ":"
			<< obe.SourceLine()
			<< "]: "
			<< obe.what()
			<< std::endl;
	}
	catch(oglplus::Error& err)
	{
		std::cerr
			<< "Error (in "
			<< err.GLFunc()
			<< "') ["
			<< err.SourceFile()
			<< ":"
			<< err.SourceLine()
			<< "]: "
			<< err.what()
			<< std::endl;
	}
	catch(std::exception& se)
	{
		std::cerr << "Error: " << se.what() << std::endl;
	}
	return 1;
}

} // namespace oglplus

#endif
