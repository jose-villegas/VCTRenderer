/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_enums_functions_example_1

__Context gl;

__Bitfield<__ContextProfileBit> cpm = gl.ProfileMask(); /*<
Query the profile mask for the current GL context.
>*/

for(auto r=__EnumValueRange<__ContextProfileBit>(); !r.Empty(); r.Next()) /*<
Get a range enumerating all individual bits in the __ContextProfileBit enumeration.
>*/
{
	auto bit = r.Front(); /*<
	For each individual bit in the enumeration.
	>*/

	std::cout << __EnumValueName(bit).c_str() << ": "; /*<
	Prints the bit's name.
	>*/

	std::cout << (cpm.Has(bit)?"true":"false") << std::endl; /*<
	Print whether the bit is set in the mask.
	>*/
}
//]
//[oglplus_enums_OneOf_example_1
typedef __OneOf<
	__FramebufferAttachment,
	__FramebufferColorAttachment
> Attachment;

Attachment a1 = __FramebufferAttachment::Depth;
Attachment a2 = __FramebufferColorAttachment::_0;
//]
//[oglplus_enums_EnumToClass_ex1

enum class SomeEnum
{
	ValueA,
	ValueB,
	ValueC,
	ValueD
};

//]
//[oglplus_enums_EnumToClass_ex2

template <SomeEnum Value>
class SomeEnumTransform
{
public:
	SomeEnumTransform(void); /*<
	One of these constructors must be implemented.
	>*/
	SomeEnumTransform(Base&);
};

//]
//[oglplus_enums_EnumToClass_ex3

typedef __EnumToClass<Base, SomeEnum, SomeEnumTransform> EnumToClassSomeEnumTransform;

//]
//[oglplus_enums_EnumToClass_ex4

class EnumToClassSomeEnumTransform
 : public Base
{
public:
	SomeEnumTransform<SomeEnum::ValueA> ValueA;
	SomeEnumTransform<SomeEnum::ValueB> ValueB;
	SomeEnumTransform<SomeEnum::ValueC> ValueC;
	SomeEnumTransform<SomeEnum::ValueD> ValueD;
};

//]

