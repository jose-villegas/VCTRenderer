//  File doc/quickbook/oglplus/quickref/enums/ext/reset_notif_strategy_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/reset_notif_strategy.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_reset_notif_strategy_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ResetNotificationStrategyARB> class Transform>
class __EnumToClass<Base, __ResetNotificationStrategyARB, Transform> /*<
Specialization of __EnumToClass for the __ResetNotificationStrategyARB enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ResetNotificationStrategyARB::NoResetNotification>
		NoResetNotification;
	Transform<ResetNotificationStrategyARB::LoseContextOnReset>
		LoseContextOnReset;
};

} // namespace enums
#endif
//]

