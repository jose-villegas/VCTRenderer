/**
 *  @example standalone/001_gl_info.cpp
 *  @brief Shows the basic usage of OGLplus by printing some info
 *
 *  Copyright 2008-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <oglplus/context.hpp>
#include <oglplus/string/ref_ios.hpp>

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

		std::cout << "Vendor: " << context.Vendor() << std::endl;
		std::cout << "Version: " << context.Version() << std::endl;
		std::cout << "Major version: " << context.MajorVersion() << std::endl;
		std::cout << "Minor version: " << context.MinorVersion() << std::endl;
		std::cout << "GLSL Version: " << context.ShadingLanguageVersion() << std::endl;
		std::cout << "Renderer: " << context.Renderer() << std::endl;
		std::cout << "Extensions:" << std::endl;

		for(auto r=context.Extensions(); !r.Empty(); r.Next())
			std::cout << '\t' << r.Front() << std::endl;

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
