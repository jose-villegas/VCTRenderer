/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_Array

template <typename Object>
class Array;
{
public:
	Array(const Array&) = delete;
	Array(Array&&);

	Array(std::size_t count); /*<
	Constructs an an array of [^count] instances of Object.
	>*/

	Array(std::size_t n, typename __ObjectSubtype<__ObjTag>::Type type); /*<
	Constructs an an array of [^n] instances of Object with
	the specified [^type]
	>*/

	~Array(void);

	bool empty(void) const; /*<
	Returns true if the array is empty.
	>*/

	std::size_t size(void) const; /*<
	Returns the number of instances in the array.
	>*/

	typedef __Reference<__Object> reference;
	typedef const reference const_reference;

	reference operator[](std::size_t index); /*<
	Returns a reference to the i-th instance in the array.
	>*/

	const_reference operator[](std::size_t index) const; /*<
	Returns a const reference to the i-th instance in the array.
	>*/

	typedef __SeqIterator<__Object> iterator;
	typedef iterator const_iterator;

	const_iterator begin(void) const; /*<
	Returns an iterator pointing to the first element.
	>*/
	const_iterator end(void) const; /*<
	Returns an iterator pointing past the last element.
	>*/

	__Sequence<__ObjectName<__ObjTag>> seq(void) const;

	operator __Sequence<__ObjectName<__ObjTag>> (void) const; /*<
	Implicit conversion to a sequence of object names, which can
	be passed as an argument to various functions.
	Note that the returned sequence must not be used after
	this array has been destroyed.
	>*/
};

//]

