//  File doc/quickbook/oglplus/quickref/enums/ext/debug_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugType> class Transform>
class __EnumToClass<Base, __DebugType, Transform> /*<
Specialization of __EnumToClass for the __DebugType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugType::Error>
		Error;
	Transform<DebugType::DeprecatedBehavior>
		DeprecatedBehavior;
	Transform<DebugType::UndefinedBehavior>
		UndefinedBehavior;
	Transform<DebugType::Portability>
		Portability;
	Transform<DebugType::Performance>
		Performance;
	Transform<DebugType::Other>
		Other;
	Transform<DebugType::Marker>
		Marker;
	Transform<DebugType::PushGroup>
		PushGroup;
	Transform<DebugType::PopGroup>
		PopGroup;
	Transform<DebugType::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

