//  File doc/quickbook/eglplus/quickref/enums/config_caveat.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/config_caveat.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_config_caveat

enum class ConfigCaveat : EGLenum
{
	None                = EGL_NONE,
	SlowConfig          = EGL_SLOW_CONFIG,
	NonConformantConfig = EGL_NON_CONFORMANT_CONFIG
};

template <>
__Range<ConfigCaveat> __EnumValueRange<ConfigCaveat>(void);

__StrCRef __EnumValueName(ConfigCaveat);

//]
