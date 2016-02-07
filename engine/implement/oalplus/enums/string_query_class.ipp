//  File implement/oalplus/enums/string_query_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/string_query.txt'
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

#if defined AL_VERSION
# if defined Version
#  pragma push_macro("Version")
#  undef Version
	Transform<StringQuery::Version> Version;
#  pragma pop_macro("Version")
# else
	Transform<StringQuery::Version> Version;
# endif
#endif
#if defined AL_RENDERER
# if defined Renderer
#  pragma push_macro("Renderer")
#  undef Renderer
	Transform<StringQuery::Renderer> Renderer;
#  pragma pop_macro("Renderer")
# else
	Transform<StringQuery::Renderer> Renderer;
# endif
#endif
#if defined AL_VENDOR
# if defined Vendor
#  pragma push_macro("Vendor")
#  undef Vendor
	Transform<StringQuery::Vendor> Vendor;
#  pragma pop_macro("Vendor")
# else
	Transform<StringQuery::Vendor> Vendor;
# endif
#endif
#if defined AL_EXTENSIONS
# if defined Extensions
#  pragma push_macro("Extensions")
#  undef Extensions
	Transform<StringQuery::Extensions> Extensions;
#  pragma pop_macro("Extensions")
# else
	Transform<StringQuery::Extensions> Extensions;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined AL_VERSION
# if defined Version
#  pragma push_macro("Version")
#  undef Version
	 , Version(_base())
#  pragma pop_macro("Version")
# else
	 , Version(_base())
# endif
#endif
#if defined AL_RENDERER
# if defined Renderer
#  pragma push_macro("Renderer")
#  undef Renderer
	 , Renderer(_base())
#  pragma pop_macro("Renderer")
# else
	 , Renderer(_base())
# endif
#endif
#if defined AL_VENDOR
# if defined Vendor
#  pragma push_macro("Vendor")
#  undef Vendor
	 , Vendor(_base())
#  pragma pop_macro("Vendor")
# else
	 , Vendor(_base())
# endif
#endif
#if defined AL_EXTENSIONS
# if defined Extensions
#  pragma push_macro("Extensions")
#  undef Extensions
	 , Extensions(_base())
#  pragma pop_macro("Extensions")
# else
	 , Extensions(_base())
# endif
#endif
	{ }
};

} // namespace enums

