//  File doc/quickbook/oglplus/quickref/enums/stencil_operation_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/stencil_operation.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_stencil_operation_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__StencilOperation> class Transform>
class __EnumToClass<Base, __StencilOperation, Transform> /*<
Specialization of __EnumToClass for the __StencilOperation enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<StencilOperation::Keep>
		Keep;
	Transform<StencilOperation::Zero>
		Zero;
	Transform<StencilOperation::Replace>
		Replace;
	Transform<StencilOperation::Incr>
		Incr;
	Transform<StencilOperation::Decr>
		Decr;
	Transform<StencilOperation::Invert>
		Invert;
	Transform<StencilOperation::IncrWrap>
		IncrWrap;
	Transform<StencilOperation::DecrWrap>
		DecrWrap;
};

} // namespace enums
#endif
//]

