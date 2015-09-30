/**
 *  @example standalone/001_program_resources.cpp
 *  @brief Shows program resource/interface functionality usage
 *
 *  Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define OGLPLUS_NO_ENUM_VALUE_RANGES 0
#define OGLPLUS_NO_ENUM_VALUE_NAMES  0

#include <cassert>
#include <cstring>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <oglplus/all.hpp>

int main(int argc, char* argv[])
{
#if GL_VERSION_4_3
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("OGLplus+GLUT+GLEW");

	if(glewInit() == GLEW_OK) try
	{
		glGetError();
		using namespace oglplus;
		Program prog;
		ShaderType shader_type;

		int arg = 1;

		// if there are no arguments
		if(arg >= argc)
		{
			// attach a default vertex shader
			prog.AttachShader(VertexShader(
				"#version 330\n"
				"in vec3 Position;"
				"void main(void)"
				"{"
				"	gl_Position = vec4(Position, 1.0);"
				"}"
			));

			// and a default fragment shader
			prog.AttachShader(FragmentShader(
				"#version 330\n"
				"uniform vec4 Color1, Color2;"
				"uniform float Mult;"
				"out vec4 fragColor;"
				"void main(void)"
				"{"
				"	fragColor = Mult*Color1 + Color2;"
				"}"
			));
		}
		else while(arg != argc)
		{
			if(std::strcmp(argv[arg], "--compute") == 0)
				shader_type = ShaderType::Compute;
			else if(std::strcmp(argv[arg], "--vertex") == 0)
				shader_type = ShaderType::Vertex;
			else if(std::strcmp(argv[arg], "--tess-control") == 0)
				shader_type = ShaderType::TessControl;
			else if(std::strcmp(argv[arg], "--tess-evaluation") == 0)
				shader_type = ShaderType::TessEvaluation;
			else if(std::strcmp(argv[arg], "--geometry") == 0)
				shader_type = ShaderType::Geometry;
			else if(std::strcmp(argv[arg], "--fragment") == 0)
				shader_type = ShaderType::Fragment;
			else
			{
				std::cerr
					<< "Invalid option '"
					<< argv[arg]
					<< "'."
					<< std::endl;
				return 1;
			}

			if(++arg != argc)
			{
				Shader shader(shader_type, ObjectDesc(argv[arg]));
				shader.Source(GLSLSource::FromFile(argv[arg]));
				shader.Compile();
				prog.AttachShader(shader);
			}
			else
			{
				std::cerr
					<< "The '"
					<< argv[arg-1]
					<< "' option must be followed by a valid file path."
					<< std::endl;
				return 1;
			}
			++arg;
		}
		prog.Link();

		auto interfaces = EnumValueRange<ProgramInterface>();
		std::cout << "-+-[Program Resources]" << std::endl;
		std::cout << " |" << std::endl;

		if(!interfaces.Empty())	while(true)
		{
			ProgramInterface intf = interfaces.Front();
			interfaces.Next();
			bool last_intf = interfaces.Empty();

			auto resources = prog.ActiveResources(intf);
			bool empty = resources.Empty();

			if(last_intf) std::cout << " `-";
			else std::cout << " +-";
			if(empty) std::cout << "--";
			else std::cout << "+-";
			std::cout << "[" << EnumValueName(intf).c_str() << "]" << std::endl;

			if(!empty) std::cout << " | |" << std::endl;
			else if(!last_intf) std::cout << " |" << std::endl;

			if(!empty) while(true)
			{
				auto res = resources.Front();
				resources.Next();
				bool last_res = resources.Empty();

				if(last_res) std::cout << " | `-";
				else std::cout << " | +-";

				bool has_type = res.HasType();
				bool has_name = !res.Name().empty();

				if(has_name || has_type)
					std::cout << "+-";
				else std::cout << "--";
				std::cout << "[Location " << res.Index() << "]" << std::endl;

				if(has_type)
				{
					if(last_res) std::cout << " |  ";
					else std::cout << " | |";
					std::cout << " |" << std::endl;

					if(last_res) std::cout << " |  ";
					else std::cout << " | |";
					if(has_name) std::cout << " +-";
					else std::cout << " `-";
					std::cout << "--[Type: ";
					std::cout << EnumValueName(res.Type()).c_str();
					std::cout << "]" << std::endl;
				}

				if(has_name)
				{
					if(last_res) std::cout << " |  ";
					else std::cout << " | |";
					std::cout << " |" << std::endl;

					if(last_res) std::cout << " |  ";
					else std::cout << " | |";
					std::cout << " `---[Name: ";
					std::cout << res.Name();
					std::cout << "]" << std::endl;
				}

				if(last_res) std::cout << " |" << std::endl;
				else std::cout << " | |" << std::endl;

				if(last_res) break;
			}

			if(last_intf) break;
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
	catch(const std::exception& se)
	{
		std::cerr <<
			"General error: " <<
			se.what() << std::endl;
	}
#else
	OGLPLUS_FAKE_USE(argc);
	OGLPLUS_FAKE_USE(argv);
	std::cout << "OpenGL 4.3 is required." << std::endl;
#endif
	return 1;
}

