//  File doc/quickbook/oglplus/quickref/enums/texture_target_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_target_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TextureTarget> class Transform>
class __EnumToClass<Base, __TextureTarget, Transform> /*<
Specialization of __EnumToClass for the __TextureTarget enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TextureTarget::_1D>
		_1D;
	Transform<TextureTarget::_2D>
		_2D;
	Transform<TextureTarget::_3D>
		_3D;
	Transform<TextureTarget::_1DArray>
		_1DArray;
	Transform<TextureTarget::_2DArray>
		_2DArray;
	Transform<TextureTarget::Rectangle>
		Rectangle;
	Transform<TextureTarget::Buffer>
		Buffer;
	Transform<TextureTarget::CubeMap>
		CubeMap;
	Transform<TextureTarget::CubeMapArray>
		CubeMapArray;
	Transform<TextureTarget::_2DMultisample>
		_2DMultisample;
	Transform<TextureTarget::_2DMultisampleArray>
		_2DMultisampleArray;
	Transform<TextureTarget::CubeMapPositiveX>
		CubeMapPositiveX;
	Transform<TextureTarget::CubeMapNegativeX>
		CubeMapNegativeX;
	Transform<TextureTarget::CubeMapPositiveY>
		CubeMapPositiveY;
	Transform<TextureTarget::CubeMapNegativeY>
		CubeMapNegativeY;
	Transform<TextureTarget::CubeMapPositiveZ>
		CubeMapPositiveZ;
	Transform<TextureTarget::CubeMapNegativeZ>
		CubeMapNegativeZ;
};

} // namespace enums
#endif
//]

