//  File doc/quickbook/eglplus/quickref/enums/surface_attrib_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/surface_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_surface_attrib_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__SurfaceAttrib> class Transform>
class __EnumToClass<Base, __SurfaceAttrib, Transform> /*<
Specialization of __EnumToClass for the __SurfaceAttrib enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<SurfaceAttrib::Width>
		Width;
	Transform<SurfaceAttrib::Height>
		Height;
	Transform<SurfaceAttrib::ConfigId>
		ConfigId;
	Transform<SurfaceAttrib::VGAlphaFormat>
		VGAlphaFormat;
	Transform<SurfaceAttrib::VGColorspace>
		VGColorspace;
	Transform<SurfaceAttrib::GLColorspace>
		GLColorspace;
	Transform<SurfaceAttrib::HorizontalResolution>
		HorizontalResolution;
	Transform<SurfaceAttrib::VerticalResolution>
		VerticalResolution;
	Transform<SurfaceAttrib::LargestPbuffer>
		LargestPbuffer;
	Transform<SurfaceAttrib::MipmapTexture>
		MipmapTexture;
	Transform<SurfaceAttrib::MipmapLevel>
		MipmapLevel;
	Transform<SurfaceAttrib::MultisampleResolve>
		MultisampleResolve;
	Transform<SurfaceAttrib::PixelAspectRatio>
		PixelAspectRatio;
	Transform<SurfaceAttrib::RenderBuffer>
		RenderBuffer;
	Transform<SurfaceAttrib::SwapBehavior>
		SwapBehavior;
	Transform<SurfaceAttrib::TextureFormat>
		TextureFormat;
	Transform<SurfaceAttrib::TextureTarget>
		TextureTarget;
};

} // namespace enums
#endif
//]

