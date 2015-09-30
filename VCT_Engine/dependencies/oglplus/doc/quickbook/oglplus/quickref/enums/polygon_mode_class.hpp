//  File doc/quickbook/oglplus/quickref/enums/polygon_mode_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/polygon_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_polygon_mode_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PolygonMode> class Transform>
class __EnumToClass<Base, __PolygonMode, Transform> /*<
Specialization of __EnumToClass for the __PolygonMode enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PolygonMode::Point>
		Point;
	Transform<PolygonMode::Line>
		Line;
	Transform<PolygonMode::Fill>
		Fill;
	Transform<PolygonMode::FillRectangle>
		FillRectangle;
};

} // namespace enums
#endif
//]

