/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_DeferredHandler

class DeferredHandler
{
public:
	DeferredHandler(DeferredHandler&&);

	~DeferredHandler(void); /*<
	Invokes the error handler (which usually throws an exception)
	unless the handler already has been triggered or it was cancelled.
	>*/

	void trigger(void); /*<
	If [^this] instance wraps an error handler, it is invoked
	>*/
	bool cancel(void) /*<
	If [^this] instance wraps an error handler, this function
	cancels its invocation and returns [^true], otherwise returns [^false].
	>*/
	noexcept;

	explicit
	operator bool (void) const /*<
	Returns true if [^this] instance wraps an error handler.
	>*/
	noexcept;

	bool operator ! (void) const /*<
	Returns true if [^this] instance does not wrap an error handler.
	>*/
	noexcept;
};

//]

