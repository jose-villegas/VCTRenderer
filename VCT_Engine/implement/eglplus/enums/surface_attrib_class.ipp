//  File implement/eglplus/enums/surface_attrib_class.ipp
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
namespace enums {
template <typename Base, template<SurfaceAttrib> class Transform>
class EnumToClass<Base, SurfaceAttrib, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_WIDTH
# if defined Width
#  pragma push_macro("Width")
#  undef Width
	Transform<SurfaceAttrib::Width> Width;
#  pragma pop_macro("Width")
# else
	Transform<SurfaceAttrib::Width> Width;
# endif
#endif
#if defined EGL_HEIGHT
# if defined Height
#  pragma push_macro("Height")
#  undef Height
	Transform<SurfaceAttrib::Height> Height;
#  pragma pop_macro("Height")
# else
	Transform<SurfaceAttrib::Height> Height;
# endif
#endif
#if defined EGL_CONFIG_ID
# if defined ConfigId
#  pragma push_macro("ConfigId")
#  undef ConfigId
	Transform<SurfaceAttrib::ConfigId> ConfigId;
#  pragma pop_macro("ConfigId")
# else
	Transform<SurfaceAttrib::ConfigId> ConfigId;
# endif
#endif
#if defined EGL_VG_ALPHA_FORMAT
# if defined VGAlphaFormat
#  pragma push_macro("VGAlphaFormat")
#  undef VGAlphaFormat
	Transform<SurfaceAttrib::VGAlphaFormat> VGAlphaFormat;
#  pragma pop_macro("VGAlphaFormat")
# else
	Transform<SurfaceAttrib::VGAlphaFormat> VGAlphaFormat;
# endif
#endif
#if defined EGL_VG_COLORSPACE
# if defined VGColorspace
#  pragma push_macro("VGColorspace")
#  undef VGColorspace
	Transform<SurfaceAttrib::VGColorspace> VGColorspace;
#  pragma pop_macro("VGColorspace")
# else
	Transform<SurfaceAttrib::VGColorspace> VGColorspace;
# endif
#endif
#if defined EGL_GL_COLORSPACE
# if defined GLColorspace
#  pragma push_macro("GLColorspace")
#  undef GLColorspace
	Transform<SurfaceAttrib::GLColorspace> GLColorspace;
#  pragma pop_macro("GLColorspace")
# else
	Transform<SurfaceAttrib::GLColorspace> GLColorspace;
# endif
#endif
#if defined EGL_HORIZONTAL_RESOLUTION
# if defined HorizontalResolution
#  pragma push_macro("HorizontalResolution")
#  undef HorizontalResolution
	Transform<SurfaceAttrib::HorizontalResolution> HorizontalResolution;
#  pragma pop_macro("HorizontalResolution")
# else
	Transform<SurfaceAttrib::HorizontalResolution> HorizontalResolution;
# endif
#endif
#if defined EGL_VERTICAL_RESOLUTION
# if defined VerticalResolution
#  pragma push_macro("VerticalResolution")
#  undef VerticalResolution
	Transform<SurfaceAttrib::VerticalResolution> VerticalResolution;
#  pragma pop_macro("VerticalResolution")
# else
	Transform<SurfaceAttrib::VerticalResolution> VerticalResolution;
# endif
#endif
#if defined EGL_LARGEST_PBUFFER
# if defined LargestPbuffer
#  pragma push_macro("LargestPbuffer")
#  undef LargestPbuffer
	Transform<SurfaceAttrib::LargestPbuffer> LargestPbuffer;
#  pragma pop_macro("LargestPbuffer")
# else
	Transform<SurfaceAttrib::LargestPbuffer> LargestPbuffer;
# endif
#endif
#if defined EGL_MIPMAP_TEXTURE
# if defined MipmapTexture
#  pragma push_macro("MipmapTexture")
#  undef MipmapTexture
	Transform<SurfaceAttrib::MipmapTexture> MipmapTexture;
#  pragma pop_macro("MipmapTexture")
# else
	Transform<SurfaceAttrib::MipmapTexture> MipmapTexture;
# endif
#endif
#if defined EGL_MIPMAP_LEVEL
# if defined MipmapLevel
#  pragma push_macro("MipmapLevel")
#  undef MipmapLevel
	Transform<SurfaceAttrib::MipmapLevel> MipmapLevel;
#  pragma pop_macro("MipmapLevel")
# else
	Transform<SurfaceAttrib::MipmapLevel> MipmapLevel;
# endif
#endif
#if defined EGL_MULTISAMPLE_RESOLVE
# if defined MultisampleResolve
#  pragma push_macro("MultisampleResolve")
#  undef MultisampleResolve
	Transform<SurfaceAttrib::MultisampleResolve> MultisampleResolve;
#  pragma pop_macro("MultisampleResolve")
# else
	Transform<SurfaceAttrib::MultisampleResolve> MultisampleResolve;
# endif
#endif
#if defined EGL_PIXEL_ASPECT_RATIO
# if defined PixelAspectRatio
#  pragma push_macro("PixelAspectRatio")
#  undef PixelAspectRatio
	Transform<SurfaceAttrib::PixelAspectRatio> PixelAspectRatio;
#  pragma pop_macro("PixelAspectRatio")
# else
	Transform<SurfaceAttrib::PixelAspectRatio> PixelAspectRatio;
# endif
#endif
#if defined EGL_RENDER_BUFFER
# if defined RenderBuffer
#  pragma push_macro("RenderBuffer")
#  undef RenderBuffer
	Transform<SurfaceAttrib::RenderBuffer> RenderBuffer;
#  pragma pop_macro("RenderBuffer")
# else
	Transform<SurfaceAttrib::RenderBuffer> RenderBuffer;
# endif
#endif
#if defined EGL_SWAP_BEHAVIOR
# if defined SwapBehavior
#  pragma push_macro("SwapBehavior")
#  undef SwapBehavior
	Transform<SurfaceAttrib::SwapBehavior> SwapBehavior;
#  pragma pop_macro("SwapBehavior")
# else
	Transform<SurfaceAttrib::SwapBehavior> SwapBehavior;
# endif
#endif
#if defined EGL_TEXTURE_FORMAT
# if defined TextureFormat
#  pragma push_macro("TextureFormat")
#  undef TextureFormat
	Transform<SurfaceAttrib::TextureFormat> TextureFormat;
#  pragma pop_macro("TextureFormat")
# else
	Transform<SurfaceAttrib::TextureFormat> TextureFormat;
# endif
#endif
#if defined EGL_TEXTURE_TARGET
# if defined TextureTarget
#  pragma push_macro("TextureTarget")
#  undef TextureTarget
	Transform<SurfaceAttrib::TextureTarget> TextureTarget;
#  pragma pop_macro("TextureTarget")
# else
	Transform<SurfaceAttrib::TextureTarget> TextureTarget;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_WIDTH
# if defined Width
#  pragma push_macro("Width")
#  undef Width
	 , Width(_base())
#  pragma pop_macro("Width")
# else
	 , Width(_base())
# endif
#endif
#if defined EGL_HEIGHT
# if defined Height
#  pragma push_macro("Height")
#  undef Height
	 , Height(_base())
#  pragma pop_macro("Height")
# else
	 , Height(_base())
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
#if defined EGL_VG_ALPHA_FORMAT
# if defined VGAlphaFormat
#  pragma push_macro("VGAlphaFormat")
#  undef VGAlphaFormat
	 , VGAlphaFormat(_base())
#  pragma pop_macro("VGAlphaFormat")
# else
	 , VGAlphaFormat(_base())
# endif
#endif
#if defined EGL_VG_COLORSPACE
# if defined VGColorspace
#  pragma push_macro("VGColorspace")
#  undef VGColorspace
	 , VGColorspace(_base())
#  pragma pop_macro("VGColorspace")
# else
	 , VGColorspace(_base())
# endif
#endif
#if defined EGL_GL_COLORSPACE
# if defined GLColorspace
#  pragma push_macro("GLColorspace")
#  undef GLColorspace
	 , GLColorspace(_base())
#  pragma pop_macro("GLColorspace")
# else
	 , GLColorspace(_base())
# endif
#endif
#if defined EGL_HORIZONTAL_RESOLUTION
# if defined HorizontalResolution
#  pragma push_macro("HorizontalResolution")
#  undef HorizontalResolution
	 , HorizontalResolution(_base())
#  pragma pop_macro("HorizontalResolution")
# else
	 , HorizontalResolution(_base())
# endif
#endif
#if defined EGL_VERTICAL_RESOLUTION
# if defined VerticalResolution
#  pragma push_macro("VerticalResolution")
#  undef VerticalResolution
	 , VerticalResolution(_base())
#  pragma pop_macro("VerticalResolution")
# else
	 , VerticalResolution(_base())
# endif
#endif
#if defined EGL_LARGEST_PBUFFER
# if defined LargestPbuffer
#  pragma push_macro("LargestPbuffer")
#  undef LargestPbuffer
	 , LargestPbuffer(_base())
#  pragma pop_macro("LargestPbuffer")
# else
	 , LargestPbuffer(_base())
# endif
#endif
#if defined EGL_MIPMAP_TEXTURE
# if defined MipmapTexture
#  pragma push_macro("MipmapTexture")
#  undef MipmapTexture
	 , MipmapTexture(_base())
#  pragma pop_macro("MipmapTexture")
# else
	 , MipmapTexture(_base())
# endif
#endif
#if defined EGL_MIPMAP_LEVEL
# if defined MipmapLevel
#  pragma push_macro("MipmapLevel")
#  undef MipmapLevel
	 , MipmapLevel(_base())
#  pragma pop_macro("MipmapLevel")
# else
	 , MipmapLevel(_base())
# endif
#endif
#if defined EGL_MULTISAMPLE_RESOLVE
# if defined MultisampleResolve
#  pragma push_macro("MultisampleResolve")
#  undef MultisampleResolve
	 , MultisampleResolve(_base())
#  pragma pop_macro("MultisampleResolve")
# else
	 , MultisampleResolve(_base())
# endif
#endif
#if defined EGL_PIXEL_ASPECT_RATIO
# if defined PixelAspectRatio
#  pragma push_macro("PixelAspectRatio")
#  undef PixelAspectRatio
	 , PixelAspectRatio(_base())
#  pragma pop_macro("PixelAspectRatio")
# else
	 , PixelAspectRatio(_base())
# endif
#endif
#if defined EGL_RENDER_BUFFER
# if defined RenderBuffer
#  pragma push_macro("RenderBuffer")
#  undef RenderBuffer
	 , RenderBuffer(_base())
#  pragma pop_macro("RenderBuffer")
# else
	 , RenderBuffer(_base())
# endif
#endif
#if defined EGL_SWAP_BEHAVIOR
# if defined SwapBehavior
#  pragma push_macro("SwapBehavior")
#  undef SwapBehavior
	 , SwapBehavior(_base())
#  pragma pop_macro("SwapBehavior")
# else
	 , SwapBehavior(_base())
# endif
#endif
#if defined EGL_TEXTURE_FORMAT
# if defined TextureFormat
#  pragma push_macro("TextureFormat")
#  undef TextureFormat
	 , TextureFormat(_base())
#  pragma pop_macro("TextureFormat")
# else
	 , TextureFormat(_base())
# endif
#endif
#if defined EGL_TEXTURE_TARGET
# if defined TextureTarget
#  pragma push_macro("TextureTarget")
#  undef TextureTarget
	 , TextureTarget(_base())
#  pragma pop_macro("TextureTarget")
# else
	 , TextureTarget(_base())
# endif
#endif
	{ }
};

} // namespace enums

