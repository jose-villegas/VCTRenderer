/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_ObjectBinding
namespace context {

class ObjectBinding
{
public:
	template <typename Object>
	static
	void Bind(typename Object::Target target, const Object& object); /*<
	Binds the specified [^object] to the specified [^target].
	Equivalent to [^object.Bind(target)].
	>*/

	template <typename Object>
	static
	void Bind(
		typename Object::IndexedTarget target,
		GLuint index,
		const Object& object
	); /*<
	Binds the specified [^object] to the specified indexed [^target].
	Equivalent to [^object.Bind(target, index)].
	>*/

	template <typename Object>
	static
	void Bind(const Object& object); /*<
	Binds the specified object to the appropriate binding point.
	Equivalent to [^object.Bind()].
	>*/

	template <typename Object>
	static
	void Use(const Object& object); /*<
	Uses (makes current) the specified [^object] (for example a __Program).
	Equivalent to [^object.Use()].
	>*/

	template <typename __Object>
	static __Reference_ObjectOps_Bound<__ObjectOps_Bound<
		__tag_CurrentBound,
		__ObjTag
	>> Current(void); /*<
	Returns the currently bound (active) instance of the specified
	[^Object] type.
	>*/

	template <typename ObjectTarget>
	static __Reference_ObjectOps_Bound<__ObjectOps_Bound<
		__tag_CurrentBound,
		__ObjTag
	>> Current(ObjectTarget target); /*<
	Returns the object currently bound to the specified object [^target].
	>*/

	template <typename __Object>
	static __Reference_ObjectOps_Bound<__ObjectOps_Bound<
		__tag_CurrentBound,
		__ObjTag
	>> Bound(typename __Object::Target target, const __Object& object); /*<
	Binds an [^object] to a binding point specified by [^target] and returns
	a reference to that object.
	>*/
};

} // namespace context
//]

