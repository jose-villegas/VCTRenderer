//  File doc/quickbook/oglplus/quickref/enums/ext/debug_severity_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_severity.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_severity_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugSeverity> class Transform>
class __EnumToClass<Base, __DebugSeverity, Transform> /*<
Specialization of __EnumToClass for the __DebugSeverity enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugSeverity::High>
		High;
	Transform<DebugSeverity::Medium>
		Medium;
	Transform<DebugSeverity::Low>
		Low;
	Transform<DebugSeverity::Notification>
		Notification;
	Transform<DebugSeverity::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

