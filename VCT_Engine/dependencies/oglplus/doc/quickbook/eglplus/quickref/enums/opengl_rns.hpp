//  File doc/quickbook/eglplus/quickref/enums/opengl_rns.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/opengl_rns.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_opengl_rns

enum class OpenGLResetNotificationStrategy : EGLenum
{
	NoResetNotification = EGL_NO_RESET_NOTIFICATION,
	LoseContextOnReset  = EGL_LOSE_CONTEXT_ON_RESET
};

template <>
__Range<OpenGLResetNotificationStrategy> __EnumValueRange<OpenGLResetNotificationStrategy>(void);

__StrCRef __EnumValueName(OpenGLResetNotificationStrategy);

//]
