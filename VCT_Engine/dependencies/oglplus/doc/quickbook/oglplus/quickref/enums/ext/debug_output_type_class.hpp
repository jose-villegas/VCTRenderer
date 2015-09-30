//  File doc/quickbook/oglplus/quickref/enums/ext/debug_output_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_output_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_output_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugOutputARBType> class Transform>
class __EnumToClass<Base, __DebugOutputARBType, Transform> /*<
Specialization of __EnumToClass for the __DebugOutputARBType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugOutputARBType::Error>
		Error;
	Transform<DebugOutputARBType::DeprecatedBehavior>
		DeprecatedBehavior;
	Transform<DebugOutputARBType::UndefinedBehavior>
		UndefinedBehavior;
	Transform<DebugOutputARBType::Portability>
		Portability;
	Transform<DebugOutputARBType::Performance>
		Performance;
	Transform<DebugOutputARBType::Other>
		Other;
	Transform<DebugOutputARBType::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

