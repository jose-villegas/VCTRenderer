//  File doc/quickbook/oglplus/quickref/enums/debug_output_severity_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/debug_output_severity.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_debug_output_severity_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugOutputSeverity> class Transform>
class __EnumToClass<Base, __DebugOutputSeverity, Transform> /*<
Specialization of __EnumToClass for the __DebugOutputSeverity enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugOutputSeverity::High>
		High;
	Transform<DebugOutputSeverity::Medium>
		Medium;
	Transform<DebugOutputSeverity::Low>
		Low;
	Transform<DebugOutputSeverity::Notification>
		Notification;
	Transform<DebugOutputSeverity::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

