//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_dash_offset_reset_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_dash_offset_reset.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_dash_offset_reset_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PathNVDashOffsetReset> class Transform>
class __EnumToClass<Base, __PathNVDashOffsetReset, Transform> /*<
Specialization of __EnumToClass for the __PathNVDashOffsetReset enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PathNVDashOffsetReset::MoveToReset>
		MoveToReset;
	Transform<PathNVDashOffsetReset::MoveToContinues>
		MoveToContinues;
};

} // namespace enums
#endif
//]

