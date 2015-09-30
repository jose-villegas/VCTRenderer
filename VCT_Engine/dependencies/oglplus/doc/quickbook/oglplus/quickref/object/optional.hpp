/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_Optional

template <typename __Object>
class Optional
 : public __Object
{
public:
	Optional(void) noexcept; /*<
	Construction of an uninitialized instance.
	The only things that can safely be done with
	an uninitialized [^Optional<__Object>] is assignment,
	moving, destruction and checking whether it is
	initialized.
	>*/
	Optional(Optional&& temp) noexcept; /*<
	Optionals are move-constructible.
	>*/
	Optional(__Object&& temp) noexcept; /*<
	Construction of an initialized instance.
	Initialized [^Optional<__Object>] can be used everywhere where
	a plain [^__Object] could be used, furthermore [^Optional<__Object>]
	can also be [^Release]d (this brings it in uninitialized state).
	>*/
	Optional& operator = (__Object&& temp); /*<
	Optionals are move-assignable.
	>*/

	explicit operator bool (void) const; /*<
	Equivalent to [^HasValidName].
	>*/
	bool HasValidName(void) const noexcept; /*<
	Returns true if the object has a valid name, false otherwise.
	The only things that can safely be done with
	an invalid name [^Optional<__Object>] is assignment,
	moving, destruction and checking whether it is
	initialized. On the other hand initialized
	[^Optional<__Object>] can be used everywhere where
	a plain [^__Object] could be used.
	>*/

	__Object Release(void) noexcept; /*<
	Releases and returns the stored object and makes this
	[^Optional] uninitialized.
	>*/
};

//]

