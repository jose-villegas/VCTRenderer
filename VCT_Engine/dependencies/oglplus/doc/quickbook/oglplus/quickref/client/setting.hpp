/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_SettingHolder
namespace client {

template <typename T, ...>
class SettingHolder
{
public:
	SettingHolder(const SettingHolder&) = delete;

	SettingHolder(SettingHolder&&)
	noexcept;

	~SettingHolder(void) /*<
	Implicitly pops the value which this holder keeps from the setting stack
	which created [^this] instance of [^Holder].
	>*/
	noexcept;

	void Pop(void) /*<
	Explicitly pops the value which this holder keeps from the setting stack
	which created [^this] instance of [^Holder].
	This function can be only called if the value has not been popped yet.
	>*/
	noexcept;

	explicit
	operator bool (void) const /*<
	Returns [^true] if the setting has not been popped yet,
	returns [^false] otherwise.
	>*/
	noexcept;
};

} // namespace client
//]
//[oglplus_client_SettingStack
namespace client {

template <typename T, ...>
class SettingStack
{
public:
	SettingStack(const SettingStack&) = delete;

	SettingStack(void); /*<
	Queries the current value of the setting that this stack manages
	and pushes it on itself as the first element.
	>*/

	typedef __SettingHolder<T, ...> Holder;

	SettingStack& operator [] (Index index);
	SettingStack& Indexed(Index index); /*<
	Returns a reference to itself if [^index] is zero, otherwise the behavior
	is undefined.
	This function and the indexing operator are here to make the interface
	of [^SettingStack] compatible with __SettingStackIndexed.
	>*/

	Holder Push(T value); /*<
	Pushes a new value on top of the setting stack. When the returned
	instance of [^Holder] gets destroyed (or its [^Pop] member function is
	called explicitly) then this value is popped from the stack.
	Storing the returned instance keeps the value on the stack.
	Care must be taken to destroy the instances of [^Holder] in the reverse
	of the order in which they were created. This can be easily achieved
	by storing the holders in local variables.
	>*/

	template <typename ... A>
	Holder Push(A&& ... a); /*<
	This overload of [^Push] constructs a new instance of [^T] from
	the specified arguments and pushes it on the stack.
	>*/

	T Get(void) const /*<
	Returns the value of the setting on the top of this setting stack.
	>*/
	noexcept;

	void Set(T value); /*<
	Changes the current setting value without pushing on the stack.
	>*/

	template <typename ... A>
	void Set(A&& ... a); /*<
	This overload of [^Set] changes the value by constructing a new instance
	of [^T] from the specified arguments.
	>*/
};

} // namespace client
//]
//[oglplus_client_SettingStackIndexed
namespace client {

template <typename Derived, typename T, typename Index = GLuint>
class SettingStackIndexed
{
public:
	SettingStackIndexed(const SettingStackIndexed&) = delete;

	SettingStackIndexed(void); /*<
	Initialized the __SettingStack on index zero.
	>*/

	Derived& operator [] (Index index);
	Derived& Indexed(Index index); /*<
	Returns a reference to the setting stack at the specified index.
	The value of index should not exceed the maximum value for the managed
	indexed setting.
	>*/

	typedef __SettingHolder<T, GLuint> Holder;

	Holder Push(T value); /*<
	Pushes a new value on the stack at index zero.
	>*/

	template <typename ... A>
	Holder Push(A&& ... a);

	T Get(void) const /*<
	Gets the value from the stack at index zero.
	>*/
	noexcept;

	void Set(T value); /*<
	Sets a new value on the stack at index zero.
	>*/

	template <typename ... A>
	void Set(A&& ... a);
};

} // namespace client
//]

