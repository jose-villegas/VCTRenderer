//  File doc/quickbook/oglplus/quickref/enums/sync_wait_result_class.hpp
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
//[oglplus_enums_sync_wait_result_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__SyncWaitResult> class Transform>
class __EnumToClass<Base, __SyncWaitResult, Transform> /*<
Specialization of __EnumToClass for the __SyncWaitResult enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<SyncWaitResult::ConditionSatisfied>
		ConditionSatisfied;
	Transform<SyncWaitResult::AlreadySignaled>
		AlreadySignaled;
	Transform<SyncWaitResult::TimeoutExpired>
		TimeoutExpired;
	Transform<SyncWaitResult::WaitFailed>
		WaitFailed;
};

} // namespace enums
#endif
//]

