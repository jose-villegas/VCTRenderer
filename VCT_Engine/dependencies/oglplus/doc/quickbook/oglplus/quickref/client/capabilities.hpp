/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_CapabilityState
namespace client {

class CapabilityState
{
private:
	template <__Capability Value>
	class CurrentCapabilityIndexed /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	 : __SettingStack<__Boolean, ...>
	{
	public:
		__Boolean IsEnabled(void) const /*<
		Checks if the capability is currently enabled ([^true])
		or disabled ([^false]).
		>*/
		noexcept;

		explicit
		operator __Boolean (void) const /*<
		Checks if the capability is currently enabled ([^true])
		or disabled ([^false]).
		>*/
		noexcept;

		void Enable(__Boolean status = true); /*<
		Enables (by default) or sets the capability
		to the specified [^status].
		>*/

		void Disable(void); /*<
		Disables the capability.
		>*/
	};

	template <__Capability Value>
	class CurrentCapability /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	 : __SettingStackIndexed<CurrentCapabilityIndexed<Value>, __Boolean>
	{
	public:
		__Boolean IsEnabled(void) const
		noexcept;

		explicit
		operator __Boolean (void) const
		noexcept;

		void Enable(__Boolean status = true);

		void Disable(void);
	};
public:

	__enums_EnumToClass_Capability<
		__Nothing,
		__Capability,
		CurrentCapability
	> Caps; /*<
	Stacks managing the state (enabled or disabled) individually for each
	capability in the __Capability enumeration.
	>*/
};

} // namespace client
//]

