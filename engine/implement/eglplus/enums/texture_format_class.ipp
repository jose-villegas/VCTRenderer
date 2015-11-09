//  File implement/eglplus/enums/texture_format_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/texture_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<TextureFormat> class Transform>
class EnumToClass<Base, TextureFormat, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_TEXTURE_RGB
# if defined RGB
#  pragma push_macro("RGB")
#  undef RGB
	Transform<TextureFormat::RGB> RGB;
#  pragma pop_macro("RGB")
# else
	Transform<TextureFormat::RGB> RGB;
# endif
#endif
#if defined EGL_TEXTURE_RGBA
# if defined RGBA
#  pragma push_macro("RGBA")
#  undef RGBA
	Transform<TextureFormat::RGBA> RGBA;
#  pragma pop_macro("RGBA")
# else
	Transform<TextureFormat::RGBA> RGBA;
# endif
#endif
#if defined EGL_NO_TEXTURE
# if defined NoTexture
#  pragma push_macro("NoTexture")
#  undef NoTexture
	Transform<TextureFormat::NoTexture> NoTexture;
#  pragma pop_macro("NoTexture")
# else
	Transform<TextureFormat::NoTexture> NoTexture;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_TEXTURE_RGB
# if defined RGB
#  pragma push_macro("RGB")
#  undef RGB
	 , RGB(_base())
#  pragma pop_macro("RGB")
# else
	 , RGB(_base())
# endif
#endif
#if defined EGL_TEXTURE_RGBA
# if defined RGBA
#  pragma push_macro("RGBA")
#  undef RGBA
	 , RGBA(_base())
#  pragma pop_macro("RGBA")
# else
	 , RGBA(_base())
# endif
#endif
#if defined EGL_NO_TEXTURE
# if defined NoTexture
#  pragma push_macro("NoTexture")
#  undef NoTexture
	 , NoTexture(_base())
#  pragma pop_macro("NoTexture")
# else
	 , NoTexture(_base())
# endif
#endif
	{ }
};

} // namespace enums

