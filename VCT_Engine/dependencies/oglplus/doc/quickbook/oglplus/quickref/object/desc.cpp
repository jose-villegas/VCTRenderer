/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_desc_example_1
try
{
	__VertexShader vxs(__ObjectDesc("Vertex shader 1"));
	vxs.Source(/* ... */).Compile(); /*<
	Compilation of any of these five shaders may fail.
	>*/

	__TessControlShader tcs(__ObjectDesc("Tessellation control shader 1"));
	tcs.Source(/* ... */).Compile();

	__TessEvaluationShader tes(__ObjectDesc("Tessellation evaluation shader 1"));
	tes.Source(/* ... */).Compile();

	__GeometryShader gys(__ObjectDesc("Geometry shader 1"));
	gys.Source(/* ... */).Compile();

	__FragmentShader fgs(__ObjectDesc("Fragment shader 1"));
	fgs.Source(/* ... */).Compile();

	__Program prog(__ObjectDesc("Program 1"));

	prog << vxs << tcs << tes << gys << fgs; /*<
	Attach the shaders.
	>*/

	prog.Link(); /*<
	Linking may fail too.
	>*/

	return prog;
}
catch(__ProgramBuildError& error) /*<
This catches both compilation and linking errors.
>*/
{
	std::cout << error.ObjectDesc() << std::cout; /*<
	Print the description of the object which caused the error.
	>*/
}
//]

