//  File doc/quickbook/oglplus/quickref/enums/sync_wait_result.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/sync_wait_result.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_sync_wait_result

enum class SyncWaitResult : GLenum
{
	ConditionSatisfied = GL_CONDITION_SATISFIED,
	AlreadySignaled    = GL_ALREADY_SIGNALED,
	TimeoutExpired     = GL_TIMEOUT_EXPIRED,
	WaitFailed         = GL_WAIT_FAILED
};

template <>
__Range<SyncWaitResult> __EnumValueRange<SyncWaitResult>(void);

__StrCRef __EnumValueName(SyncWaitResult);

//]
