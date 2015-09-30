/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_program_example_1
__Context gl;

__VertexShader vxs;
vxs.Source(
	"#version 150\n"
	"in vec3 Position;\n"
	"void main(void)\n"
	"{\n"
	"	gl_Position = Position;\n"
	"}\n"
).Compile();

__TessControlShader tcs;
tcs.Source(/* ... */).Compile();

__TessEvaluationShader tes;
tes.Source(/* ... */).Compile();

__GeometryShader gys;
gys.Source(/* ... */).Compile();

__FragmentShader fgs;
fgs.Source(/* ... */).Compile();

__Program prog;

prog
	.AttachShader(vxs)
	.AttachShader(tcs)
	.AttachShader(tes)
	.AttachShader(gys)
	.AttachShader(fgs)
	.Link();

gl.Use(prog);
//]

//[oglplus_program_sugar_example_1
__Context gl;

__VertexShader vxs;
__TessControlShader tcs;
__TessEvaluationShader tes;
__GeometryShader gys;
__FragmentShader fgs;

/* ... */

__Program prog;

prog << vxs << tcs << tes << gys << fgs; /*<
Equivalent to [^__Program::AttachShader(...)].
>*/
prog.Link();

gl.Use(prog);
//]

//[oglplus_program_sugar_example_2
__Program prog;

prog << __VertexShader(
	__ObjectDesc("Dist"),
	"#version 150\n"
	"uniform mat4 ProjectionMatrix, CameraMatrix;\n"
	"in vec4 Position;\n"
	"out uint xfbIndex;\n"
	"out float xfbDistance;\n"
	"void main(void)\n"
	"{\n"
	"	vec4 p = ProjectionMatrix*CameraMatrix*Position;\n"
	"	xfbIndex = uint(gl_VertexID);\n"
	"	xfbDistance = p.z;\n"
	"}\n"
);

prog	<< __TransformFeedbackMode::SeparateAttribs /*<
Equivalent to calling [^__Program::TransformFeedbackVaryings(...)].
>*/
	<< "xfbIndex"
	<< "xfbDistance";

prog.Link();
//]

