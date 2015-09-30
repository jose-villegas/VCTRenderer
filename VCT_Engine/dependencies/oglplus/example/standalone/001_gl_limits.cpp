/**
 *  @example standalone/001_gl_limits.cpp
 *  @brief Shows the basic usage of OGLplus by printing some info
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#include <cassert>
#include <iostream>
#include <iomanip>

#include <GL/glew.h>
#include <GL/glut.h>

#include <oglplus/context.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OGLplus+GLUT+GLEW");

	if(glewInit() == GLEW_OK) try
	{
		glGetError();
		oglplus::Context context;
		//
		std::cout << "Limits:" << std::endl;
		//
		std::size_t w = 0;
		for(auto r=oglplus::EnumValueRange<oglplus::LimitQuery>(); !r.Empty(); r.Next())
		{
			std::size_t n = EnumValueName(r.Front()).size();
			if(w < n) w = n;
		}
		for(auto r=oglplus::EnumValueRange<oglplus::LimitQuery>(); !r.Empty(); r.Next())
		{
			auto ev = r.Front();
			std::cout << std::setw(int(w)) << EnumValueName(ev).c_str() << ": ";
			try { std::cout << context.FloatLimit(ev); }
			catch(...){ std::cout << "N/A"; }
			std::cout << std::endl;
		}
		return 0;
	}
	catch(oglplus::Error& err)
	{
		std::cerr
			<< "Error (in "
			<< err.GLFunc()
			<< "'): "
			<< err.what()
			<< " ["
			<< err.SourceFile()
			<< ":"
			<< err.SourceLine()
			<< "] "
			<< std::endl;
	}
	return 1;
}
