//  File implement/eglplus/enums/config_attrib_class.ipp
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
namespace enums {
template <typename Base, template<ConfigAttrib> class Transform>
class EnumToClass<Base, ConfigAttrib, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_BUFFER_SIZE
# if defined BufferSize
#  pragma push_macro("BufferSize")
#  undef BufferSize
	Transform<ConfigAttrib::BufferSize> BufferSize;
#  pragma pop_macro("BufferSize")
# else
	Transform<ConfigAttrib::BufferSize> BufferSize;
# endif
#endif
#if defined EGL_RED_SIZE
# if defined RedSize
#  pragma push_macro("RedSize")
#  undef RedSize
	Transform<ConfigAttrib::RedSize> RedSize;
#  pragma pop_macro("RedSize")
# else
	Transform<ConfigAttrib::RedSize> RedSize;
# endif
#endif
#if defined EGL_GREEN_SIZE
# if defined GreenSize
#  pragma push_macro("GreenSize")
#  undef GreenSize
	Transform<ConfigAttrib::GreenSize> GreenSize;
#  pragma pop_macro("GreenSize")
# else
	Transform<ConfigAttrib::GreenSize> GreenSize;
# endif
#endif
#if defined EGL_BLUE_SIZE
# if defined BlueSize
#  pragma push_macro("BlueSize")
#  undef BlueSize
	Transform<ConfigAttrib::BlueSize> BlueSize;
#  pragma pop_macro("BlueSize")
# else
	Transform<ConfigAttrib::BlueSize> BlueSize;
# endif
#endif
#if defined EGL_LUMINANCE_SIZE
# if defined LuminanceSize
#  pragma push_macro("LuminanceSize")
#  undef LuminanceSize
	Transform<ConfigAttrib::LuminanceSize> LuminanceSize;
#  pragma pop_macro("LuminanceSize")
# else
	Transform<ConfigAttrib::LuminanceSize> LuminanceSize;
# endif
#endif
#if defined EGL_ALPHA_SIZE
# if defined AlphaSize
#  pragma push_macro("AlphaSize")
#  undef AlphaSize
	Transform<ConfigAttrib::AlphaSize> AlphaSize;
#  pragma pop_macro("AlphaSize")
# else
	Transform<ConfigAttrib::AlphaSize> AlphaSize;
# endif
#endif
#if defined EGL_ALPHA_MASK_SIZE
# if defined AlphaMaskSize
#  pragma push_macro("AlphaMaskSize")
#  undef AlphaMaskSize
	Transform<ConfigAttrib::AlphaMaskSize> AlphaMaskSize;
#  pragma pop_macro("AlphaMaskSize")
# else
	Transform<ConfigAttrib::AlphaMaskSize> AlphaMaskSize;
# endif
#endif
#if defined EGL_BIND_TO_TEXTURE_RGB
# if defined BindToTextureRGB
#  pragma push_macro("BindToTextureRGB")
#  undef BindToTextureRGB
	Transform<ConfigAttrib::BindToTextureRGB> BindToTextureRGB;
#  pragma pop_macro("BindToTextureRGB")
# else
	Transform<ConfigAttrib::BindToTextureRGB> BindToTextureRGB;
# endif
#endif
#if defined EGL_BIND_TO_TEXTURE_RGBA
# if defined BindToTextureRGBA
#  pragma push_macro("BindToTextureRGBA")
#  undef BindToTextureRGBA
	Transform<ConfigAttrib::BindToTextureRGBA> BindToTextureRGBA;
#  pragma pop_macro("BindToTextureRGBA")
# else
	Transform<ConfigAttrib::BindToTextureRGBA> BindToTextureRGBA;
# endif
#endif
#if defined EGL_COLOR_BUFFER_TYPE
# if defined ColorBufferType
#  pragma push_macro("ColorBufferType")
#  undef ColorBufferType
	Transform<ConfigAttrib::ColorBufferType> ColorBufferType;
#  pragma pop_macro("ColorBufferType")
# else
	Transform<ConfigAttrib::ColorBufferType> ColorBufferType;
# endif
#endif
#if defined EGL_CONFIG_CAVEAT
# if defined ConfigCaveat
#  pragma push_macro("ConfigCaveat")
#  undef ConfigCaveat
	Transform<ConfigAttrib::ConfigCaveat> ConfigCaveat;
#  pragma pop_macro("ConfigCaveat")
# else
	Transform<ConfigAttrib::ConfigCaveat> ConfigCaveat;
# endif
#endif
#if defined EGL_CONFIG_ID
# if defined ConfigId
#  pragma push_macro("ConfigId")
#  undef ConfigId
	Transform<ConfigAttrib::ConfigId> ConfigId;
#  pragma pop_macro("ConfigId")
# else
	Transform<ConfigAttrib::ConfigId> ConfigId;
# endif
#endif
#if defined EGL_CONFORMANT
# if defined Conformant
#  pragma push_macro("Conformant")
#  undef Conformant
	Transform<ConfigAttrib::Conformant> Conformant;
#  pragma pop_macro("Conformant")
# else
	Transform<ConfigAttrib::Conformant> Conformant;
# endif
#endif
#if defined EGL_DEPTH_SIZE
# if defined DepthSize
#  pragma push_macro("DepthSize")
#  undef DepthSize
	Transform<ConfigAttrib::DepthSize> DepthSize;
#  pragma pop_macro("DepthSize")
# else
	Transform<ConfigAttrib::DepthSize> DepthSize;
# endif
#endif
#if defined EGL_LEVEL
# if defined Level
#  pragma push_macro("Level")
#  undef Level
	Transform<ConfigAttrib::Level> Level;
#  pragma pop_macro("Level")
# else
	Transform<ConfigAttrib::Level> Level;
# endif
#endif
#if defined EGL_MAX_PBUFFER_WIDTH
# if defined MaxPbufferWidth
#  pragma push_macro("MaxPbufferWidth")
#  undef MaxPbufferWidth
	Transform<ConfigAttrib::MaxPbufferWidth> MaxPbufferWidth;
#  pragma pop_macro("MaxPbufferWidth")
# else
	Transform<ConfigAttrib::MaxPbufferWidth> MaxPbufferWidth;
# endif
#endif
#if defined EGL_MAX_PBUFFER_HEIGHT
# if defined MaxPbufferHeight
#  pragma push_macro("MaxPbufferHeight")
#  undef MaxPbufferHeight
	Transform<ConfigAttrib::MaxPbufferHeight> MaxPbufferHeight;
#  pragma pop_macro("MaxPbufferHeight")
# else
	Transform<ConfigAttrib::MaxPbufferHeight> MaxPbufferHeight;
# endif
#endif
#if defined EGL_MAX_PBUFFER_PIXELS
# if defined MaxPbufferPixels
#  pragma push_macro("MaxPbufferPixels")
#  undef MaxPbufferPixels
	Transform<ConfigAttrib::MaxPbufferPixels> MaxPbufferPixels;
#  pragma pop_macro("MaxPbufferPixels")
# else
	Transform<ConfigAttrib::MaxPbufferPixels> MaxPbufferPixels;
# endif
#endif
#if defined EGL_MAX_SWAP_INTERVAL
# if defined MaxSwapInterval
#  pragma push_macro("MaxSwapInterval")
#  undef MaxSwapInterval
	Transform<ConfigAttrib::MaxSwapInterval> MaxSwapInterval;
#  pragma pop_macro("MaxSwapInterval")
# else
	Transform<ConfigAttrib::MaxSwapInterval> MaxSwapInterval;
# endif
#endif
#if defined EGL_MIN_SWAP_INTERVAL
# if defined MinSwapInterval
#  pragma push_macro("MinSwapInterval")
#  undef MinSwapInterval
	Transform<ConfigAttrib::MinSwapInterval> MinSwapInterval;
#  pragma pop_macro("MinSwapInterval")
# else
	Transform<ConfigAttrib::MinSwapInterval> MinSwapInterval;
# endif
#endif
#if defined EGL_NATIVE_RENDERABLE
# if defined NativeRenderable
#  pragma push_macro("NativeRenderable")
#  undef NativeRenderable
	Transform<ConfigAttrib::NativeRenderable> NativeRenderable;
#  pragma pop_macro("NativeRenderable")
# else
	Transform<ConfigAttrib::NativeRenderable> NativeRenderable;
# endif
#endif
#if defined EGL_NATIVE_VISUAL_ID
# if defined NativeVisualId
#  pragma push_macro("NativeVisualId")
#  undef NativeVisualId
	Transform<ConfigAttrib::NativeVisualId> NativeVisualId;
#  pragma pop_macro("NativeVisualId")
# else
	Transform<ConfigAttrib::NativeVisualId> NativeVisualId;
# endif
#endif
#if defined EGL_NATIVE_VISUAL_TYPE
# if defined NativeVisualType
#  pragma push_macro("NativeVisualType")
#  undef NativeVisualType
	Transform<ConfigAttrib::NativeVisualType> NativeVisualType;
#  pragma pop_macro("NativeVisualType")
# else
	Transform<ConfigAttrib::NativeVisualType> NativeVisualType;
# endif
#endif
#if defined EGL_RENDERABLE_TYPE
# if defined RenderableType
#  pragma push_macro("RenderableType")
#  undef RenderableType
	Transform<ConfigAttrib::RenderableType> RenderableType;
#  pragma pop_macro("RenderableType")
# else
	Transform<ConfigAttrib::RenderableType> RenderableType;
# endif
#endif
#if defined EGL_SAMPLE_BUFFERS
# if defined SampleBuffers
#  pragma push_macro("SampleBuffers")
#  undef SampleBuffers
	Transform<ConfigAttrib::SampleBuffers> SampleBuffers;
#  pragma pop_macro("SampleBuffers")
# else
	Transform<ConfigAttrib::SampleBuffers> SampleBuffers;
# endif
#endif
#if defined EGL_SAMPLES
# if defined Samples
#  pragma push_macro("Samples")
#  undef Samples
	Transform<ConfigAttrib::Samples> Samples;
#  pragma pop_macro("Samples")
# else
	Transform<ConfigAttrib::Samples> Samples;
# endif
#endif
#if defined EGL_STENCIL_SIZE
# if defined StencilSize
#  pragma push_macro("StencilSize")
#  undef StencilSize
	Transform<ConfigAttrib::StencilSize> StencilSize;
#  pragma pop_macro("StencilSize")
# else
	Transform<ConfigAttrib::StencilSize> StencilSize;
# endif
#endif
#if defined EGL_SURFACE_TYPE
# if defined SurfaceType
#  pragma push_macro("SurfaceType")
#  undef SurfaceType
	Transform<ConfigAttrib::SurfaceType> SurfaceType;
#  pragma pop_macro("SurfaceType")
# else
	Transform<ConfigAttrib::SurfaceType> SurfaceType;
# endif
#endif
#if defined EGL_TRANSPARENT_TYPE
# if defined TransparentType
#  pragma push_macro("TransparentType")
#  undef TransparentType
	Transform<ConfigAttrib::TransparentType> TransparentType;
#  pragma pop_macro("TransparentType")
# else
	Transform<ConfigAttrib::TransparentType> TransparentType;
# endif
#endif
#if defined EGL_TRANSPARENT_RED_VALUE
# if defined TransparentRedValue
#  pragma push_macro("TransparentRedValue")
#  undef TransparentRedValue
	Transform<ConfigAttrib::TransparentRedValue> TransparentRedValue;
#  pragma pop_macro("TransparentRedValue")
# else
	Transform<ConfigAttrib::TransparentRedValue> TransparentRedValue;
# endif
#endif
#if defined EGL_TRANSPARENT_GREEN_VALUE
# if defined TransparentGreenValue
#  pragma push_macro("TransparentGreenValue")
#  undef TransparentGreenValue
	Transform<ConfigAttrib::TransparentGreenValue> TransparentGreenValue;
#  pragma pop_macro("TransparentGreenValue")
# else
	Transform<ConfigAttrib::TransparentGreenValue> TransparentGreenValue;
# endif
#endif
#if defined EGL_TRANSPARENT_BLUE_VALUE
# if defined TransparentBlueValue
#  pragma push_macro("TransparentBlueValue")
#  undef TransparentBlueValue
	Transform<ConfigAttrib::TransparentBlueValue> TransparentBlueValue;
#  pragma pop_macro("TransparentBlueValue")
# else
	Transform<ConfigAttrib::TransparentBlueValue> TransparentBlueValue;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_BUFFER_SIZE
# if defined BufferSize
#  pragma push_macro("BufferSize")
#  undef BufferSize
	 , BufferSize(_base())
#  pragma pop_macro("BufferSize")
# else
	 , BufferSize(_base())
# endif
#endif
#if defined EGL_RED_SIZE
# if defined RedSize
#  pragma push_macro("RedSize")
#  undef RedSize
	 , RedSize(_base())
#  pragma pop_macro("RedSize")
# else
	 , RedSize(_base())
# endif
#endif
#if defined EGL_GREEN_SIZE
# if defined GreenSize
#  pragma push_macro("GreenSize")
#  undef GreenSize
	 , GreenSize(_base())
#  pragma pop_macro("GreenSize")
# else
	 , GreenSize(_base())
# endif
#endif
#if defined EGL_BLUE_SIZE
# if defined BlueSize
#  pragma push_macro("BlueSize")
#  undef BlueSize
	 , BlueSize(_base())
#  pragma pop_macro("BlueSize")
# else
	 , BlueSize(_base())
# endif
#endif
#if defined EGL_LUMINANCE_SIZE
# if defined LuminanceSize
#  pragma push_macro("LuminanceSize")
#  undef LuminanceSize
	 , LuminanceSize(_base())
#  pragma pop_macro("LuminanceSize")
# else
	 , LuminanceSize(_base())
# endif
#endif
#if defined EGL_ALPHA_SIZE
# if defined AlphaSize
#  pragma push_macro("AlphaSize")
#  undef AlphaSize
	 , AlphaSize(_base())
#  pragma pop_macro("AlphaSize")
# else
	 , AlphaSize(_base())
# endif
#endif
#if defined EGL_ALPHA_MASK_SIZE
# if defined AlphaMaskSize
#  pragma push_macro("AlphaMaskSize")
#  undef AlphaMaskSize
	 , AlphaMaskSize(_base())
#  pragma pop_macro("AlphaMaskSize")
# else
	 , AlphaMaskSize(_base())
# endif
#endif
#if defined EGL_BIND_TO_TEXTURE_RGB
# if defined BindToTextureRGB
#  pragma push_macro("BindToTextureRGB")
#  undef BindToTextureRGB
	 , BindToTextureRGB(_base())
#  pragma pop_macro("BindToTextureRGB")
# else
	 , BindToTextureRGB(_base())
# endif
#endif
#if defined EGL_BIND_TO_TEXTURE_RGBA
# if defined BindToTextureRGBA
#  pragma push_macro("BindToTextureRGBA")
#  undef BindToTextureRGBA
	 , BindToTextureRGBA(_base())
#  pragma pop_macro("BindToTextureRGBA")
# else
	 , BindToTextureRGBA(_base())
# endif
#endif
#if defined EGL_COLOR_BUFFER_TYPE
# if defined ColorBufferType
#  pragma push_macro("ColorBufferType")
#  undef ColorBufferType
	 , ColorBufferType(_base())
#  pragma pop_macro("ColorBufferType")
# else
	 , ColorBufferType(_base())
# endif
#endif
#if defined EGL_CONFIG_CAVEAT
# if defined ConfigCaveat
#  pragma push_macro("ConfigCaveat")
#  undef ConfigCaveat
	 , ConfigCaveat(_base())
#  pragma pop_macro("ConfigCaveat")
# else
	 , ConfigCaveat(_base())
# endif
#endif
#if defined EGL_CONFIG_ID
# if defined ConfigId
#  pragma push_macro("ConfigId")
#  undef ConfigId
	 , ConfigId(_base())
#  pragma pop_macro("ConfigId")
# else
	 , ConfigId(_base())
# endif
#endif
#if defined EGL_CONFORMANT
# if defined Conformant
#  pragma push_macro("Conformant")
#  undef Conformant
	 , Conformant(_base())
#  pragma pop_macro("Conformant")
# else
	 , Conformant(_base())
# endif
#endif
#if defined EGL_DEPTH_SIZE
# if defined DepthSize
#  pragma push_macro("DepthSize")
#  undef DepthSize
	 , DepthSize(_base())
#  pragma pop_macro("DepthSize")
# else
	 , DepthSize(_base())
# endif
#endif
#if defined EGL_LEVEL
# if defined Level
#  pragma push_macro("Level")
#  undef Level
	 , Level(_base())
#  pragma pop_macro("Level")
# else
	 , Level(_base())
# endif
#endif
#if defined EGL_MAX_PBUFFER_WIDTH
# if defined MaxPbufferWidth
#  pragma push_macro("MaxPbufferWidth")
#  undef MaxPbufferWidth
	 , MaxPbufferWidth(_base())
#  pragma pop_macro("MaxPbufferWidth")
# else
	 , MaxPbufferWidth(_base())
# endif
#endif
#if defined EGL_MAX_PBUFFER_HEIGHT
# if defined MaxPbufferHeight
#  pragma push_macro("MaxPbufferHeight")
#  undef MaxPbufferHeight
	 , MaxPbufferHeight(_base())
#  pragma pop_macro("MaxPbufferHeight")
# else
	 , MaxPbufferHeight(_base())
# endif
#endif
#if defined EGL_MAX_PBUFFER_PIXELS
# if defined MaxPbufferPixels
#  pragma push_macro("MaxPbufferPixels")
#  undef MaxPbufferPixels
	 , MaxPbufferPixels(_base())
#  pragma pop_macro("MaxPbufferPixels")
# else
	 , MaxPbufferPixels(_base())
# endif
#endif
#if defined EGL_MAX_SWAP_INTERVAL
# if defined MaxSwapInterval
#  pragma push_macro("MaxSwapInterval")
#  undef MaxSwapInterval
	 , MaxSwapInterval(_base())
#  pragma pop_macro("MaxSwapInterval")
# else
	 , MaxSwapInterval(_base())
# endif
#endif
#if defined EGL_MIN_SWAP_INTERVAL
# if defined MinSwapInterval
#  pragma push_macro("MinSwapInterval")
#  undef MinSwapInterval
	 , MinSwapInterval(_base())
#  pragma pop_macro("MinSwapInterval")
# else
	 , MinSwapInterval(_base())
# endif
#endif
#if defined EGL_NATIVE_RENDERABLE
# if defined NativeRenderable
#  pragma push_macro("NativeRenderable")
#  undef NativeRenderable
	 , NativeRenderable(_base())
#  pragma pop_macro("NativeRenderable")
# else
	 , NativeRenderable(_base())
# endif
#endif
#if defined EGL_NATIVE_VISUAL_ID
# if defined NativeVisualId
#  pragma push_macro("NativeVisualId")
#  undef NativeVisualId
	 , NativeVisualId(_base())
#  pragma pop_macro("NativeVisualId")
# else
	 , NativeVisualId(_base())
# endif
#endif
#if defined EGL_NATIVE_VISUAL_TYPE
# if defined NativeVisualType
#  pragma push_macro("NativeVisualType")
#  undef NativeVisualType
	 , NativeVisualType(_base())
#  pragma pop_macro("NativeVisualType")
# else
	 , NativeVisualType(_base())
# endif
#endif
#if defined EGL_RENDERABLE_TYPE
# if defined RenderableType
#  pragma push_macro("RenderableType")
#  undef RenderableType
	 , RenderableType(_base())
#  pragma pop_macro("RenderableType")
# else
	 , RenderableType(_base())
# endif
#endif
#if defined EGL_SAMPLE_BUFFERS
# if defined SampleBuffers
#  pragma push_macro("SampleBuffers")
#  undef SampleBuffers
	 , SampleBuffers(_base())
#  pragma pop_macro("SampleBuffers")
# else
	 , SampleBuffers(_base())
# endif
#endif
#if defined EGL_SAMPLES
# if defined Samples
#  pragma push_macro("Samples")
#  undef Samples
	 , Samples(_base())
#  pragma pop_macro("Samples")
# else
	 , Samples(_base())
# endif
#endif
#if defined EGL_STENCIL_SIZE
# if defined StencilSize
#  pragma push_macro("StencilSize")
#  undef StencilSize
	 , StencilSize(_base())
#  pragma pop_macro("StencilSize")
# else
	 , StencilSize(_base())
# endif
#endif
#if defined EGL_SURFACE_TYPE
# if defined SurfaceType
#  pragma push_macro("SurfaceType")
#  undef SurfaceType
	 , SurfaceType(_base())
#  pragma pop_macro("SurfaceType")
# else
	 , SurfaceType(_base())
# endif
#endif
#if defined EGL_TRANSPARENT_TYPE
# if defined TransparentType
#  pragma push_macro("TransparentType")
#  undef TransparentType
	 , TransparentType(_base())
#  pragma pop_macro("TransparentType")
# else
	 , TransparentType(_base())
# endif
#endif
#if defined EGL_TRANSPARENT_RED_VALUE
# if defined TransparentRedValue
#  pragma push_macro("TransparentRedValue")
#  undef TransparentRedValue
	 , TransparentRedValue(_base())
#  pragma pop_macro("TransparentRedValue")
# else
	 , TransparentRedValue(_base())
# endif
#endif
#if defined EGL_TRANSPARENT_GREEN_VALUE
# if defined TransparentGreenValue
#  pragma push_macro("TransparentGreenValue")
#  undef TransparentGreenValue
	 , TransparentGreenValue(_base())
#  pragma pop_macro("TransparentGreenValue")
# else
	 , TransparentGreenValue(_base())
# endif
#endif
#if defined EGL_TRANSPARENT_BLUE_VALUE
# if defined TransparentBlueValue
#  pragma push_macro("TransparentBlueValue")
#  undef TransparentBlueValue
	 , TransparentBlueValue(_base())
#  pragma pop_macro("TransparentBlueValue")
# else
	 , TransparentBlueValue(_base())
# endif
#endif
	{ }
};

} // namespace enums

