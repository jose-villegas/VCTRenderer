//  File implement/eglplus/enums/string_query_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/string_query.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<StringQuery> class Transform>
class EnumToClass<Base, StringQuery, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_CLIENT_APIS
# if defined ClientAPIs
#  pragma push_macro("ClientAPIs")
#  undef ClientAPIs
	Transform<StringQuery::ClientAPIs> ClientAPIs;
#  pragma pop_macro("ClientAPIs")
# else
	Transform<StringQuery::ClientAPIs> ClientAPIs;
# endif
#endif
#if defined EGL_EXTENSIONS
# if defined Extensions
#  pragma push_macro("Extensions")
#  undef Extensions
	Transform<StringQuery::Extensions> Extensions;
#  pragma pop_macro("Extensions")
# else
	Transform<StringQuery::Extensions> Extensions;
# endif
#endif
#if defined EGL_VENDOR
# if defined Vendor
#  pragma push_macro("Vendor")
#  undef Vendor
	Transform<StringQuery::Vendor> Vendor;
#  pragma pop_macro("Vendor")
# else
	Transform<StringQuery::Vendor> Vendor;
# endif
#endif
#if defined EGL_VERSION
# if defined Version
#  pragma push_macro("Version")
#  undef Version
	Transform<StringQuery::Version> Version;
#  pragma pop_macro("Version")
# else
	Transform<StringQuery::Version> Version;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_CLIENT_APIS
# if defined ClientAPIs
#  pragma push_macro("ClientAPIs")
#  undef ClientAPIs
	 , ClientAPIs(_base())
#  pragma pop_macro("ClientAPIs")
# else
	 , ClientAPIs(_base())
# endif
#endif
#if defined EGL_EXTENSIONS
# if defined Extensions
#  pragma push_macro("Extensions")
#  undef Extensions
	 , Extensions(_base())
#  pragma pop_macro("Extensions")
# else
	 , Extensions(_base())
# endif
#endif
#if defined EGL_VENDOR
# if defined Vendor
#  pragma push_macro("Vendor")
#  undef Vendor
	 , Vendor(_base())
#  pragma pop_macro("Vendor")
# else
	 , Vendor(_base())
# endif
#endif
#if defined EGL_VERSION
# if defined Version
#  pragma push_macro("Version")
#  undef Version
	 , Version(_base())
#  pragma pop_macro("Version")
# else
	 , Version(_base())
# endif
#endif
	{ }
};

} // namespace enums

