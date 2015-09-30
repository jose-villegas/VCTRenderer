//  File doc/quickbook/oglplus/quickref/enums/image_unit_format_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/image_unit_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_image_unit_format_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ImageUnitFormat> class Transform>
class __EnumToClass<Base, __ImageUnitFormat, Transform> /*<
Specialization of __EnumToClass for the __ImageUnitFormat enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ImageUnitFormat::RGBA32F>
		RGBA32F;
	Transform<ImageUnitFormat::RGBA16F>
		RGBA16F;
	Transform<ImageUnitFormat::RG32F>
		RG32F;
	Transform<ImageUnitFormat::RG16F>
		RG16F;
	Transform<ImageUnitFormat::R11FG11FB10F>
		R11FG11FB10F;
	Transform<ImageUnitFormat::R32F>
		R32F;
	Transform<ImageUnitFormat::R16F>
		R16F;
	Transform<ImageUnitFormat::RGBA32UI>
		RGBA32UI;
	Transform<ImageUnitFormat::RGBA16UI>
		RGBA16UI;
	Transform<ImageUnitFormat::RGB10_A2UI>
		RGB10_A2UI;
	Transform<ImageUnitFormat::RGBA8UI>
		RGBA8UI;
	Transform<ImageUnitFormat::RG32UI>
		RG32UI;
	Transform<ImageUnitFormat::RG16UI>
		RG16UI;
	Transform<ImageUnitFormat::RG8UI>
		RG8UI;
	Transform<ImageUnitFormat::R32UI>
		R32UI;
	Transform<ImageUnitFormat::R16UI>
		R16UI;
	Transform<ImageUnitFormat::R8UI>
		R8UI;
	Transform<ImageUnitFormat::RGBA32I>
		RGBA32I;
	Transform<ImageUnitFormat::RGBA16I>
		RGBA16I;
	Transform<ImageUnitFormat::RGBA8I>
		RGBA8I;
	Transform<ImageUnitFormat::RG32I>
		RG32I;
	Transform<ImageUnitFormat::RG16I>
		RG16I;
	Transform<ImageUnitFormat::RG8I>
		RG8I;
	Transform<ImageUnitFormat::R32I>
		R32I;
	Transform<ImageUnitFormat::R16I>
		R16I;
	Transform<ImageUnitFormat::R8I>
		R8I;
	Transform<ImageUnitFormat::RGBA16>
		RGBA16;
	Transform<ImageUnitFormat::RGB10A2>
		RGB10A2;
	Transform<ImageUnitFormat::RGBA8>
		RGBA8;
	Transform<ImageUnitFormat::RG16>
		RG16;
	Transform<ImageUnitFormat::RG8>
		RG8;
	Transform<ImageUnitFormat::R16>
		R16;
	Transform<ImageUnitFormat::R8>
		R8;
	Transform<ImageUnitFormat::RGBA16SNorm>
		RGBA16SNorm;
	Transform<ImageUnitFormat::RGBA8SNorm>
		RGBA8SNorm;
	Transform<ImageUnitFormat::RG16SNorm>
		RG16SNorm;
	Transform<ImageUnitFormat::RG8SNorm>
		RG8SNorm;
	Transform<ImageUnitFormat::R16SNorm>
		R16SNorm;
	Transform<ImageUnitFormat::R8SNorm>
		R8SNorm;
};

} // namespace enums
#endif
//]

