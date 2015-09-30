//  File doc/quickbook/oalplus/quickref/enums/source_state_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/source_state.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_source_state_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__SourceState> class Transform>
class __EnumToClass<Base, __SourceState, Transform> /*<
Specialization of __EnumToClass for the __SourceState enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<SourceState::Initial>
		Initial;
	Transform<SourceState::Playing>
		Playing;
	Transform<SourceState::Paused>
		Paused;
	Transform<SourceState::Stopped>
		Stopped;
};

} // namespace enums
#endif
//]

