/*
 *  .file test/oglplus/fixture_glx.cpp
 *  Implements GLX-based test fixture
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "fixture.hpp"

#include <oglplus/glx/context.hpp>
#include <oglplus/glx/fb_configs.hpp>
#include <oglplus/glx/version.hpp>
#include <oglplus/glx/pixmap.hpp>
#include <oglplus/x11/window.hpp>
#include <oglplus/x11/color_map.hpp>
#include <oglplus/x11/visual_info.hpp>
#include <oglplus/x11/display.hpp>

#include <cassert>

namespace oglplus {

class GLXTestFixtureBase
{
private:
	x11::Display display;
	glx::Version version;

	static int* visual_attribs(void)
	{
		static int va[] =
		{
			GLX_X_RENDERABLE    , True,
			GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
			GLX_RENDER_TYPE     , GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
			GLX_RED_SIZE        , 8,
			GLX_GREEN_SIZE      , 8,
			GLX_BLUE_SIZE       , 8,
			GLX_ALPHA_SIZE      , 8,
			GLX_DEPTH_SIZE      , 24,
			GLX_STENCIL_SIZE    , 8,
			GLX_DOUBLEBUFFER    , True,
			None
		};
		return va;
	}

	glx::FBConfig fb_config;
	x11::VisualInfo visual_info;
	x11::Window window;
	glx::Context gl_context;
public:
	GLXTestFixtureBase(void)
	 : display()
	 , version(display)
	 , fb_config(glx::FBConfigs(display,visual_attribs()).FindBest(display))
	 , visual_info(display, fb_config)
	 , window(
		display,
		visual_info,
		x11::Colormap(display, visual_info),
		"OGLplus test fixture window",
		128, 128
	), gl_context(display, fb_config, 3, 3)
	{
		version.AssertAtLeast(1, 3);
		gl_context.MakeCurrent(window);
	}

	~GLXTestFixtureBase(void)
	{
		gl_context.Release(display);
	}
};

class GLXTestFixture
 : public GLXTestFixtureBase
{
private:
	oglplus::GLAPIInitializer gl_api_init;
public:
	GLXTestFixture(void)
	 : GLXTestFixtureBase()
	{ }
};


TestFixture::TestFixture(void)
 : _pimpl(static_cast<void*>(new GLXTestFixture()))
{ }

TestFixture::TestFixture(TestFixture&& tmp)
 : _pimpl(tmp._pimpl)
{
	tmp._pimpl = nullptr;
}

TestFixture::~TestFixture(void)
{
	if(_pimpl) delete static_cast<GLXTestFixture*>(_pimpl);
}

} // namespace oglplus

