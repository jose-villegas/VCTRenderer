//  File doc/quickbook/eglplus/quickref/enums/vg_colorspace.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/vg_colorspace.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_vg_colorspace

enum class VGColorspace : EGLenum
{
	sRGB   = EGL_VG_COLORSPACE_sRGB,
	Linear = EGL_VG_COLORSPACE_LINEAR
};

template <>
__Range<VGColorspace> __EnumValueRange<VGColorspace>(void);

__StrCRef __EnumValueName(VGColorspace);

//]
