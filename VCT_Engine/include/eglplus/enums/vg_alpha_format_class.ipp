//  File implement/eglplus/enums/vg_alpha_format_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/vg_alpha_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<VGAlphaFormat> class Transform>
class EnumToClass<Base, VGAlphaFormat, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_VG_ALPHA_FORMAT_PRE
# if defined Pre
#  pragma push_macro("Pre")
#  undef Pre
	Transform<VGAlphaFormat::Pre> Pre;
#  pragma pop_macro("Pre")
# else
	Transform<VGAlphaFormat::Pre> Pre;
# endif
#endif
#if defined EGL_VG_ALPHA_FORMAT_NONPRE
# if defined NonPre
#  pragma push_macro("NonPre")
#  undef NonPre
	Transform<VGAlphaFormat::NonPre> NonPre;
#  pragma pop_macro("NonPre")
# else
	Transform<VGAlphaFormat::NonPre> NonPre;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_VG_ALPHA_FORMAT_PRE
# if defined Pre
#  pragma push_macro("Pre")
#  undef Pre
	 , Pre(_base())
#  pragma pop_macro("Pre")
# else
	 , Pre(_base())
# endif
#endif
#if defined EGL_VG_ALPHA_FORMAT_NONPRE
# if defined NonPre
#  pragma push_macro("NonPre")
#  undef NonPre
	 , NonPre(_base())
#  pragma pop_macro("NonPre")
# else
	 , NonPre(_base())
# endif
#endif
	{ }
};

} // namespace enums

