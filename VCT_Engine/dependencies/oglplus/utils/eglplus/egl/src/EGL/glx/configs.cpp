/**
 *  .file EGL/glx/configs.cpp
 *  .brief EGL Config implementation
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <EGL/egl.h>

#include "display.hpp"
#include "error.hpp"

#include <cassert>
#include <vector>

#include <X11/Xlib.h>
#include <GL/glx.h>

//------------------------------------------------------------------------------
// eglplus_egl_glx_ConfigImpl
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------
bool eglplus_egl_glx_TranslateAttribName(
	EGLint egl_attrib_name,
	int& glx_attrib_name
)
{
	switch(egl_attrib_name)
	{
		case EGL_CONFIG_ID:
			glx_attrib_name = GLX_FBCONFIG_ID;
			break;
		case EGL_LEVEL:
			glx_attrib_name = GLX_LEVEL;
			break;
		case EGL_CONFIG_CAVEAT:
			glx_attrib_name = GLX_CONFIG_CAVEAT;
			break;
		case EGL_MAX_PBUFFER_WIDTH:
			glx_attrib_name = GLX_MAX_PBUFFER_WIDTH;
			break;
		case EGL_MAX_PBUFFER_HEIGHT:
			glx_attrib_name = GLX_MAX_PBUFFER_HEIGHT;
			break;
		case EGL_MAX_PBUFFER_PIXELS:
			glx_attrib_name = GLX_MAX_PBUFFER_PIXELS;
			break;

		case EGL_NATIVE_VISUAL_TYPE:
			glx_attrib_name = GLX_X_VISUAL_TYPE;
			break;
		case EGL_NATIVE_RENDERABLE:
			glx_attrib_name = GLX_X_RENDERABLE;
			break;

		case EGL_SURFACE_TYPE:
			glx_attrib_name = GLX_DRAWABLE_TYPE;
			break;

		case EGL_BUFFER_SIZE:
			glx_attrib_name = GLX_BUFFER_SIZE;
			break;
		case EGL_RED_SIZE:
			glx_attrib_name = GLX_RED_SIZE;
			break;
		case EGL_GREEN_SIZE:
			glx_attrib_name = GLX_GREEN_SIZE;
			break;
		case EGL_BLUE_SIZE:
			glx_attrib_name = GLX_BLUE_SIZE;
			break;
		case EGL_ALPHA_SIZE:
			glx_attrib_name = GLX_ALPHA_SIZE;
			break;
		case EGL_DEPTH_SIZE:
			glx_attrib_name = GLX_DEPTH_SIZE;
			break;
		case EGL_STENCIL_SIZE:
			glx_attrib_name = GLX_STENCIL_SIZE;
			break;

		case EGL_SAMPLES:
			glx_attrib_name = GLX_SAMPLES;
			break;
		case EGL_SAMPLE_BUFFERS:
			glx_attrib_name = GLX_SAMPLE_BUFFERS;
			break;

		case EGL_TRANSPARENT_TYPE:
			glx_attrib_name = GLX_TRANSPARENT_TYPE;
			break;
		case EGL_TRANSPARENT_RED_VALUE:
			glx_attrib_name = GLX_TRANSPARENT_RED_VALUE;
			break;
		case EGL_TRANSPARENT_GREEN_VALUE:
			glx_attrib_name = GLX_TRANSPARENT_GREEN_VALUE;
			break;
		case EGL_TRANSPARENT_BLUE_VALUE:
			glx_attrib_name = GLX_TRANSPARENT_BLUE_VALUE;
			break;

		default: return false;
	}
	return true;
}
//------------------------------------------------------------------------------
bool eglplus_egl_glx_TranslateAttrib(
	EGLint egl_attrib_name,
	EGLint egl_attrib_value,
	int& glx_attrib_name,
	int& glx_attrib_value
)
{
	if(!eglplus_egl_glx_TranslateAttribName(
		egl_attrib_name,
		glx_attrib_name
	)) return false;

	switch(egl_attrib_name)
	{
		case EGL_BUFFER_SIZE:
		{
			if(egl_attrib_value == EGL_DONT_CARE)
				glx_attrib_value = 0;
			else glx_attrib_value = egl_attrib_value;
			break;
		}
		case EGL_SURFACE_TYPE:
		{
			glx_attrib_value = 0;
			if((egl_attrib_value & EGL_WINDOW_BIT) == EGL_WINDOW_BIT)
				glx_attrib_value |= GLX_WINDOW_BIT;
			if((egl_attrib_value & EGL_PIXMAP_BIT) == EGL_PIXMAP_BIT)
				glx_attrib_value |= GLX_PIXMAP_BIT;
			if((egl_attrib_value & EGL_PBUFFER_BIT) == EGL_PBUFFER_BIT)
				glx_attrib_value |= GLX_PBUFFER_BIT;
			break;
		}
		case EGL_NATIVE_VISUAL_TYPE:
		{
			if(egl_attrib_value == EGL_DONT_CARE)
				glx_attrib_value = EGL_DONT_CARE;
			else glx_attrib_value = egl_attrib_value;
			break;
		}
		case EGL_NATIVE_RENDERABLE:
		{
			if(egl_attrib_value == EGL_TRUE)
				glx_attrib_value = True;
			else if(egl_attrib_value == EGL_FALSE)
				glx_attrib_value = False;
			else return false;
			break;
		}
		case EGL_CONFIG_CAVEAT:
		{
			switch(egl_attrib_value)
			{
				case EGL_NONE:
					glx_attrib_value = GLX_NONE;
					break;
				case EGL_SLOW_CONFIG:
					glx_attrib_value = GLX_SLOW_CONFIG;
					break;
				case EGL_NON_CONFORMANT_CONFIG:
					glx_attrib_value =
						GLX_NON_CONFORMANT_CONFIG;
					break;
			}
			break;
		}
		case EGL_TRANSPARENT_TYPE:
		{
			switch(egl_attrib_value)
			{
				case EGL_NONE:
					glx_attrib_value = GLX_NONE;
					break;
				case EGL_TRANSPARENT_RGB:
					glx_attrib_value = GLX_TRANSPARENT_RGB;
					break;
			}
			break;
		}
		case EGL_SAMPLES:
		{
			if(egl_attrib_value == EGL_DONT_CARE)
				glx_attrib_value = 0;
			else glx_attrib_value = egl_attrib_value;
			break;
		}
		case EGL_SAMPLE_BUFFERS:
		{
			if(egl_attrib_value == EGL_DONT_CARE)
				glx_attrib_value = 0;
			else glx_attrib_value = egl_attrib_value;
			break;
		}

		default: glx_attrib_value = int(egl_attrib_value);
	}
	return true;
}
//------------------------------------------------------------------------------
void eglplus_egl_glx_TranslateAttribBack(
	EGLint egl_attrib_name,
	EGLint& egl_attrib_value,
	int glx_attrib_value
)
{
	switch(egl_attrib_name)
	{
		case EGL_SURFACE_TYPE:
		{
			egl_attrib_value = 0;
			if((glx_attrib_value & GLX_WINDOW_BIT) == GLX_WINDOW_BIT)
				egl_attrib_value |= EGL_WINDOW_BIT;
			if((glx_attrib_value & GLX_PIXMAP_BIT) == GLX_PIXMAP_BIT)
				egl_attrib_value |= EGL_PIXMAP_BIT;
			if((glx_attrib_value & GLX_PBUFFER_BIT) == GLX_PBUFFER_BIT)
				egl_attrib_value |= EGL_PBUFFER_BIT;
			break;
		}
		case EGL_NATIVE_RENDERABLE:
		{
			if(glx_attrib_value == True)
				egl_attrib_value = EGL_TRUE;
			else	egl_attrib_value = EGL_FALSE;
			break;
		}
		case EGL_CONFIG_CAVEAT:
		{
			switch(glx_attrib_value)
			{
				case GLX_NONE:
					egl_attrib_value = EGL_NONE;
					break;
				case GLX_SLOW_CONFIG:
					egl_attrib_value = EGL_SLOW_CONFIG;
					break;
				case GLX_NON_CONFORMANT_CONFIG:
					egl_attrib_value =
						EGL_NON_CONFORMANT_CONFIG;
					break;
			}
			break;
		}
		case EGL_TRANSPARENT_TYPE:
		{
			switch(glx_attrib_value)
			{
				case GLX_TRANSPARENT_INDEX:
				case GLX_NONE:
					egl_attrib_value = EGL_NONE;
					break;
				case GLX_TRANSPARENT_RGB:
					egl_attrib_value = EGL_TRANSPARENT_RGB;
					break;
			}
			break;
		}

		default: egl_attrib_value = EGLint(glx_attrib_value);
	}
}
//------------------------------------------------------------------------------
// EGL API
//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------
// eglGetConfigs
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigs(
	EGLDisplay display,
	EGLConfig * configs,
	EGLint config_size,
	EGLint * num_configs
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_FALSE;
	}

	if(!display->initialized())
	{
		eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	if(!num_configs)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}

	int num_glx_configs = 0;
	::GLXFBConfig * glx_configs = ::glXGetFBConfigs(
		display->_x_open_display,
		DefaultScreen(display->_x_open_display),
		&num_glx_configs
	);

	if(configs)
	{
		int n = (num_glx_configs < int(config_size))?
			num_glx_configs:
			int(config_size);

		for(int i=0; i<n; ++i)
		{
			configs[i]._glx_fb_config =
				static_cast<void*>(glx_configs[i]);
		}
		*num_configs = n>=0?n:0;
	}
	else *num_configs = EGLint(num_glx_configs);
	return EGL_TRUE;
}
//------------------------------------------------------------------------------
// eglChooseConfig
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglChooseConfig(
	EGLDisplay display,
	const EGLint *egl_attrib_list,
	EGLConfig *configs,
	EGLint config_size,
	EGLint *num_configs
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_FALSE;
	}

	if(!display->initialized())
	{
		eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	EGLint empty_list = EGL_NONE;
	if(!egl_attrib_list)
	{
		egl_attrib_list = &empty_list;
	}

	std::vector<int> glx_attrib_list(1);
	bool unsupported_config = false;

	if(*egl_attrib_list != EGL_NONE)
	{
		unsigned attr_count = 0;
		const EGLint *egl_attrib = egl_attrib_list;
		while(*egl_attrib != EGL_NONE)
		{
			switch(*(++egl_attrib))
			{
				case EGL_NONE:
				{
					eglplus_egl_SetErrorCode(EGL_BAD_ATTRIBUTE);
					return EGL_FALSE;
				}
				case EGL_SURFACE_TYPE:
				case EGL_COLOR_BUFFER_TYPE:
				{
					attr_count += 2;
					break;
				}
				default: ++attr_count;
			}
			++egl_attrib;
		}
		glx_attrib_list.resize(2*attr_count+1);

		attr_count = 0;
		egl_attrib = egl_attrib_list;
		while(*egl_attrib != EGL_NONE)
		{
			EGLint egl_attrib_name = *(egl_attrib++);
			EGLint egl_attrib_value= *(egl_attrib++);
			int glx_attrib_name, glx_attrib_value;

			bool special_attrib = true;
			bool unsupported_attrib = false;
			switch(egl_attrib_name)
			{
				case EGL_MIN_SWAP_INTERVAL:
				case EGL_MAX_SWAP_INTERVAL:
				{
					// just skip this
					break;
				}
				case EGL_ALPHA_MASK_SIZE:
				case EGL_LUMINANCE_SIZE:
				{
					// must be zero (or don't care)
					if(egl_attrib_value != 0)
					{
						unsupported_attrib = true;
					}
					break;
				}
				case EGL_RENDERABLE_TYPE:
				{
					if(egl_attrib_value != EGL_OPENGL_BIT)
					{
						unsupported_attrib = true;
					}
					break;
				}
				case EGL_SURFACE_TYPE:
				{
					bool doublebuffer =
						(egl_attrib_value & EGL_WINDOW_BIT)||
						(egl_attrib_value & EGL_PBUFFER_BIT);

					glx_attrib_list[attr_count++] =
						GLX_DOUBLEBUFFER;
					glx_attrib_list[attr_count++] =
						doublebuffer?True:False;
					break;
				}
				case EGL_COLOR_BUFFER_TYPE:
				{
					if(egl_attrib_value == EGL_RGB_BUFFER)
					{
						glx_attrib_list[attr_count++] =
							GLX_RENDER_TYPE;
						glx_attrib_list[attr_count++] =
							GLX_RGBA_BIT;
						glx_attrib_list[attr_count++] =
							GLX_X_VISUAL_TYPE;
						glx_attrib_list[attr_count++] =
							GLX_TRUE_COLOR;
					}
					else unsupported_attrib = true;
					break;
				}
				default: special_attrib = false;
			}

			if(special_attrib)
			{
				if(unsupported_attrib)
				{
					if(egl_attrib_value != EGL_DONT_CARE)
					{
						unsupported_config = true;
						break;
					}
				}
			}
			else if(eglplus_egl_glx_TranslateAttrib(
				egl_attrib_name,
				egl_attrib_value,
				glx_attrib_name,
				glx_attrib_value
			))
			{
				glx_attrib_list[attr_count++] = glx_attrib_name;
				glx_attrib_list[attr_count++] = glx_attrib_value;
			}
			else
			{
				eglplus_egl_SetErrorCode(EGL_BAD_ATTRIBUTE);
				return EGL_FALSE;
			}
		}
		glx_attrib_list.resize(2*attr_count+1);
	}
	glx_attrib_list.back() = None;

	if(unsupported_config)
	{
		*num_configs = 0;
		return EGL_TRUE;
	}

	int num_glx_configs = 0;
	::GLXFBConfig * glx_configs = ::glXChooseFBConfig(
		display->_x_open_display,
		DefaultScreen(display->_x_open_display),
		glx_attrib_list.data(),
		&num_glx_configs
	);

	if(configs)
	{
		int n = (num_glx_configs < int(config_size))?
			num_glx_configs:
			int(config_size);

		for(int i=0; i<n; ++i)
		{
			configs[i]._glx_fb_config =
				static_cast<void*>(glx_configs[i]);
		}
		*num_configs = n>=0?n:0;
	}
	else *num_configs = EGLint(num_glx_configs);

	return EGL_TRUE;
}
//------------------------------------------------------------------------------
// eglGetConfigAttrib
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigAttrib(
	EGLDisplay display,
	EGLConfig config,
	EGLint egl_attrib_name,
	EGLint *egl_attrib_value
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_FALSE;
	}

	if(!display->initialized())
	{
		eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	if(!egl_attrib_value)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}

	int glx_attrib_name;

	switch(egl_attrib_name)
	{
		case EGL_COLOR_BUFFER_TYPE:
		{
			*egl_attrib_value = EGL_RGB_BUFFER;
			// TODO: are luminance buffers supported by GLX?
			return EGL_TRUE;
		}
		case EGL_RENDERABLE_TYPE:
		case EGL_CONFORMANT:
		{
			*egl_attrib_value = EGL_OPENGL_BIT;
			//TODO | EGL_OPENGL_ES3_BIT_KHR;
			return EGL_TRUE;
		}

		case EGL_SAMPLES:
		case EGL_SAMPLE_BUFFERS:
		case EGL_LUMINANCE_SIZE:
		case EGL_ALPHA_MASK_SIZE:
		case EGL_MIN_SWAP_INTERVAL:
		case EGL_MAX_SWAP_INTERVAL:
		{
			*egl_attrib_value = 0;
			return EGL_TRUE;
		}

		default:
		{
			if(!eglplus_egl_glx_TranslateAttribName(
				egl_attrib_name,
				glx_attrib_name
			))
			{
				eglplus_egl_SetErrorCode(EGL_BAD_ATTRIBUTE);
				return EGL_FALSE;
			}
		}
	}

	int glx_attrib_value = 0;

	int glx_result = ::glXGetFBConfigAttrib(
		display->_x_open_display,
		static_cast< ::GLXFBConfig>(config._glx_fb_config),
		glx_attrib_name,
		&glx_attrib_value
	);

	if(glx_result == Success)
	{
		eglplus_egl_glx_TranslateAttribBack(
			egl_attrib_name,
			*egl_attrib_value,
			glx_attrib_value
		);
		return EGL_TRUE;
	}
	else if(glx_result == GLX_BAD_ATTRIBUTE)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_ATTRIBUTE);
	}
	else
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
	}

	return EGL_FALSE;
}
//------------------------------------------------------------------------------
} // extern "C"
