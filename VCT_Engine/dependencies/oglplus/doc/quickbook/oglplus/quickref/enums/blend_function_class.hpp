//  File doc/quickbook/oglplus/quickref/enums/blend_function_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/blend_function.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_blend_function_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__BlendFunction> class Transform>
class __EnumToClass<Base, __BlendFunction, Transform> /*<
Specialization of __EnumToClass for the __BlendFunction enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<BlendFunction::Zero>
		Zero;
	Transform<BlendFunction::One>
		One;
	Transform<BlendFunction::SrcColor>
		SrcColor;
	Transform<BlendFunction::OneMinusSrcColor>
		OneMinusSrcColor;
	Transform<BlendFunction::DstColor>
		DstColor;
	Transform<BlendFunction::OneMinusDstColor>
		OneMinusDstColor;
	Transform<BlendFunction::SrcAlpha>
		SrcAlpha;
	Transform<BlendFunction::OneMinusSrcAlpha>
		OneMinusSrcAlpha;
	Transform<BlendFunction::DstAlpha>
		DstAlpha;
	Transform<BlendFunction::OneMinusDstAlpha>
		OneMinusDstAlpha;
	Transform<BlendFunction::ConstantColor>
		ConstantColor;
	Transform<BlendFunction::OneMinusConstantColor>
		OneMinusConstantColor;
	Transform<BlendFunction::ConstantAlpha>
		ConstantAlpha;
	Transform<BlendFunction::OneMinusConstantAlpha>
		OneMinusConstantAlpha;
	Transform<BlendFunction::SrcAlphaSaturate>
		SrcAlphaSaturate;
	Transform<BlendFunction::Src1Color>
		Src1Color;
	Transform<BlendFunction::OneMinusSrc1Color>
		OneMinusSrc1Color;
	Transform<BlendFunction::Src1Alpha>
		Src1Alpha;
	Transform<BlendFunction::OneMinusSrc1Alpha>
		OneMinusSrc1Alpha;
};

} // namespace enums
#endif
//]

