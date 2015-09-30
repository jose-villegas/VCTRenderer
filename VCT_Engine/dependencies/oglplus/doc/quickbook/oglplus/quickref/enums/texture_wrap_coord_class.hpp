//  File doc/quickbook/oglplus/quickref/enums/texture_wrap_coord_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_wrap_coord.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_wrap_coord_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TextureWrapCoord> class Transform>
class __EnumToClass<Base, __TextureWrapCoord, Transform> /*<
Specialization of __EnumToClass for the __TextureWrapCoord enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TextureWrapCoord::S>
		S;
	Transform<TextureWrapCoord::T>
		T;
	Transform<TextureWrapCoord::R>
		R;
};

} // namespace enums
#endif
//]

