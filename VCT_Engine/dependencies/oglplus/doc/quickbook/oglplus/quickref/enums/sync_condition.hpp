//  File doc/quickbook/oglplus/quickref/enums/sync_condition.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/sync_condition.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_sync_condition

enum class SyncCondition : GLenum
{
	GPUCommandsComplete = GL_SYNC_GPU_COMMANDS_COMPLETE
};

template <>
__Range<SyncCondition> __EnumValueRange<SyncCondition>(void);

__StrCRef __EnumValueName(SyncCondition);

//]
