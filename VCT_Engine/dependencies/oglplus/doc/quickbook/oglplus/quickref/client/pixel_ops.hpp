/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_PixelOps
namespace client {

using oglplus::__context_PixelOps;

} // namespace client
//]

//[oglplus_client_PixelState
namespace client {

class PixelState
{
private:
	template <__PixelParameter Parameter>
	class PixelParams /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	 : __SettingStack<__Unspecified, ...> /*<
	Stack of numeric values (booleans, integers, floats) appropriate
	for the pixel [^Parameter] type.
	>*/
	{ };
public:
	__enums_EnumToClass_PixelParameter<
		__Nothing,
		__PixelParameter,
		PixelParams
	> PixelStore; /*<
	Set of stacks managing the setting of numeric pixel parameters
	individually for each value in the __PixelParameter enumeration.
	>*/
};

} // namespace client
//]

