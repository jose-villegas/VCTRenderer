//  File doc/quickbook/oalplus/quickref/enums/data_format_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/data_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_data_format_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DataFormat> class Transform>
class __EnumToClass<Base, __DataFormat, Transform> /*<
Specialization of __EnumToClass for the __DataFormat enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DataFormat::Mono8>
		Mono8;
	Transform<DataFormat::Mono16>
		Mono16;
	Transform<DataFormat::Stereo8>
		Stereo8;
	Transform<DataFormat::Stereo16>
		Stereo16;
};

} // namespace enums
#endif
//]

