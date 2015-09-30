/**
 *  Copyright 2013-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <jni.h>

#include <GLES3/gl3.h>

#include <sstream>

#define OGLPLUS_NO_SITE_CONFIG 1
#include <oglplus/all.hpp>

class TriangleExample
{
private:
	oglplus::Context gl;

	oglplus::Program prog;
	static oglplus::Program make_prog(void)
	{
		oglplus::Program prog;

		oglplus::VertexShader vs;
		vs.Source(
			"#version 300 es\n"
			"in vec4 Position;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"}"
		).Compile();

		oglplus::FragmentShader fs;
		fs.Source(
			"#version 300 es\n"
			"precision highp float;"
			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec3(0.0, 1.0, 0.0);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs);
		prog.Link().Use();

		return std::move(prog);
	}

	oglplus::VertexArray triangle;

	oglplus::Buffer positions;
public:
	TriangleExample(void)
	 : gl()
	 , prog(make_prog())
	{
		triangle.Bind();
		GLfloat triangle_pos[9] = {
			-1.0f,-1.0f, 0.0f,
			 1.0f,-1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f
		};

		positions.Bind(oglplus::Buffer::Target::Array);

		oglplus::Buffer::Data(
			oglplus::Buffer::Target::Array,
			triangle_pos
		);

		oglplus::VertexArrayAttrib(prog, "Position")
			.Setup<GLfloat>(3)
			.Enable();

		gl.ClearColor(1.0, 0.0, 1.0, 0.0);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(0, 0, width, height);
	}

	void Render(void)
	{
		gl.Clear().ColorBuffer();

		gl.DrawArrays(
			oglplus::PrimitiveType::Triangles,
			0, 3
		);
	}
};
TriangleExample* example = nullptr;


// ------ JNI ------

void ExampleFormatErrorMessage(
	oglplus::Error& error,
	std::ostream& output
)
{
	output	<< "'" << error.what()
		<< "' in '"
		<< error.GLFunc()
		<< "' at ["
		<< error.SourceFile()
		<< ":"
		<< error.SourceLine()
		<< "]"
		<< std::endl;
}

void ExampleThrowError(
	JNIEnv* env,
	const char* message
)
{
	if(env)
	{
		if(jclass ecls = env->FindClass("java/lang/RuntimeException"))
		{
			env->ThrowNew(ecls, message);
		}
	}
}

extern "C" {

JNIEXPORT void JNICALL
Java_org_oglplus_triangle_ExampleLib_init(
	JNIEnv* env,
	jclass /*cls*/
)
{
	if(example) delete example;
	try
	{
		example = new TriangleExample();
	}
	catch(oglplus::ProgramBuildError& pbe)
	{
		std::stringstream msg;
		ExampleFormatErrorMessage(pbe, msg);
		msg << "Build log: " << std::endl;
		msg << pbe.Log() << std::endl;
		ExampleThrowError(env, msg.str().c_str());
	}
	catch(oglplus::Error& oe)
	{
		std::stringstream msg;
		ExampleFormatErrorMessage(oe, msg);
		ExampleThrowError(env, msg.str().c_str());
	}
	catch(std::exception& se)
	{
		ExampleThrowError(env, se.what());
	}
}

JNIEXPORT void JNICALL
Java_org_oglplus_triangle_ExampleLib_resize(
	JNIEnv* /*env*/,
	jclass /*cls*/,
	jint width,
	jint height
)
{
	if(example) example->Reshape(width, height);
}

JNIEXPORT void JNICALL
Java_org_oglplus_triangle_ExampleLib_step(
	JNIEnv* /*env*/,
	jclass /*cls*/
)
{
	if(example) example->Render();
}

} // extern "C"
