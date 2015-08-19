//  File implement/eglplus/enums/transparent_type_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/transparent_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<TransparentType> class Transform>
class EnumToClass<Base, TransparentType, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_NONE
# if defined None
#  pragma push_macro("None")
#  undef None
	Transform<TransparentType::None> None;
#  pragma pop_macro("None")
# else
	Transform<TransparentType::None> None;
# endif
#endif
#if defined EGL_TRANSPARENT_RGB
# if defined TransparentRGB
#  pragma push_macro("TransparentRGB")
#  undef TransparentRGB
	Transform<TransparentType::TransparentRGB> TransparentRGB;
#  pragma pop_macro("TransparentRGB")
# else
	Transform<TransparentType::TransparentRGB> TransparentRGB;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_NONE
# if defined None
#  pragma push_macro("None")
#  undef None
	 , None(_base())
#  pragma pop_macro("None")
# else
	 , None(_base())
# endif
#endif
#if defined EGL_TRANSPARENT_RGB
# if defined TransparentRGB
#  pragma push_macro("TransparentRGB")
#  undef TransparentRGB
	 , TransparentRGB(_base())
#  pragma pop_macro("TransparentRGB")
# else
	 , TransparentRGB(_base())
# endif
#endif
	{ }
};

} // namespace enums

