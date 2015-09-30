//  File doc/quickbook/eglplus/quickref/enums/config_caveat_class.hpp
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
//[eglplus_enums_config_caveat_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ConfigCaveat> class Transform>
class __EnumToClass<Base, __ConfigCaveat, Transform> /*<
Specialization of __EnumToClass for the __ConfigCaveat enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ConfigCaveat::None>
		None;
	Transform<ConfigCaveat::SlowConfig>
		SlowConfig;
	Transform<ConfigCaveat::NonConformantConfig>
		NonConformantConfig;
};

} // namespace enums
#endif
//]

