//  File doc/quickbook/oglplus/quickref/enums/color_buffer_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/color_buffer.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_color_buffer_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ColorBuffer> class Transform>
class __EnumToClass<Base, __ColorBuffer, Transform> /*<
Specialization of __EnumToClass for the __ColorBuffer enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ColorBuffer::None>
		None;
	Transform<ColorBuffer::FrontLeft>
		FrontLeft;
	Transform<ColorBuffer::FrontRight>
		FrontRight;
	Transform<ColorBuffer::BackLeft>
		BackLeft;
	Transform<ColorBuffer::BackRight>
		BackRight;
	Transform<ColorBuffer::Front>
		Front;
	Transform<ColorBuffer::Back>
		Back;
	Transform<ColorBuffer::Left>
		Left;
	Transform<ColorBuffer::Right>
		Right;
	Transform<ColorBuffer::FrontAndBack>
		FrontAndBack;
};

} // namespace enums
#endif
//]

