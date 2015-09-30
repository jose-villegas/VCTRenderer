//  File doc/quickbook/oglplus/quickref/enums/clip_depth_mode_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/clip_depth_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_clip_depth_mode_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ClipDepthMode> class Transform>
class __EnumToClass<Base, __ClipDepthMode, Transform> /*<
Specialization of __EnumToClass for the __ClipDepthMode enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ClipDepthMode::NegativeOneToOne>
		NegativeOneToOne;
	Transform<ClipDepthMode::ZeroToOne>
		ZeroToOne;
};

} // namespace enums
#endif
//]

