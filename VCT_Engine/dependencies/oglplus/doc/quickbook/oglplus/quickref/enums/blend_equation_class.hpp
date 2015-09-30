//  File doc/quickbook/oglplus/quickref/enums/blend_equation_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/blend_equation.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_blend_equation_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__BlendEquation> class Transform>
class __EnumToClass<Base, __BlendEquation, Transform> /*<
Specialization of __EnumToClass for the __BlendEquation enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<BlendEquation::Add>
		Add;
	Transform<BlendEquation::Subtract>
		Subtract;
	Transform<BlendEquation::ReverseSubtract>
		ReverseSubtract;
	Transform<BlendEquation::Min>
		Min;
	Transform<BlendEquation::Max>
		Max;
};

} // namespace enums
#endif
//]

