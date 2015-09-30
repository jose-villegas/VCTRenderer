//  File doc/quickbook/eglplus/quickref/enums/multisample_resolve_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/multisample_resolve.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_multisample_resolve_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__MultisampleResolve> class Transform>
class __EnumToClass<Base, __MultisampleResolve, Transform> /*<
Specialization of __EnumToClass for the __MultisampleResolve enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<MultisampleResolve::Default>
		Default;
	Transform<MultisampleResolve::Box>
		Box;
};

} // namespace enums
#endif
//]

