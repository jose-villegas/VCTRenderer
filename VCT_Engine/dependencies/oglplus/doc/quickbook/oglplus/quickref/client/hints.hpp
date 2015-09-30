/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_HintState
namespace client {

class HintState
{
private:
	template <__HintTarget Value>
	class Hint /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	 : __SettingStack<__HintOption, ...>
	{ };
public:

	__enums_EnumToClass_HintTarget<
		__Nothing,
		__HintTarget,
		Hint
	> Hints; /*<
	Set of stacks managing the setting of __HintOption settings
	individually for each value in the __HintTarget enumeration.
	>*/
};

} // namespace client
//]

