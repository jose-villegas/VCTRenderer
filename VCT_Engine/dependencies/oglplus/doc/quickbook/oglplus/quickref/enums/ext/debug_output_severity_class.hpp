//  File doc/quickbook/oglplus/quickref/enums/ext/debug_output_severity_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_output_severity.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_output_severity_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugOutputARBSeverity> class Transform>
class __EnumToClass<Base, __DebugOutputARBSeverity, Transform> /*<
Specialization of __EnumToClass for the __DebugOutputARBSeverity enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugOutputARBSeverity::High>
		High;
	Transform<DebugOutputARBSeverity::Medium>
		Medium;
	Transform<DebugOutputARBSeverity::Low>
		Low;
	Transform<DebugOutputARBSeverity::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

