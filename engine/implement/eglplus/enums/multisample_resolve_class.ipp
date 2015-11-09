//  File implement/eglplus/enums/multisample_resolve_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/multisample_resolve.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<MultisampleResolve> class Transform>
class EnumToClass<Base, MultisampleResolve, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_MULTISAMPLE_RESOLVE_DEFAULT
# if defined Default
#  pragma push_macro("Default")
#  undef Default
	Transform<MultisampleResolve::Default> Default;
#  pragma pop_macro("Default")
# else
	Transform<MultisampleResolve::Default> Default;
# endif
#endif
#if defined EGL_MULTISAMPLE_RESOLVE_BOX
# if defined Box
#  pragma push_macro("Box")
#  undef Box
	Transform<MultisampleResolve::Box> Box;
#  pragma pop_macro("Box")
# else
	Transform<MultisampleResolve::Box> Box;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_MULTISAMPLE_RESOLVE_DEFAULT
# if defined Default
#  pragma push_macro("Default")
#  undef Default
	 , Default(_base())
#  pragma pop_macro("Default")
# else
	 , Default(_base())
# endif
#endif
#if defined EGL_MULTISAMPLE_RESOLVE_BOX
# if defined Box
#  pragma push_macro("Box")
#  undef Box
	 , Box(_base())
#  pragma pop_macro("Box")
# else
	 , Box(_base())
# endif
#endif
	{ }
};

} // namespace enums

