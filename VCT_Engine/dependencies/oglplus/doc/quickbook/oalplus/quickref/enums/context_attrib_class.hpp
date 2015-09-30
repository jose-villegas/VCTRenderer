//  File doc/quickbook/oalplus/quickref/enums/context_attrib_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/context_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_context_attrib_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ContextAttrib> class Transform>
class __EnumToClass<Base, __ContextAttrib, Transform> /*<
Specialization of __EnumToClass for the __ContextAttrib enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ContextAttrib::Frequency>
		Frequency;
	Transform<ContextAttrib::Refresh>
		Refresh;
	Transform<ContextAttrib::Sync>
		Sync;
	Transform<ContextAttrib::MonoSources>
		MonoSources;
	Transform<ContextAttrib::StereoSources>
		StereoSources;
};

} // namespace enums
#endif
//]

