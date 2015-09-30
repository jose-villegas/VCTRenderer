//  File doc/quickbook/oglplus/quickref/enums/sync_status.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/sync_status.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_sync_status

enum class SyncStatus : GLenum
{
	Signaled   = GL_SIGNALED,
	Unsignaled = GL_UNSIGNALED
};

template <>
__Range<SyncStatus> __EnumValueRange<SyncStatus>(void);

__StrCRef __EnumValueName(SyncStatus);

//]
