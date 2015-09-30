//  File doc/quickbook/oglplus/quickref/enums/precision_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/precision_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_precision_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PrecisionType> class Transform>
class __EnumToClass<Base, __PrecisionType, Transform> /*<
Specialization of __EnumToClass for the __PrecisionType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PrecisionType::LowFloat>
		LowFloat;
	Transform<PrecisionType::MediumFloat>
		MediumFloat;
	Transform<PrecisionType::HighFloat>
		HighFloat;
	Transform<PrecisionType::LowInt>
		LowInt;
	Transform<PrecisionType::MediumInt>
		MediumInt;
	Transform<PrecisionType::HighInt>
		HighInt;
};

} // namespace enums
#endif
//]

