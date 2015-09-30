//  File doc/quickbook/oglplus/quickref/enums/ext/compat_client_attrib_group_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/compat_client_attrib_group.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_compat_client_attrib_group_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__CompatibilityClientAttributeGroup> class Transform>
class __EnumToClass<Base, __CompatibilityClientAttributeGroup, Transform> /*<
Specialization of __EnumToClass for the __CompatibilityClientAttributeGroup enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<CompatibilityClientAttributeGroup::VertexArray>
		VertexArray;
	Transform<CompatibilityClientAttributeGroup::PixelStore>
		PixelStore;
	Transform<CompatibilityClientAttributeGroup::AllAttribs>
		AllAttribs;
};

} // namespace enums
#endif
//]

