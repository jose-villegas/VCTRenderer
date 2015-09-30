/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_enums_functions

template <typename Enumeration>
__Range<Enumeration> EnumValueRange(void); /*<
Returns a __Range of values in a OGLplus enum value.
This template function is overloaded for the enumerated types defined by
OGLplus and returns a __Range which allows to traverse all values of
a particular [^Enumeration] type.
The result of this function is influenced by the __OGLPLUS_NO_ENUM_VALUE_RANGES
preprocessor-symbol. If it is set to a nonzero value then [^EnumValueRange<Enum>()]
returns an empty range.
>*/

template <typename Enumeration>
__StrCRef EnumValueName(Enumeration enum_value); /*<
Returns the name of the GL enumerated value for an OGLplus enum value.
This function is overloaded for the enumerated types defined by OGLplus
and returns the GL constant name (without the "GL_" prefix) as managed
const string reference.
The result of this function is influenced by the __OGLPLUS_NO_ENUM_VALUE_NAMES
preprocessor-symbol. If it is set to a nonzero value then
[^EnumValueName(enum_value)] returns an empty string.
>*/
//]
//[oglplus_enums_EnumArray

template <typename Enum>
class EnumArray
{
public:
	template <std::size_t N>
	EnumArray(const Enum (&enums)[N]);
	EnumArray(const std::vector<Enum>& enums);
	EnumArray(size_t count, const Enum* enums);
};
//]

//[oglplus_enums_OneOf
template <typename ... Enum>
class OneOf
{
public:
	OneOf(Enum)...; /*<
	(['Pseudocode]) - [^OneOf] has a constructor for each enumeration
	type passed in the template argument pack.
	>*/
};

//]
//[oglplus_enums_EnumToClass
namespace enums {

template <
	typename Base,
	typename Enumeration,
	template <Enumeration> class Transform
> class EnumToClass
{
public:
	EnumToClass(void);
	EnumToClass(const EnumToClass&);

	/* ... */
};

} // namespace enums
//]

