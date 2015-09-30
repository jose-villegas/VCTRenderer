//  File doc/quickbook/oglplus/quickref/enums/sync_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/sync_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_sync_type

enum class SyncType : GLenum
{
	Fence = GL_SYNC_FENCE
};

template <>
__Range<SyncType> __EnumValueRange<SyncType>(void);

__StrCRef __EnumValueName(SyncType);

//]
