//  File doc/quickbook/eglplus/quickref/enums/transparent_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/transparent_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_transparent_type

enum class TransparentType : EGLenum
{
	None           = EGL_NONE,
	TransparentRGB = EGL_TRANSPARENT_RGB
};

template <>
__Range<TransparentType> __EnumValueRange<TransparentType>(void);

__StrCRef __EnumValueName(TransparentType);

//]
