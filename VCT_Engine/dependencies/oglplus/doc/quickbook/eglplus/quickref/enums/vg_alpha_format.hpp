//  File doc/quickbook/eglplus/quickref/enums/vg_alpha_format.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/vg_alpha_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_vg_alpha_format

enum class VGAlphaFormat : EGLenum
{
	Pre    = EGL_VG_ALPHA_FORMAT_PRE,
	NonPre = EGL_VG_ALPHA_FORMAT_NONPRE
};

template <>
__Range<VGAlphaFormat> __EnumValueRange<VGAlphaFormat>(void);

__StrCRef __EnumValueName(VGAlphaFormat);

//]
