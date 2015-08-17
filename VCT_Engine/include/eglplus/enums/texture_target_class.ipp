//  File implement/eglplus/enums/texture_target_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/texture_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<TextureTarget> class Transform>
class EnumToClass<Base, TextureTarget, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_NO_TEXTURE
# if defined NoTexture
#  pragma push_macro("NoTexture")
#  undef NoTexture
	Transform<TextureTarget::NoTexture> NoTexture;
#  pragma pop_macro("NoTexture")
# else
	Transform<TextureTarget::NoTexture> NoTexture;
# endif
#endif
#if defined EGL_TEXTURE_2D
# if defined Texture2D
#  pragma push_macro("Texture2D")
#  undef Texture2D
	Transform<TextureTarget::Texture2D> Texture2D;
#  pragma pop_macro("Texture2D")
# else
	Transform<TextureTarget::Texture2D> Texture2D;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
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
#if defined EGL_TEXTURE_2D
# if defined Texture2D
#  pragma push_macro("Texture2D")
#  undef Texture2D
	 , Texture2D(_base())
#  pragma pop_macro("Texture2D")
# else
	 , Texture2D(_base())
# endif
#endif
	{ }
};

} // namespace enums

