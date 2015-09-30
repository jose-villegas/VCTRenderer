//  File doc/quickbook/oglplus/quickref/enums/debug_output_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/debug_output_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_debug_output_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugOutputType> class Transform>
class __EnumToClass<Base, __DebugOutputType, Transform> /*<
Specialization of __EnumToClass for the __DebugOutputType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugOutputType::Error>
		Error;
	Transform<DebugOutputType::DeprecatedBehavior>
		DeprecatedBehavior;
	Transform<DebugOutputType::UndefinedBehavior>
		UndefinedBehavior;
	Transform<DebugOutputType::Portability>
		Portability;
	Transform<DebugOutputType::Performance>
		Performance;
	Transform<DebugOutputType::Marker>
		Marker;
	Transform<DebugOutputType::PushGroup>
		PushGroup;
	Transform<DebugOutputType::PopGroup>
		PopGroup;
	Transform<DebugOutputType::Other>
		Other;
	Transform<DebugOutputType::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

