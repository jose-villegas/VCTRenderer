//  File doc/quickbook/eglplus/quickref/enums/color_buffer_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/color_buffer_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_color_buffer_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ColorBufferType> class Transform>
class __EnumToClass<Base, __ColorBufferType, Transform> /*<
Specialization of __EnumToClass for the __ColorBufferType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ColorBufferType::RGBBuffer>
		RGBBuffer;
	Transform<ColorBufferType::LuminanceBuffer>
		LuminanceBuffer;
};

} // namespace enums
#endif
//]

