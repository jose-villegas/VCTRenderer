//  File doc/quickbook/oglplus/quickref/enums/reset_notif_strategy_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/reset_notif_strategy.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_reset_notif_strategy_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ResetNotificationStrategy> class Transform>
class __EnumToClass<Base, __ResetNotificationStrategy, Transform> /*<
Specialization of __EnumToClass for the __ResetNotificationStrategy enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ResetNotificationStrategy::NoResetNotification>
		NoResetNotification;
	Transform<ResetNotificationStrategy::LoseContextOnReset>
		LoseContextOnReset;
};

} // namespace enums
#endif
//]

