//  File doc/quickbook/oglplus/quickref/enums/data_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/data_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_data_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DataType> class Transform>
class __EnumToClass<Base, __DataType, Transform> /*<
Specialization of __EnumToClass for the __DataType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DataType::Byte>
		Byte;
	Transform<DataType::Short>
		Short;
	Transform<DataType::Int>
		Int;
	Transform<DataType::Fixed>
		Fixed;
	Transform<DataType::Float>
		Float;
	Transform<DataType::HalfFloat>
		HalfFloat;
	Transform<DataType::Double>
		Double;
	Transform<DataType::UnsignedByte>
		UnsignedByte;
	Transform<DataType::UnsignedShort>
		UnsignedShort;
	Transform<DataType::UnsignedInt>
		UnsignedInt;
};

} // namespace enums
#endif
//]

