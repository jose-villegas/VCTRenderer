/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_StaticGroup_example_1

__VertexShader vxs;
vxs.Source(/* ... */).Compile();

__TessControlShader tcs;
tcs.Source(/* ... */).Compile();

__TessEvaluationShader tes;
tes.Source(/* ... */).Compile();

__GeometryShader gys;
gys.Source(/* ... */).Compile();

__FragmentShader fgs;
fgs.Source(/* ... */).Compile();

__Program prog;

prog.AttachShaders(__MakeGroup(vxs, tcs, tes, gys, fgs)); /*<
__MakeGroup creates a __StaticGroup<__Shader> which is implicitly
convertible to __Sequence<Shader> accepted by [^__Program::AttachShaders].
>*/
prog.Link();


//]

