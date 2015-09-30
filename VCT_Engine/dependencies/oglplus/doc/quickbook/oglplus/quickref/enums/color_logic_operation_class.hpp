//  File doc/quickbook/oglplus/quickref/enums/color_logic_operation_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/color_logic_operation.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_color_logic_operation_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ColorLogicOperation> class Transform>
class __EnumToClass<Base, __ColorLogicOperation, Transform> /*<
Specialization of __EnumToClass for the __ColorLogicOperation enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ColorLogicOperation::Clear>
		Clear;
	Transform<ColorLogicOperation::And>
		And;
	Transform<ColorLogicOperation::AndReverse>
		AndReverse;
	Transform<ColorLogicOperation::Copy>
		Copy;
	Transform<ColorLogicOperation::AndInverted>
		AndInverted;
	Transform<ColorLogicOperation::Noop>
		Noop;
	Transform<ColorLogicOperation::Xor>
		Xor;
	Transform<ColorLogicOperation::Or>
		Or;
	Transform<ColorLogicOperation::Nor>
		Nor;
	Transform<ColorLogicOperation::Equiv>
		Equiv;
	Transform<ColorLogicOperation::Invert>
		Invert;
	Transform<ColorLogicOperation::OrReverse>
		OrReverse;
	Transform<ColorLogicOperation::CopyInverted>
		CopyInverted;
	Transform<ColorLogicOperation::OrInverted>
		OrInverted;
	Transform<ColorLogicOperation::Nand>
		Nand;
	Transform<ColorLogicOperation::Set>
		Set;
};

} // namespace enums
#endif
//]

