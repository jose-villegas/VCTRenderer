//  File doc/quickbook/oalplus/quickref/enums/distance_model.hpp
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
//[oalplus_enums_distance_model

enum class DistanceModel : ALenum
{
	None                    = AL_NONE,
	InverseDistance         = AL_INVERSE_DISTANCE,
	InverseDistanceClamped  = AL_INVERSE_DISTANCE_CLAMPED,
	LinearDistance          = AL_LINEAR_DISTANCE,
	LinearDistanceClamped   = AL_LINEAR_DISTANCE_CLAMPED,
	ExponentDistance        = AL_EXPONENT_DISTANCE,
	ExponentDistanceClamped = AL_EXPONENT_DISTANCE_CLAMPED
};

template <>
__Range<DistanceModel> __EnumValueRange<DistanceModel>(void);

__StrCRef __EnumValueName(DistanceModel);

//]
