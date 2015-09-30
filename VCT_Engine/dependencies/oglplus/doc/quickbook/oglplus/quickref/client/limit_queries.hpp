/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_LimitQueries
namespace client {

class LimitQueries
{
private:
	template <__LimitQuery Limit>
	class LimitGetter /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	{
	public:
		typedef	__Unspecified value_type; /*<
		The type of the value associated with the [^Limit].
		>*/

		static value_type Get(void); /*<
		Returns the value of the [^Limit].
		>*/

		static value_type Get(GLuint index); /*<
		Returns the value of the [^Limit] if this particular
		__LimitQuery is indexed.
		>*/

		operator value_type(void) const; /*<
		Returns the value of the [^Limit].
		>*/
	};
public:
	__enums_EnumToClass_LimitQuery<
		__Nothing,
		__LimitQuery,
		LimitGetter
	> Limits; /*<
	Set of classes with static member functions which allow to get
	the values of various limits.
	>*/
};

} // namespace client
//]

