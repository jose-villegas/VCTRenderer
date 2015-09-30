//  File doc/quickbook/oglplus/quickref/enums/texture_swizzle_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_swizzle.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_swizzle_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TextureSwizzle> class Transform>
class __EnumToClass<Base, __TextureSwizzle, Transform> /*<
Specialization of __EnumToClass for the __TextureSwizzle enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TextureSwizzle::Red>
		Red;
	Transform<TextureSwizzle::Green>
		Green;
	Transform<TextureSwizzle::Blue>
		Blue;
	Transform<TextureSwizzle::Alpha>
		Alpha;
	Transform<TextureSwizzle::Zero>
		Zero;
	Transform<TextureSwizzle::One>
		One;
};

} // namespace enums
#endif
//]

