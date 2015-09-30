//  File doc/quickbook/oglplus/quickref/enums/pixel_data_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/pixel_data_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_pixel_data_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PixelDataType> class Transform>
class __EnumToClass<Base, __PixelDataType, Transform> /*<
Specialization of __EnumToClass for the __PixelDataType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PixelDataType::UnsignedByte>
		UnsignedByte;
	Transform<PixelDataType::Byte>
		Byte;
	Transform<PixelDataType::UnsignedShort>
		UnsignedShort;
	Transform<PixelDataType::Short>
		Short;
	Transform<PixelDataType::UnsignedInt>
		UnsignedInt;
	Transform<PixelDataType::Int>
		Int;
	Transform<PixelDataType::HalfFloat>
		HalfFloat;
	Transform<PixelDataType::Float>
		Float;
	Transform<PixelDataType::UnsignedByte_3_3_2>
		UnsignedByte_3_3_2;
	Transform<PixelDataType::UnsignedByte_2_3_3_Rev>
		UnsignedByte_2_3_3_Rev;
	Transform<PixelDataType::UnsignedShort_5_6_5>
		UnsignedShort_5_6_5;
	Transform<PixelDataType::UnsignedShort_5_6_5_Rev>
		UnsignedShort_5_6_5_Rev;
	Transform<PixelDataType::UnsignedShort_4_4_4_4>
		UnsignedShort_4_4_4_4;
	Transform<PixelDataType::UnsignedShort_4_4_4_4_Rev>
		UnsignedShort_4_4_4_4_Rev;
	Transform<PixelDataType::UnsignedShort_5_5_5_1>
		UnsignedShort_5_5_5_1;
	Transform<PixelDataType::UnsignedShort_1_5_5_5_Rev>
		UnsignedShort_1_5_5_5_Rev;
	Transform<PixelDataType::UnsignedInt_8_8_8_8>
		UnsignedInt_8_8_8_8;
	Transform<PixelDataType::UnsignedInt_8_8_8_8_Rev>
		UnsignedInt_8_8_8_8_Rev;
	Transform<PixelDataType::UnsignedInt_10_10_10_2>
		UnsignedInt_10_10_10_2;
	Transform<PixelDataType::UnsignedInt_2_10_10_10_Rev>
		UnsignedInt_2_10_10_10_Rev;
	Transform<PixelDataType::UnsignedInt_24_8>
		UnsignedInt_24_8;
	Transform<PixelDataType::UnsignedInt_10f_11f_11f_Rev>
		UnsignedInt_10f_11f_11f_Rev;
	Transform<PixelDataType::UnsignedInt_5_9_9_9_Rev>
		UnsignedInt_5_9_9_9_Rev;
	Transform<PixelDataType::Float_32UnsignedInt_24_8_Rev>
		Float_32UnsignedInt_24_8_Rev;
};

} // namespace enums
#endif
//]

