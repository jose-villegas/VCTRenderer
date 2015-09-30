//  File doc/quickbook/oglplus/quickref/enums/hint_option_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/hint_option.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_hint_option_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__HintOption> class Transform>
class __EnumToClass<Base, __HintOption, Transform> /*<
Specialization of __EnumToClass for the __HintOption enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<HintOption::Fastest>
		Fastest;
	Transform<HintOption::Nicest>
		Nicest;
	Transform<HintOption::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

