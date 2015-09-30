/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_Context_example_1

__Context gl; /*<
Stateless wrapper for the current GL context operations.
>*/

std::cout << "Vendor: " << gl.Vendor() << std::endl;
std::cout << "Version: " << gl.Version() << std::endl;
std::cout << "Major version: " << gl.MajorVersion() << std::endl;
std::cout << "Minor version: " << gl.MinorVersion() << std::endl;
std::cout << "GLSL Version: " << gl.ShadingLanguageVersion() << std::endl;
std::cout << "Renderer: " << gl.Renderer() << std::endl;
std::cout << "Extensions:" << std::endl;

for(auto r=gl.Extensions(); !r.Empty(); r.Next())
{
	std::cout << '\t' << r.Front() << std::endl;
}

std::cout << "Limits:" << std::endl;

for(auto r=__EnumValueRange<__LimitQuery>(); !r.Empty(); r.Next())
{
	auto ev = r.Front();
	std::cout << __EnumValueName(ev).c_str() << ": ";
	try { std::cout << gl.FloatLimit(ev); }
	catch(...){ std::cout << "N/A"; }
	std::cout << std::endl;
}


gl.ClearColor(0.3f, 0.3f, 0.3f, 0.0f);
gl.ClearDepth(1.0f);
gl.Disable(__Capability::DepthTest);
gl.Enable(Capability::Blend);
gl.BlendFunc(__BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha);

gl.LineWidth(1.5f);

gl.Viewport(800, 600);

gl.Clear().ColorBuffer().DepthBuffer().StencilBuffer();

__Program prog;
__VertexArray vao;

/* ... */

gl.Use(prog);
gl.Bind(vao);

gl.DrawArrays(__PrimitiveType::TriangleStrip, 0, n);

gl.DrawElements(__PrimitiveType::TriangleStrip, m, __DataType::UnsignedInt);

//]

