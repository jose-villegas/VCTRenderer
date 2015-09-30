/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_attrib_list

template <typename AttribKind, class Traits>
class FinishedAttributeList
{
public:
	FinishedAttributeList(const FinishedAttributeList& that);
	FinishedAttributeList(FinishedAttributeList&& tmp);
};

template <typename AttribKind, class ValueToAttribMap, class Traits>
class AttributeList
{
public:
	AttributeList(void); /*<
	Creates an empty list of attributes/
	>*/

	AttributeList& Add(AttribKind attrib, __Unspecified value); /*<
	Adds a new attribute/value pair.
	This list must not be finished.
	>*/

	template <typename AttribValueType>
	AttributeList& Add(AttribValueType value); /*<
	Adds a new enumerated attribute value.
	This list must not be finished.
	>*/

	template <typename AttribValueType>
	AttributeList& Add(__Bitfield<AttribValueType> bits); /*<
	Adds a new bitfield attribute value.
	This list must not be finished.
	>*/

	AttributeList& DontCare(AttribKind attrib); /*<
	Sets the attribute value to don't care.
	This list must not be finished.
	>*/

	bool Finished(void) const; /*<
	Returns true if the list of attributes has been finished.
	>*/

	AttributeList& Finish(void); /*<
	Finishes this list of attributes. After doing that new attribute
	values cannot be added.
	>*/

	__FinishedAttributeList<AttribKind, Traits> Get(void) const; /*<
	Returns a finished attribute list.
	[note This function does not call the [^Finish] function
	on this attribute list, it must be called manually before
	calling [^Get]].
	>*/
};

//]

