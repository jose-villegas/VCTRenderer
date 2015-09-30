//  File doc/quickbook/oglplus/quickref/enums/patch_parameter_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/patch_parameter.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_patch_parameter_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PatchParameter> class Transform>
class __EnumToClass<Base, __PatchParameter, Transform> /*<
Specialization of __EnumToClass for the __PatchParameter enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PatchParameter::PatchVertices>
		PatchVertices;
	Transform<PatchParameter::PatchDefaultOuterLevel>
		PatchDefaultOuterLevel;
	Transform<PatchParameter::PatchDefaultInnerLevel>
		PatchDefaultInnerLevel;
};

} // namespace enums
#endif
//]

