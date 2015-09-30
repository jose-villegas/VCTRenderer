/**
 *  .file EGL/glx/context.cpp
 *  .brief EGL rendering context implementation
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <EGL/egl.h>

#include "context.hpp"
#include "surface.hpp"
#include "display.hpp"
#include "error.hpp"
#include "threads.hpp"
#include "../api.hpp"

#include <pthread.h>

#include <set>
#include <vector>
#include <cassert>

//------------------------------------------------------------------------------
static std::set<eglplus_egl_glx_ContextImpl*> eglplus_egl_glx_Contexts;
static ::pthread_mutex_t eglplus_egl_Contexts_mutex = PTHREAD_MUTEX_INITIALIZER;
//------------------------------------------------------------------------------
// eglplus_egl_glx_ContextImpl
//------------------------------------------------------------------------------
eglplus_egl_glx_ContextImpl::eglplus_egl_glx_ContextImpl(
	::GLXContext context,
	::GLXPbuffer empty_surf
): _glx_context(context)
 , _glx_empty_surf(empty_surf)
{
	::pthread_mutex_lock(&eglplus_egl_Contexts_mutex);
	eglplus_egl_glx_Contexts.insert(this);
	::pthread_mutex_unlock(&eglplus_egl_Contexts_mutex);
}
//------------------------------------------------------------------------------
eglplus_egl_glx_ContextImpl::~eglplus_egl_glx_ContextImpl(void)
{
	::pthread_mutex_lock(&eglplus_egl_Contexts_mutex);
	eglplus_egl_glx_Contexts.erase(this);
	::pthread_mutex_unlock(&eglplus_egl_Contexts_mutex);

	assert(!_glx_context);
}
//------------------------------------------------------------------------------
void eglplus_egl_glx_ContextImpl::_cleanup( ::Display* display)
{
	if(_glx_empty_surf != ::GLXPbuffer(0))
	{
		::glXDestroyPbuffer(display, _glx_empty_surf);
		_glx_empty_surf = ::GLXPbuffer(0);
	}
	::glXDestroyContext(display, _glx_context);
	_glx_context = ::GLXContext(0);
}
//------------------------------------------------------------------------------
bool eglplus_egl_valid_context(EGLContext context)
{
	::pthread_mutex_lock(&eglplus_egl_Contexts_mutex);
	bool result =
		eglplus_egl_glx_Contexts.find(context) !=
		eglplus_egl_glx_Contexts.end();
	::pthread_mutex_unlock(&eglplus_egl_Contexts_mutex);

	return result;
}
//------------------------------------------------------------------------------
// glXCreateContextAttribsARB
//------------------------------------------------------------------------------
typedef ::GLXContext (*glXCreateContextAttribsARBProc)(
	::Display*,
	::GLXFBConfig,
	::GLXContext,
	Bool,
	const int*
);
static glXCreateContextAttribsARBProc eglplus_egl_glXCreateContextAttribsARB = 0;
//------------------------------------------------------------------------------
// EGL API
//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------
// eglCreateContext
//------------------------------------------------------------------------------
EGLAPI EGLContext EGLAPIENTRY
eglCreateContext(
	EGLDisplay display,
	EGLConfig config,
	EGLContext egl_share_context,
	const EGLint *egl_attrib_list
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_NO_CONTEXT;
	}

	if(!display->initialized())
	{
		eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
		return EGL_NO_CONTEXT;
	}

	if(!config._glx_fb_config)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_CONFIG);
		return EGL_NO_CONTEXT;
	}

	if(eglplus_egl_CurrentAPI != EGL_OPENGL_API)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_MATCH);
		return EGL_NO_CONTEXT;
	}

	if(eglplus_egl_glXCreateContextAttribsARB == nullptr)
	{
		eglplus_egl_glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
		glXGetProcAddressARB((const unsigned char*)"glXCreateContextAttribsARB");
	}

	if(eglplus_egl_glXCreateContextAttribsARB == nullptr)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_NO_CONTEXT;
	}

	::GLXContext glx_share_context = ::GLXContext(0);

	if(egl_share_context != EGL_NO_CONTEXT)
	{
		if(eglplus_egl_valid_context(egl_share_context))
		{
			glx_share_context = egl_share_context->_glx_context;
		}
		else
		{
			eglplus_egl_SetErrorCode(EGL_BAD_CONFIG);
			return EGL_NO_CONTEXT;
		}
	}

	EGLint empty_list = EGL_NONE;
	if(!egl_attrib_list)
	{
		egl_attrib_list = &empty_list;
	}

	int glx_attrib_count = 0;

	if(*egl_attrib_list != EGL_NONE)
	{
		const EGLint* tmp_attrib_list = egl_attrib_list;
		while(*tmp_attrib_list != EGL_NONE)
		{
			bool bad_attrib = false;

			switch(*tmp_attrib_list++)
			{
				case EGL_CONTEXT_MAJOR_VERSION:
				case EGL_CONTEXT_MINOR_VERSION:
					break;
				case 0x30FC: // EGL_CONTEXT_FLAGS_KHR:
				{
					if((*tmp_attrib_list & ~0x07) != 0)
					{
						bad_attrib = true;
					}
					break;
				}
				case EGL_CONTEXT_OPENGL_DEBUG:
				case EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE:
				case EGL_CONTEXT_OPENGL_ROBUST_ACCESS:
				{
					switch(*tmp_attrib_list)
					{
						case EGL_TRUE:
						case EGL_FALSE:
							break;
						default: bad_attrib = true;
					}
					break;
				}
				case EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY:
				{
					switch(*tmp_attrib_list)
					{
						case 0x31BE:
						case 0x31BF:
							break;
						default: bad_attrib = true;
					}
					break;
				}
				case EGL_CONTEXT_OPENGL_PROFILE_MASK:
				{
					if((*tmp_attrib_list & ~0x03) != 0)
					{
						bad_attrib = true;
					}
					break;
				}
				default: bad_attrib = true;
			}

			if(bad_attrib)
			{
				eglplus_egl_SetErrorCode(EGL_BAD_ATTRIBUTE);
				return EGL_NO_CONTEXT;
			}
			++tmp_attrib_list;
			glx_attrib_count += 2;
		}
	}

	std::vector<int> glx_attrib_list(glx_attrib_count+1);
	glx_attrib_count = 0;
	int glx_context_flags_pos = 0;

	if(*egl_attrib_list != EGL_NONE)
	{
		const EGLint* tmp_attrib_list = egl_attrib_list;
		while(*tmp_attrib_list != EGL_NONE)
		{
			switch(*tmp_attrib_list++)
			{
				case EGL_CONTEXT_MAJOR_VERSION:
				{
					glx_attrib_list[glx_attrib_count++] =
						//GLX_CONTEXT_MAJOR_VERSION_ARB
						0x2091;
					glx_attrib_list[glx_attrib_count++] =
						int(*tmp_attrib_list++);
					break;
				}
				case EGL_CONTEXT_MINOR_VERSION:
				{
					glx_attrib_list[glx_attrib_count++] =
						//GLX_CONTEXT_MINOR_VERSION_ARB
						0x2092;
					glx_attrib_list[glx_attrib_count++] =
						int(*tmp_attrib_list++);
					break;
				}
				case 0x30FC: //EGL_CONTEXT_FLAGS_KHR
				{
					//GLX_CONTEXT_FLAGS_ARB
					glx_attrib_list[glx_attrib_count++] = 0x2094;
					EGLint egl_flags = *tmp_attrib_list++;
					int glx_flags = 0;

					// EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR
					if((egl_flags & 0x00000001) == 0x00000001)
						glx_flags |= 0x0001;

					// EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR
					if((egl_flags & 0x00000002) == 0x00000002)
						glx_flags |= 0x0002;

					// EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR
					if((egl_flags & 0x00000004) == 0x00000004)
						glx_flags |= 0x0004;

					glx_attrib_list[glx_attrib_count++] =
						glx_flags;
					break;
				}
				case EGL_CONTEXT_OPENGL_DEBUG:
				{
					if(!glx_context_flags_pos)
					{
						// GLX_CONTEXT_FLAGS_ARB
						glx_attrib_list[glx_attrib_count++] = 0x2094;
						glx_context_flags_pos = glx_attrib_count;
						glx_attrib_list[glx_attrib_count++] = 0;
					}
					if(*tmp_attrib_list++ == EGL_TRUE)
						glx_attrib_list[glx_context_flags_pos] |= 0x0001;
					break;
				}
				case EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE:
				{
					if(!glx_context_flags_pos)
					{
						// GLX_CONTEXT_FLAGS_ARB
						glx_attrib_list[glx_attrib_count++] = 0x2094;
						glx_context_flags_pos = glx_attrib_count;
						glx_attrib_list[glx_attrib_count++] = 0;
					}
					if(*tmp_attrib_list++ == EGL_TRUE)
						glx_attrib_list[glx_context_flags_pos] |= 0x0002;
					break;
				}
				case EGL_CONTEXT_OPENGL_ROBUST_ACCESS:
				{
					if(!glx_context_flags_pos)
					{
						// GLX_CONTEXT_FLAGS_ARB
						glx_attrib_list[glx_attrib_count++] = 0x2094;
						glx_context_flags_pos = glx_attrib_count;
						glx_attrib_list[glx_attrib_count++] = 0;
					}
					if(*tmp_attrib_list++ == EGL_TRUE)
						glx_attrib_list[glx_context_flags_pos] |= 0x0004;
					break;
				}
				case EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY:
				{
					// GLX_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB
					glx_attrib_list[glx_attrib_count++] = 0x8256;

					switch(*tmp_attrib_list++)
					{
						case 0x31BE: // EGL_NO_RESET_NOTIFICATION
						{
							glx_attrib_list[glx_attrib_count++] = 0x8261;
							break;
						}
						case 0x31BF: // EGL_LOSE_CONTEXT_ON_RESET
						{
							glx_attrib_list[glx_attrib_count++] = 0x8252;
							break;
						}
						default: assert(!"Invalid value");
					}
					break;
				}
				case EGL_CONTEXT_OPENGL_PROFILE_MASK:
				{
					//GLX_CONTEXT_PROFILE_MASK_ARB
					glx_attrib_list[glx_attrib_count++] = 0x9126;
					EGLint egl_flags = *tmp_attrib_list++;
					int glx_flags = 0;

					if((egl_flags & EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT) != 0)
						glx_flags |= 0x0001;

					if((egl_flags & EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT) != 0)
						glx_flags |= 0x0002;

					glx_attrib_list[glx_attrib_count++] =
						glx_flags;
					break;
				}
				default:;
			}
		}
	}
	glx_attrib_list[glx_attrib_count] = None;

	::GLXContext context = eglplus_egl_glXCreateContextAttribsARB(
		display->_x_open_display,
		static_cast< ::GLXFBConfig>(config._glx_fb_config),
		glx_share_context,
		True, // direct rendering
		glx_attrib_list.data()
	);

	if(context == ::GLXContext(0))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_ALLOC);
		return EGL_NO_CONTEXT;
	}

	::XSync(display->_x_open_display, False);

	int empty_surf_attr[] = {
		GLX_PBUFFER_WIDTH, 0,
		GLX_PBUFFER_HEIGHT, 0,
		GLX_PRESERVED_CONTENTS, False,
		None
	};
	::GLXPbuffer empty_surf = ::glXCreatePbuffer(
		display->_x_open_display,
		static_cast< ::GLXFBConfig>(config._glx_fb_config),
		empty_surf_attr
	);

	try { return new eglplus_egl_glx_ContextImpl(context, empty_surf); }
	catch(...)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_ALLOC);
		return EGL_NO_CONTEXT;
	}
}
//------------------------------------------------------------------------------
// eglDestroyContext
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyContext(
	EGLDisplay display,
	EGLContext context
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

	if(!eglplus_egl_valid_context(context))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_CONTEXT);
		return EGL_FALSE;
	}

	context->_cleanup(display->_x_open_display);
	delete context;
}
//------------------------------------------------------------------------------
// eglMakeCurrent
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglMakeCurrent(
	EGLDisplay display,
	EGLSurface egl_draw,
	EGLSurface egl_read,
	EGLContext context
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_FALSE;
	}

	::GLXContext glx_context = ::GLXContext(0);
	::GLXDrawable glx_draw = ::GLXDrawable(0);
	::GLXDrawable glx_read = ::GLXDrawable(0);

	if(context != EGL_NO_CONTEXT)
	{
		if(!eglplus_egl_valid_context(context))
		{
			eglplus_egl_SetErrorCode(EGL_BAD_CONTEXT);
			return EGL_FALSE;
		}

		if((egl_draw == EGL_NO_SURFACE) != (egl_read == EGL_NO_SURFACE))
		{
			eglplus_egl_SetErrorCode(EGL_BAD_MATCH);
			return EGL_FALSE;
		}

		if(egl_draw != EGL_NO_SURFACE)
		{
			if(!eglplus_egl_valid_surface(egl_draw))
			{
				eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
				return EGL_FALSE;
			}
			glx_draw = egl_draw->_glx_drawable;
		}
		else if(context->_glx_empty_surf)
		{
			// KHR_surfaceless_context
			glx_draw = context->_glx_empty_surf;
		}
		else
		{
			eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
			return EGL_FALSE;
		}

		if(egl_read != EGL_NO_SURFACE)
		{
			if(!eglplus_egl_valid_surface(egl_read))
			{
				eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
				return EGL_FALSE;
			}
			glx_read = egl_read->_glx_drawable;
		}
		else if(context->_glx_empty_surf)
		{
			// KHR_surfaceless_context
			glx_read = context->_glx_empty_surf;
		}
		else
		{
			eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
			return EGL_FALSE;
		}

		if(!display->initialized())
		{
			eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
			return EGL_FALSE;
		}

		glx_context = context->_glx_context;
	}
	else
	{
		if(egl_draw != EGL_NO_SURFACE)
		{
			eglplus_egl_SetErrorCode(EGL_BAD_MATCH);
			return EGL_FALSE;
		}

		if(egl_read != EGL_NO_SURFACE)
		{
			eglplus_egl_SetErrorCode(EGL_BAD_MATCH);
			return EGL_FALSE;
		}
	}

	if(::glXMakeContextCurrent(
		display->_x_open_display,
		glx_draw,
		glx_read,
		glx_context
	) != True)
	{
		return EGL_FALSE;
	}

	eglplus_egl_glx_Current& current = eglplus_egl_CurrentRef();

	current.display = display;
	current.draw_surface = egl_draw;
	current.read_surface = egl_read;
	current.context = context;

	return EGL_TRUE;
}
//------------------------------------------------------------------------------
// eglGetCurrentContext
//------------------------------------------------------------------------------
EGLAPI EGLContext EGLAPIENTRY
eglGetCurrentContext(void)
{
	return eglplus_egl_CurrentRef().context;
}
//------------------------------------------------------------------------------
// eglGetCurrentSurface
//------------------------------------------------------------------------------
EGLAPI EGLSurface EGLAPIENTRY
eglGetCurrentSurface(EGLint which)
{
	switch(which)
	{
		case EGL_DRAW:
		{
			return eglplus_egl_CurrentRef().draw_surface;
		}
		case EGL_READ:
		{
			return eglplus_egl_CurrentRef().read_surface;
		}
		default:;
	}

	eglplus_egl_SetErrorCode(EGL_BAD_PARAMETER);
	return EGL_NO_SURFACE;
}
//------------------------------------------------------------------------------
// eglGetCurrentDisplay
//------------------------------------------------------------------------------
EGLAPI EGLDisplay EGLAPIENTRY
eglGetCurrentDisplay(void)
{
	return eglplus_egl_CurrentRef().display;
}
//------------------------------------------------------------------------------
// eglQueryContext
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglQueryContext(
	EGLDisplay display,
	EGLContext context,
	EGLint egl_attribute,
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

	if(!eglplus_egl_valid_context(context))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_CONTEXT);
		return EGL_FALSE;
	}

	int glx_attribute = 0;

	switch(egl_attribute)
	{
		case EGL_CONTEXT_CLIENT_TYPE:
		{
			if(egl_attrib_value)
				*egl_attrib_value = EGL_OPENGL_BIT;
			return EGL_TRUE;
		}
		case EGL_CONTEXT_CLIENT_VERSION:
		{
			if(egl_attrib_value)
				*egl_attrib_value = 3;
			return EGL_TRUE;
		}
		case EGL_RENDER_BUFFER:
		{
			//TODO
			if(egl_attrib_value)
				*egl_attrib_value = EGL_BACK_BUFFER;
			return EGL_TRUE;
		}
		case EGL_CONFIG_ID:
		{
			glx_attribute = GLX_FBCONFIG_ID;
			break;
		}
		default:
		{
			eglplus_egl_SetErrorCode(EGL_BAD_ATTRIBUTE);
			return EGL_FALSE;
		}
	}

	int glx_attrib_value = 0;

	if(glXQueryContext(
		display->_x_open_display,
		context->_glx_context,
		glx_attribute,
		&glx_attrib_value
	) != 0)
	{
		return EGL_FALSE;
	}

	switch(egl_attribute)
	{
		default:
		{
			if(egl_attrib_value)
				*egl_attrib_value = glx_attrib_value;
		}
	}

	return EGL_TRUE;
}
//------------------------------------------------------------------------------
// eglWaitClient
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglWaitClient(void)
{
	eglplus_egl_glx_Current& current = eglplus_egl_CurrentRef();

	if(!current.context)
	{
		return EGL_TRUE;
	}

	if(!current.draw_surface)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}
	if(!current.read_surface)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}
	// TODO finish error handling

	::glXWaitGL();
	return EGL_TRUE;
}
//------------------------------------------------------------------------------
// eglWaitGL
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglWaitGL(void)
{
	// TODO: this is not fully conforming to spec.
	return eglWaitClient();
}
//------------------------------------------------------------------------------
// eglWaitNative
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglWaitNative(EGLint engine)
{
	eglplus_egl_glx_Current& current = eglplus_egl_CurrentRef();

	if(!current.context)
	{
		return EGL_TRUE;
	}

	if(engine != EGL_CORE_NATIVE_ENGINE)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}
	if(!current.draw_surface)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}

	if(!current.read_surface)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}
	// TODO finish error handling

	::glXWaitX();
	return EGL_TRUE;
}
//------------------------------------------------------------------------------

} // extern "C"
