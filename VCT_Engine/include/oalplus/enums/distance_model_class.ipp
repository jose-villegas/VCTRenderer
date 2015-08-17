//  File implement/oalplus/enums/distance_model_class.ipp
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
namespace enums {
template <typename Base, template<DistanceModel> class Transform>
class EnumToClass<Base, DistanceModel, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined AL_NONE
# if defined None
#  pragma push_macro("None")
#  undef None
	Transform<DistanceModel::None> None;
#  pragma pop_macro("None")
# else
	Transform<DistanceModel::None> None;
# endif
#endif
#if defined AL_INVERSE_DISTANCE
# if defined InverseDistance
#  pragma push_macro("InverseDistance")
#  undef InverseDistance
	Transform<DistanceModel::InverseDistance> InverseDistance;
#  pragma pop_macro("InverseDistance")
# else
	Transform<DistanceModel::InverseDistance> InverseDistance;
# endif
#endif
#if defined AL_INVERSE_DISTANCE_CLAMPED
# if defined InverseDistanceClamped
#  pragma push_macro("InverseDistanceClamped")
#  undef InverseDistanceClamped
	Transform<DistanceModel::InverseDistanceClamped> InverseDistanceClamped;
#  pragma pop_macro("InverseDistanceClamped")
# else
	Transform<DistanceModel::InverseDistanceClamped> InverseDistanceClamped;
# endif
#endif
#if defined AL_LINEAR_DISTANCE
# if defined LinearDistance
#  pragma push_macro("LinearDistance")
#  undef LinearDistance
	Transform<DistanceModel::LinearDistance> LinearDistance;
#  pragma pop_macro("LinearDistance")
# else
	Transform<DistanceModel::LinearDistance> LinearDistance;
# endif
#endif
#if defined AL_LINEAR_DISTANCE_CLAMPED
# if defined LinearDistanceClamped
#  pragma push_macro("LinearDistanceClamped")
#  undef LinearDistanceClamped
	Transform<DistanceModel::LinearDistanceClamped> LinearDistanceClamped;
#  pragma pop_macro("LinearDistanceClamped")
# else
	Transform<DistanceModel::LinearDistanceClamped> LinearDistanceClamped;
# endif
#endif
#if defined AL_EXPONENT_DISTANCE
# if defined ExponentDistance
#  pragma push_macro("ExponentDistance")
#  undef ExponentDistance
	Transform<DistanceModel::ExponentDistance> ExponentDistance;
#  pragma pop_macro("ExponentDistance")
# else
	Transform<DistanceModel::ExponentDistance> ExponentDistance;
# endif
#endif
#if defined AL_EXPONENT_DISTANCE_CLAMPED
# if defined ExponentDistanceClamped
#  pragma push_macro("ExponentDistanceClamped")
#  undef ExponentDistanceClamped
	Transform<DistanceModel::ExponentDistanceClamped> ExponentDistanceClamped;
#  pragma pop_macro("ExponentDistanceClamped")
# else
	Transform<DistanceModel::ExponentDistanceClamped> ExponentDistanceClamped;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined AL_NONE
# if defined None
#  pragma push_macro("None")
#  undef None
	 , None(_base())
#  pragma pop_macro("None")
# else
	 , None(_base())
# endif
#endif
#if defined AL_INVERSE_DISTANCE
# if defined InverseDistance
#  pragma push_macro("InverseDistance")
#  undef InverseDistance
	 , InverseDistance(_base())
#  pragma pop_macro("InverseDistance")
# else
	 , InverseDistance(_base())
# endif
#endif
#if defined AL_INVERSE_DISTANCE_CLAMPED
# if defined InverseDistanceClamped
#  pragma push_macro("InverseDistanceClamped")
#  undef InverseDistanceClamped
	 , InverseDistanceClamped(_base())
#  pragma pop_macro("InverseDistanceClamped")
# else
	 , InverseDistanceClamped(_base())
# endif
#endif
#if defined AL_LINEAR_DISTANCE
# if defined LinearDistance
#  pragma push_macro("LinearDistance")
#  undef LinearDistance
	 , LinearDistance(_base())
#  pragma pop_macro("LinearDistance")
# else
	 , LinearDistance(_base())
# endif
#endif
#if defined AL_LINEAR_DISTANCE_CLAMPED
# if defined LinearDistanceClamped
#  pragma push_macro("LinearDistanceClamped")
#  undef LinearDistanceClamped
	 , LinearDistanceClamped(_base())
#  pragma pop_macro("LinearDistanceClamped")
# else
	 , LinearDistanceClamped(_base())
# endif
#endif
#if defined AL_EXPONENT_DISTANCE
# if defined ExponentDistance
#  pragma push_macro("ExponentDistance")
#  undef ExponentDistance
	 , ExponentDistance(_base())
#  pragma pop_macro("ExponentDistance")
# else
	 , ExponentDistance(_base())
# endif
#endif
#if defined AL_EXPONENT_DISTANCE_CLAMPED
# if defined ExponentDistanceClamped
#  pragma push_macro("ExponentDistanceClamped")
#  undef ExponentDistanceClamped
	 , ExponentDistanceClamped(_base())
#  pragma pop_macro("ExponentDistanceClamped")
# else
	 , ExponentDistanceClamped(_base())
# endif
#endif
	{ }
};

} // namespace enums

