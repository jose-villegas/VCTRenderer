//  File doc/quickbook/eglplus/quickref/enums/config_attrib_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/config_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_config_attrib_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ConfigAttrib> class Transform>
class __EnumToClass<Base, __ConfigAttrib, Transform> /*<
Specialization of __EnumToClass for the __ConfigAttrib enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ConfigAttrib::BufferSize>
		BufferSize;
	Transform<ConfigAttrib::RedSize>
		RedSize;
	Transform<ConfigAttrib::GreenSize>
		GreenSize;
	Transform<ConfigAttrib::BlueSize>
		BlueSize;
	Transform<ConfigAttrib::LuminanceSize>
		LuminanceSize;
	Transform<ConfigAttrib::AlphaSize>
		AlphaSize;
	Transform<ConfigAttrib::AlphaMaskSize>
		AlphaMaskSize;
	Transform<ConfigAttrib::BindToTextureRGB>
		BindToTextureRGB;
	Transform<ConfigAttrib::BindToTextureRGBA>
		BindToTextureRGBA;
	Transform<ConfigAttrib::ColorBufferType>
		ColorBufferType;
	Transform<ConfigAttrib::ConfigCaveat>
		ConfigCaveat;
	Transform<ConfigAttrib::ConfigId>
		ConfigId;
	Transform<ConfigAttrib::Conformant>
		Conformant;
	Transform<ConfigAttrib::DepthSize>
		DepthSize;
	Transform<ConfigAttrib::Level>
		Level;
	Transform<ConfigAttrib::MaxPbufferWidth>
		MaxPbufferWidth;
	Transform<ConfigAttrib::MaxPbufferHeight>
		MaxPbufferHeight;
	Transform<ConfigAttrib::MaxPbufferPixels>
		MaxPbufferPixels;
	Transform<ConfigAttrib::MaxSwapInterval>
		MaxSwapInterval;
	Transform<ConfigAttrib::MinSwapInterval>
		MinSwapInterval;
	Transform<ConfigAttrib::NativeRenderable>
		NativeRenderable;
	Transform<ConfigAttrib::NativeVisualId>
		NativeVisualId;
	Transform<ConfigAttrib::NativeVisualType>
		NativeVisualType;
	Transform<ConfigAttrib::RenderableType>
		RenderableType;
	Transform<ConfigAttrib::SampleBuffers>
		SampleBuffers;
	Transform<ConfigAttrib::Samples>
		Samples;
	Transform<ConfigAttrib::StencilSize>
		StencilSize;
	Transform<ConfigAttrib::SurfaceType>
		SurfaceType;
	Transform<ConfigAttrib::TransparentType>
		TransparentType;
	Transform<ConfigAttrib::TransparentRedValue>
		TransparentRedValue;
	Transform<ConfigAttrib::TransparentGreenValue>
		TransparentGreenValue;
	Transform<ConfigAttrib::TransparentBlueValue>
		TransparentBlueValue;
};

} // namespace enums
#endif
//]

