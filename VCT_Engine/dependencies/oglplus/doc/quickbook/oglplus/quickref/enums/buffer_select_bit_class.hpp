//  File doc/quickbook/oglplus/quickref/enums/buffer_select_bit_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_select_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_select_bit_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__BufferSelectBit> class Transform>
class __EnumToClass<Base, __BufferSelectBit, Transform> /*<
Specialization of __EnumToClass for the __BufferSelectBit enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<BufferSelectBit::ColorBuffer>
		ColorBuffer;
	Transform<BufferSelectBit::DepthBuffer>
		DepthBuffer;
	Transform<BufferSelectBit::StencilBuffer>
		StencilBuffer;
};

} // namespace enums
#endif
//]

