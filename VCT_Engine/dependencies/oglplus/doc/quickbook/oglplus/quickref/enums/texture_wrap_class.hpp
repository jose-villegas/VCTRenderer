//  File doc/quickbook/oglplus/quickref/enums/texture_wrap_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_wrap.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_wrap_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TextureWrap> class Transform>
class __EnumToClass<Base, __TextureWrap, Transform> /*<
Specialization of __EnumToClass for the __TextureWrap enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TextureWrap::ClampToEdge>
		ClampToEdge;
	Transform<TextureWrap::Repeat>
		Repeat;
	Transform<TextureWrap::ClampToBorder>
		ClampToBorder;
	Transform<TextureWrap::MirroredRepeat>
		MirroredRepeat;
	Transform<TextureWrap::MirrorClampToEdge>
		MirrorClampToEdge;
};

} // namespace enums
#endif
//]

