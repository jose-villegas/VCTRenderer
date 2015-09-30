/**
 *  @example standalone/001_ati_meminfo.cpp
 *  @brief Shows simple usage of the ATI_meminfo extension
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <oglplus/all.hpp>
#include <oglplus/ext/ATI_meminfo.hpp>

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
		oglplus::ATI_meminfo ami;

		oglplus::ATI_meminfo::AvailableMemory mem_info[3] = {
			ami.VBOFreeMemory(),
			ami.TextureFreeMemory(),
			ami.RenderbufferFreeMemory()
		};

		const char* name[3] = {
			"VBO",
			"Texture",
			"Renderbuffer"
		};

		for(int i=0; i!=3; ++i)
		{
			std::cout << name[i] << " Free Memory:" <<
				std::endl;
			std::cout << "  Total: " <<
				mem_info[i].TotalFree() <<
				std::endl;
			std::cout << "  Largest block: " <<
				mem_info[i].LargestFreeBlock() <<
				std::endl;
			std::cout << "  Total aux: " <<
				mem_info[i].TotalAuxFree() <<
				std::endl;
			std::cout << "  Largest aux block: " <<
				mem_info[i].LargestAuxFreeBlock() <<
				std::endl;
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

