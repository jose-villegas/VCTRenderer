/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oalplus_listener_helpers

struct ListenerOrientation
{
	ListenerOrientation(const __Vec3f& at, const __Vec3f& up)
	noexcept;

	__Vec3f At(void) const
	noexcept;

	__Vec3f Up(void) const
	noexcept;
};

//]
//[oalplus_listener_common_1

class Listener
{
public:
	static void Position(const __Vec3f& position); /*<
	Specifies the position of the listener.
	See [alfunc Listenerfv], [alconst POSITION].
	>*/
	static void Position(ALfloat x, ALfloat y, ALfloat z);
	static __Vec3f Position(void); /*<
	Returns the position of the listener.
	See [alfunc GetListenerfv], [alconst POSITION].
	>*/

	static void Velocity(const __Vec3f& velocity); /*<
	Specifies the velocity of the listener.
	See [alfunc Listenerfv], [alconst VELOCITY].
	>*/
	static void Velocity(ALfloat x, ALfloat y, ALfloat z);
	static __Vec3f Velocity(void); /*<
	Returns the velocity of the listener.
	See [alfunc GetListenerfv], [alconst VELOCITY].
	>*/

	static void Orientation(const __ListenerOrientation& orientation); /*<
	Specifies the orientation vectors of the listener.
	See [alfunc Listenerfv], [alconst ORIENTATION].
	>*/
	static void Orientation(const __Vec3f& at, const __Vec3f& up);
	static void Orientation(
		ALfloat at_x,
		ALfloat at_y,
		ALfloat at_z,
		ALfloat up_x,
		ALfloat up_y,
		ALfloat up_z
	);

	static __ListenerOrientation Orientation(void); /*<
	Return the orientation vectors of the listener.
	See [alfunc GetListenerfv], [alconst ORIENTATION].
	>*/
	static Vec3f OrientationAt(void);
	static Vec3f OrientationUp(void);

	static void Gain(ALfloat value); /*<
	Specifies the value of gain of the listener.
	See [alfunc Listenerf], [alconst GAIN].
	>*/
	static ALfloat Gain(void); /*<
	Returns the current value of gain of the listener.
	See [alfunc GetListenerfv], [alconst GAIN].
	>*/
};

//]

