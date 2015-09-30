/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_Outcome

template <typename T>
class Outcome
{
private:
	__DeferredHandler handler; /*<
	The error handler is invoked in the destructor unless
	it is dismissed.
	>*/
public:
	Outcome(Outcome&&);

	T Then(void); /*<
	If the operation which produced [^this] instance of [^Outcome]
	generated an error, then its handler is invoked, otherwise this
	function returns the resulting value of that operation.
	>*/

	bool Done(void) const; /*<
	Returns [^true] if the operation which produced [^this] instance
	completed without error, returns [^false] otherwise. The error
	handler [*is not] dismissed and will be invoked later.
	>*/

	bool DoneWithoutError(void); /*<
	If the operation that produced [^this] instance ended with an error
	then the error handler is dismissed (cancelled) and will not be
	invoked and [^false] is returned, otherwise this function returns
	[^true].
	>*/
};

//]
//[oglplus_error_PositiveOutcome

template <typename T>
class PositiveOutcome
 : public __Outcome<T>
{
public:
	explicit
	operator bool (void) const /*<
	Returns [^true] if the operation that produced [^this] instance
	completed without an error, returns [^false] otherwise.
	>*/
	noexcept;

	bool operator ! (void) const /*<
	Returns [^false] if the operation that produced [^this] instance
	completed without an error, returns [^true] otherwise.
	>*/
	noexcept;
};

template <typename T>
__PositiveOutcome<T> Succeeded(__Outcome<T>&& outcome) /*<
Converts __Outcome to __PositiveOutcome.
>*/
noexcept;

//]
//[oglplus_error_NegativeOutcome

template <typename T>
class NegativeOutcome
 : public __Outcome<T>
{
public:
	explicit
	operator bool (void) const /*<
	Returns [^false] if the operation that produced [^this] instance
	completed without an error, returns [^true] otherwise.
	>*/
	noexcept;

	bool operator ! (void) const /*<
	Returns [^true] if the operation that produced [^this] instance
	completed without an error, returns [^false] otherwise.
	>*/
	noexcept;
};

template <typename T>
__NegativeOutcome<T> Failed(__Outcome<T>&& outcome) /*<
Converts __Outcome to __NegativeOutcome.
>*/
noexcept;

//]

