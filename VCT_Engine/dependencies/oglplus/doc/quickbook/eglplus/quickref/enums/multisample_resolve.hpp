//  File doc/quickbook/eglplus/quickref/enums/multisample_resolve.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/multisample_resolve.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_multisample_resolve

enum class MultisampleResolve : EGLenum
{
	Default = EGL_MULTISAMPLE_RESOLVE_DEFAULT,
	Box     = EGL_MULTISAMPLE_RESOLVE_BOX
};

template <>
__Range<MultisampleResolve> __EnumValueRange<MultisampleResolve>(void);

__StrCRef __EnumValueName(MultisampleResolve);

//]
