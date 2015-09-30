//  File doc/quickbook/oglplus/quickref/enums/pixel_data_format_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/pixel_data_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_pixel_data_format_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PixelDataFormat> class Transform>
class __EnumToClass<Base, __PixelDataFormat, Transform> /*<
Specialization of __EnumToClass for the __PixelDataFormat enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PixelDataFormat::DepthComponent>
		DepthComponent;
	Transform<PixelDataFormat::DepthStencil>
		DepthStencil;
	Transform<PixelDataFormat::StencilIndex>
		StencilIndex;
	Transform<PixelDataFormat::Red>
		Red;
	Transform<PixelDataFormat::Green>
		Green;
	Transform<PixelDataFormat::Blue>
		Blue;
	Transform<PixelDataFormat::RG>
		RG;
	Transform<PixelDataFormat::RGB>
		RGB;
	Transform<PixelDataFormat::RGBA>
		RGBA;
	Transform<PixelDataFormat::BGR>
		BGR;
	Transform<PixelDataFormat::BGRA>
		BGRA;
	Transform<PixelDataFormat::RedInteger>
		RedInteger;
	Transform<PixelDataFormat::GreenInteger>
		GreenInteger;
	Transform<PixelDataFormat::BlueInteger>
		BlueInteger;
	Transform<PixelDataFormat::RGInteger>
		RGInteger;
	Transform<PixelDataFormat::RGBInteger>
		RGBInteger;
	Transform<PixelDataFormat::RGBAInteger>
		RGBAInteger;
	Transform<PixelDataFormat::BGRInteger>
		BGRInteger;
	Transform<PixelDataFormat::BGRAInteger>
		BGRAInteger;
};

} // namespace enums
#endif
//]

