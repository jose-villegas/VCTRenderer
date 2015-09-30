/**
 *  @file oglplus/glx/fb_configs.hpp
 *  @brief Wrapper for GLX framebuffer config-related functionality
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_GLX_FB_CONFIGS_1107121519_HPP
#define UTILS_OGLPLUS_GLX_FB_CONFIGS_1107121519_HPP

#include <oglplus/glx/fb_config.hpp>
#include <oglplus/x11/display.hpp>
#include <oglplus/x11/visual_info.hpp>

#include <GL/glx.h>
#include <stdexcept>
#include <cassert>

namespace oglplus {
namespace glx {

class FBConfigs
{
private:
	int _count;
	GLXFBConfig* _handle;
public:
	FBConfigs(const x11::Display& display, const int* visual_attribs)
	 : _count(0)
	 , _handle(
		::glXChooseFBConfig(
			display,
			DefaultScreen(display.Get()),
			visual_attribs,
			&_count
		)
	)
	{
		if(!_handle || (_count <= 0))
		{
			throw std::runtime_error(
				"Failed to get matching framebuffer configs"
			);
		}
	}

	FBConfigs(const FBConfigs&) = delete;

	FBConfigs(FBConfigs&& temp)
	 : _count(temp._count)
	 , _handle(temp._handle)
	{
		temp._count = 0;
		temp._handle = 0;
	}

	~FBConfigs(void)
	{
		if(_handle) ::XFree(_handle);
	}

	FBConfig FindBest(const x11::Display& display) const
	{
		int best = -1, best_num = -1;
		assert(_count > 0);
		for(int i=0;i!=_count;++i)
		{
			int sample_buf, samples;

			::glXGetFBConfigAttrib(
				display,
				_handle[i],
				GLX_SAMPLE_BUFFERS,
				&sample_buf
			);
			::glXGetFBConfigAttrib(
				display,
				_handle[i],
				GLX_SAMPLES,
				&samples
			);
			if((best < 0) || (sample_buf && (samples > best_num)))
			{
				best = i;
				best_num = samples;
			}
		}
		assert(best >= 0);
		assert(best < _count);
		return FBConfig(_handle[best]);
	}
};

} // namespace glx
} // namespace oglplus

#endif // include guard
