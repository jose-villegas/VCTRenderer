//  File implement/eglplus/enums/config_caveat_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/config_caveat.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<ConfigCaveat> class Transform>
class EnumToClass<Base, ConfigCaveat, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_NONE
# if defined None
#  pragma push_macro("None")
#  undef None
	Transform<ConfigCaveat::None> None;
#  pragma pop_macro("None")
# else
	Transform<ConfigCaveat::None> None;
# endif
#endif
#if defined EGL_SLOW_CONFIG
# if defined SlowConfig
#  pragma push_macro("SlowConfig")
#  undef SlowConfig
	Transform<ConfigCaveat::SlowConfig> SlowConfig;
#  pragma pop_macro("SlowConfig")
# else
	Transform<ConfigCaveat::SlowConfig> SlowConfig;
# endif
#endif
#if defined EGL_NON_CONFORMANT_CONFIG
# if defined NonConformantConfig
#  pragma push_macro("NonConformantConfig")
#  undef NonConformantConfig
	Transform<ConfigCaveat::NonConformantConfig> NonConformantConfig;
#  pragma pop_macro("NonConformantConfig")
# else
	Transform<ConfigCaveat::NonConformantConfig> NonConformantConfig;
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
#if defined EGL_SLOW_CONFIG
# if defined SlowConfig
#  pragma push_macro("SlowConfig")
#  undef SlowConfig
	 , SlowConfig(_base())
#  pragma pop_macro("SlowConfig")
# else
	 , SlowConfig(_base())
# endif
#endif
#if defined EGL_NON_CONFORMANT_CONFIG
# if defined NonConformantConfig
#  pragma push_macro("NonConformantConfig")
#  undef NonConformantConfig
	 , NonConformantConfig(_base())
#  pragma pop_macro("NonConformantConfig")
# else
	 , NonConformantConfig(_base())
# endif
#endif
	{ }
};

} // namespace enums

