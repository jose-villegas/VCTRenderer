//  File doc/quickbook/oalplus/quickref/enums/distance_model_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/distance_model.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_distance_model_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DistanceModel> class Transform>
class __EnumToClass<Base, __DistanceModel, Transform> /*<
Specialization of __EnumToClass for the __DistanceModel enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DistanceModel::None>
		None;
	Transform<DistanceModel::InverseDistance>
		InverseDistance;
	Transform<DistanceModel::InverseDistanceClamped>
		InverseDistanceClamped;
	Transform<DistanceModel::LinearDistance>
		LinearDistance;
	Transform<DistanceModel::LinearDistanceClamped>
		LinearDistanceClamped;
	Transform<DistanceModel::ExponentDistance>
		ExponentDistance;
	Transform<DistanceModel::ExponentDistanceClamped>
		ExponentDistanceClamped;
};

} // namespace enums
#endif
//]

