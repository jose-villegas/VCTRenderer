//  File doc/quickbook/oglplus/quickref/enums/access_specifier_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/access_specifier.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_access_specifier_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__AccessSpecifier> class Transform>
class __EnumToClass<Base, __AccessSpecifier, Transform> /*<
Specialization of __EnumToClass for the __AccessSpecifier enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<AccessSpecifier::ReadOnly>
		ReadOnly;
	Transform<AccessSpecifier::WriteOnly>
		WriteOnly;
	Transform<AccessSpecifier::ReadWrite>
		ReadWrite;
};

} // namespace enums
#endif
//]

