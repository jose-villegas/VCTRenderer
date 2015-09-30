//  File doc/quickbook/oglplus/quickref/enums/compare_function_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/compare_function.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_compare_function_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__CompareFunction> class Transform>
class __EnumToClass<Base, __CompareFunction, Transform> /*<
Specialization of __EnumToClass for the __CompareFunction enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<CompareFunction::LEqual>
		LEqual;
	Transform<CompareFunction::GEqual>
		GEqual;
	Transform<CompareFunction::Less>
		Less;
	Transform<CompareFunction::Greater>
		Greater;
	Transform<CompareFunction::Equal>
		Equal;
	Transform<CompareFunction::NotEqual>
		NotEqual;
	Transform<CompareFunction::Always>
		Always;
	Transform<CompareFunction::Never>
		Never;
};

} // namespace enums
#endif
//]

