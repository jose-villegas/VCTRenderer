//  File doc/quickbook/oglplus/quickref/enums/graphics_reset_status_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/graphics_reset_status.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_graphics_reset_status_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__GraphicsResetStatus> class Transform>
class __EnumToClass<Base, __GraphicsResetStatus, Transform> /*<
Specialization of __EnumToClass for the __GraphicsResetStatus enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<GraphicsResetStatus::NoError>
		NoError;
	Transform<GraphicsResetStatus::GuiltyContextReset>
		GuiltyContextReset;
	Transform<GraphicsResetStatus::InnocentContextReset>
		InnocentContextReset;
	Transform<GraphicsResetStatus::UnknownContextReset>
		UnknownContextReset;
};

} // namespace enums
#endif
//]

