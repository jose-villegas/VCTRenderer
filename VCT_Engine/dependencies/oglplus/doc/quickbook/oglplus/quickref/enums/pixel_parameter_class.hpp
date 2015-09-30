//  File doc/quickbook/oglplus/quickref/enums/pixel_parameter_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/pixel_parameter.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_pixel_parameter_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PixelParameter> class Transform>
class __EnumToClass<Base, __PixelParameter, Transform> /*<
Specialization of __EnumToClass for the __PixelParameter enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PixelParameter::PackSwapBytes>
		PackSwapBytes;
	Transform<PixelParameter::PackLSBFirst>
		PackLSBFirst;
	Transform<PixelParameter::PackRowLength>
		PackRowLength;
	Transform<PixelParameter::PackSkipRows>
		PackSkipRows;
	Transform<PixelParameter::PackSkipPixels>
		PackSkipPixels;
	Transform<PixelParameter::PackAlignment>
		PackAlignment;
	Transform<PixelParameter::PackImageHeight>
		PackImageHeight;
	Transform<PixelParameter::PackSkipImages>
		PackSkipImages;
	Transform<PixelParameter::PackCompressedBlockWidth>
		PackCompressedBlockWidth;
	Transform<PixelParameter::PackCompressedBlockHeight>
		PackCompressedBlockHeight;
	Transform<PixelParameter::PackCompressedBlockDepth>
		PackCompressedBlockDepth;
	Transform<PixelParameter::PackCompressedBlockSize>
		PackCompressedBlockSize;
	Transform<PixelParameter::UnpackSwapBytes>
		UnpackSwapBytes;
	Transform<PixelParameter::UnpackLSBFirst>
		UnpackLSBFirst;
	Transform<PixelParameter::UnpackRowLength>
		UnpackRowLength;
	Transform<PixelParameter::UnpackSkipRows>
		UnpackSkipRows;
	Transform<PixelParameter::UnpackSkipPixels>
		UnpackSkipPixels;
	Transform<PixelParameter::UnpackAlignment>
		UnpackAlignment;
	Transform<PixelParameter::UnpackImageHeight>
		UnpackImageHeight;
	Transform<PixelParameter::UnpackSkipImages>
		UnpackSkipImages;
	Transform<PixelParameter::UnpackCompressedBlockWidth>
		UnpackCompressedBlockWidth;
	Transform<PixelParameter::UnpackCompressedBlockHeight>
		UnpackCompressedBlockHeight;
	Transform<PixelParameter::UnpackCompressedBlockDepth>
		UnpackCompressedBlockDepth;
	Transform<PixelParameter::UnpackCompressedBlockSize>
		UnpackCompressedBlockSize;
};

} // namespace enums
#endif
//]

