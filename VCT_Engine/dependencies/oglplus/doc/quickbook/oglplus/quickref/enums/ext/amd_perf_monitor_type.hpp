//  File doc/quickbook/oglplus/quickref/enums/ext/amd_perf_monitor_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/amd_perf_monitor_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_amd_perf_monitor_type

enum class PerfMonitorAMDType : GLenum
{
	UnsignedInt   = GL_UNSIGNED_INT,
	Float         = GL_FLOAT,
	UnsignedInt64 = GL_UNSIGNED_INT64_AMD,
	Percentage    = GL_PERCENTAGE_AMD
};

template <>
__Range<PerfMonitorAMDType> __EnumValueRange<PerfMonitorAMDType>(void);

__StrCRef __EnumValueName(PerfMonitorAMDType);

//]
