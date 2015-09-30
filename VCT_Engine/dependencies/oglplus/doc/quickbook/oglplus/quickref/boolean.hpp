/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_bool_impl

template <typename B, typename T, T TrueValue, T FalseValue>
struct WeakBoolImpl
{
public:
	operator bool (void) const
	noexcept; /*<
	Returns [^true] if the wrapped value is not equal to [^FalseValue].
	>*/

	bool operator ! (void) const;
	noexcept; /*<
	Returns [^true] if the wrapped value is not equal to [^TrueValue].
	>*/
};

template <typename B, typename T, T TrueValue, T FalseValue>
struct BoolImpl
{
public:
	BoolImpl(void)
	noexcept; /*<
	Creates a false boolean value.
	>*/

	BoolImpl(bool bool_value)
	noexcept;

	BoolImpl(T value, std::nothrow_t);
	noexcept; /*<
	Constructs a boolean from a value in the underlying type.
	This version does not throw if the [^value] is neither [^TrueValue]
	nor [^FalseValue], in such case the constructed instance
	is ['indeterminate].
	>*/

	explicit
	BoolImpl(T value); /*<
	Constructs a boolean from a value in the underlying type.
	This constructor throws a [^std::domain_error] if [^value] is
	neither [^TrueValue] not [^FalseValue].
	>*/

	operator bool (void) const
	noexcept; /*<
	Returns [^true] if the wrapped value is equal to [^TrueValue].
	>*/

	bool operator ! (void) const;
	noexcept; /*<
	Returns [^true] if the wrapped value is equal to [^FalseValue].
	>*/

	WeakBoolImpl<T, TrueValue, FalseValue>
	operator ~ (void) const
	noexcept;
};

//]
//[oglplus_boolean

typedef __BoolImpl<GLboolean, GLint, GL_TRUE, GL_FALSE> Boolean;

//]

