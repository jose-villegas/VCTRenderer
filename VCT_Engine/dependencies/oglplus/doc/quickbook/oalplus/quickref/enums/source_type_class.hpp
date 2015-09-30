//  File doc/quickbook/oalplus/quickref/enums/source_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/source_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_source_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__SourceType> class Transform>
class __EnumToClass<Base, __SourceType, Transform> /*<
Specialization of __EnumToClass for the __SourceType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<SourceType::Undetermined>
		Undetermined;
	Transform<SourceType::Static>
		Static;
	Transform<SourceType::Streaming>
		Streaming;
};

} // namespace enums
#endif
//]

