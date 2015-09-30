//  File doc/quickbook/oglplus/quickref/enums/texture_min_filter_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_min_filter.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_min_filter_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TextureMinFilter> class Transform>
class __EnumToClass<Base, __TextureMinFilter, Transform> /*<
Specialization of __EnumToClass for the __TextureMinFilter enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TextureMinFilter::Nearest>
		Nearest;
	Transform<TextureMinFilter::Linear>
		Linear;
	Transform<TextureMinFilter::NearestMipmapNearest>
		NearestMipmapNearest;
	Transform<TextureMinFilter::NearestMipmapLinear>
		NearestMipmapLinear;
	Transform<TextureMinFilter::LinearMipmapNearest>
		LinearMipmapNearest;
	Transform<TextureMinFilter::LinearMipmapLinear>
		LinearMipmapLinear;
};

} // namespace enums
#endif
//]

