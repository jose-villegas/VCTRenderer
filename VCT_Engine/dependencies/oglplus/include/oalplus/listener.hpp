/**
 *  @file oalplus/listener.hpp
 *  @brief Wrapper for OpenAL listener object.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_LISTENER_1303201647_HPP
#define OALPLUS_LISTENER_1303201647_HPP

#include <oalplus/config.hpp>
#include <oalplus/fwd.hpp>
#include <oalplus/alfunc.hpp>
#include <oalplus/math/vector.hpp>
#include <oalplus/error/al.hpp>

namespace oalplus {

struct ListenerOrientation
{
	// implementation detail
	ALfloat _v[6];

	ListenerOrientation(void)
	OALPLUS_NOEXCEPT(true)
	{ }

	ListenerOrientation(const Vec3f& at, const Vec3f& up)
	OALPLUS_NOEXCEPT(true)
	{
		for(unsigned i=0; i<3; ++i)
		{
			_v[i+0] = at[i];
			_v[i+3] = up[i];
		}
	}

	Vec3f At(void) const
	OALPLUS_NOEXCEPT(true)
	{
		return Vec3f(_v, 3);
	}

	Vec3f Up(void) const
	OALPLUS_NOEXCEPT(true)
	{
		return Vec3f(_v+3, 3);
	}
};

/// An AL context-specific monostate object representing the listener
class Listener
{
public:
	/// Specifies the position of the listener
	/**
	 *  @see Velocity
	 *  @see Orientation
	 *  @throws Error
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{POSITION}
	 */
	static void Position(const Vec3f& position)
	{
		OALPLUS_ALFUNC(Listenerfv)(
			AL_POSITION,
			position.Data()
		);
		OALPLUS_VERIFY_SIMPLE(Listenerfv);
	}

	/// Specifies the position of the listener
	/**
	 *  @see Velocity
	 *  @see Orientation
	 *  @throws Error
	 *
	 *  @alsymbols
	 *  @alfunref{Listener3f}
	 *  @aldefref{POSITION}
	 */
	static void Position(ALfloat x, ALfloat y, ALfloat z)
	{
		OALPLUS_ALFUNC(Listener3f)(
			AL_POSITION,
			x, y, z
		);
		OALPLUS_VERIFY_SIMPLE(Listener3f);
	}

	/// Returns the position of the listener
	/**
	 *  @see Velocity
	 *  @see Orientation
	 *
	 *  @alsymbols
	 *  @alfunref{GetListenerfv}
	 *  @aldefref{POSITION}
	 */
	static Vec3f Position(void)
	{
		ALfloat result[3];
		OALPLUS_ALFUNC(GetListenerfv)(
			AL_POSITION,
			result
		);
		OALPLUS_VERIFY_SIMPLE(GetListenerfv);
		return Vec3f(result);
	}

	/// Specifies the velocity vector of the listener
	/**
	 *  @see Position
	 *  @see Orientation
	 *  @throws Error
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{VELOCITY}
	 */
	static void Velocity(const Vec3f& velocity)
	{
		OALPLUS_ALFUNC(Listenerfv)(
			AL_VELOCITY,
			velocity.Data()
		);
		OALPLUS_VERIFY_SIMPLE(Listenerfv);
	}

	/// Specifies the velocity vector of the listener
	/**
	 *  @see Position
	 *  @see Orientation
	 *  @throws Error
	 *
	 *  @alsymbols
	 *  @alfunref{Listener3f}
	 *  @aldefref{VELOCITY}
	 */
	static void Velocity(ALfloat x, ALfloat y, ALfloat z)
	{
		OALPLUS_ALFUNC(Listener3f)(
			AL_VELOCITY,
			x, y, z
		);
		OALPLUS_VERIFY_SIMPLE(Listener3f);
	}

	/// Returns the velocity vector of the listener
	/**
	 *  @see Position
	 *  @see Orientation
	 *
	 *  @alsymbols
	 *  @alfunref{GetListenerfv}
	 *  @aldefref{VELOCITY}
	 */
	static Vec3f Velocity(void)
	{
		ALfloat result[3];
		OALPLUS_ALFUNC(GetListenerfv)(
			AL_VELOCITY,
			result
		);
		OALPLUS_VERIFY_SIMPLE(GetListenerfv);
		return Vec3f(result);
	}

	/// Specifies the orientation vector of the listener
	/**
	 *  @see Position
	 *  @see Velocity
	 *  @throws Error
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{ORIENTATION}
	 */
	static void Orientation(const ListenerOrientation& lo)
	{
		OALPLUS_ALFUNC(Listenerfv)(AL_ORIENTATION, lo._v);
		OALPLUS_VERIFY_SIMPLE(Listenerfv);
	}

	/// Specifies the orientation vector of the listener
	/**
	 *  @see Position
	 *  @see Velocity
	 *  @throws Error
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{ORIENTATION}
	 */
	static void Orientation(const Vec3f& at, const Vec3f& up)
	{
		ALfloat v[6] = {
			at[0], at[1], at[2],
			up[0], up[1], up[2],
		};
		OALPLUS_ALFUNC(Listenerfv)(AL_ORIENTATION, v);
		OALPLUS_VERIFY_SIMPLE(Listenerfv);
	}

	/// Specifies the orientation vector of the listener
	/**
	 *  @see Position
	 *  @see Velocity
	 *  @throws Error
	 *  @see OrientationAt
	 *  @see OrientationUp
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{ORIENTATION}
	 */
	static void Orientation(
		ALfloat at_x,
		ALfloat at_y,
		ALfloat at_z,
		ALfloat up_x,
		ALfloat up_y,
		ALfloat up_z
	)
	{
		ALfloat v[6] = {at_x, at_y, at_z, up_x, up_y, up_z};
		OALPLUS_ALFUNC(Listenerfv)(AL_ORIENTATION, v);
		OALPLUS_VERIFY_SIMPLE(Listenerfv);
	}

	/// Returns the orientation vector of the listener
	/**
	 *  @see Position
	 *  @see Velocity
	 *  @see Orientation
	 *  @see OrientationUp
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{ORIENTATION}
	 */
	static ListenerOrientation Orientation(void)
	{
		ListenerOrientation result;
		OALPLUS_ALFUNC(GetListenerfv)(AL_ORIENTATION, result._v);
		OALPLUS_VERIFY_SIMPLE(GetListenerfv);
		return result;
	}

	/// Returns the orientation vector of the listener
	/**
	 *  @see Position
	 *  @see Velocity
	 *  @see Orientation
	 *  @see OrientationUp
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{ORIENTATION}
	 */
	static Vec3f OrientationAt(void)
	{
		ALfloat v[6];
		OALPLUS_ALFUNC(GetListenerfv)(AL_ORIENTATION, v);
		OALPLUS_VERIFY_SIMPLE(GetListenerfv);
		return Vec3f(v, 3);
	}

	/// Returns the orientation up vector of the listener
	/**
	 *  @see Position
	 *  @see Velocity
	 *  @see OrientationAt
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerfv}
	 *  @aldefref{ORIENTATION}
	 */
	static Vec3f OrientationUp(void)
	{
		ALfloat v[6];
		OALPLUS_ALFUNC(GetListenerfv)(AL_ORIENTATION, v);
		OALPLUS_VERIFY_SIMPLE(GetListenerfv);
		return Vec3f(v+3, 3);
	}

	/// Specifies the value of gain of the listener
	/**
	 *
	 *  @alsymbols
	 *  @alfunref{Listenerf}
	 *  @aldefref{GAIN}
	 */
	static void Gain(ALfloat value)
	{
		OALPLUS_ALFUNC(Listenerf)(
			AL_GAIN,
			value
		);
		OALPLUS_VERIFY_SIMPLE(Listenerf);
	}

	/// Returns the current value of gain of the listener
	/**
	 *
	 *  @alsymbols
	 *  @alfunref{GetListenerfv}
	 *  @aldefref{GAIN}
	 */
	static ALfloat Gain(void)
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetListenerfv)(
			AL_GAIN,
			&result
		);
		OALPLUS_VERIFY_SIMPLE(GetListenerfv);
		return result;
	}
};

} // namespace oalplus

#endif // include guard
