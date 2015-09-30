/**
 *  @file oalplus/source.hpp
 *  @brief Wrapper for OpenAL source object.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_SOURCE_1303201647_HPP
#define OALPLUS_SOURCE_1303201647_HPP

#include <oalplus/config.hpp>
#include <oalplus/fwd.hpp>
#include <oalplus/alfunc.hpp>
#include <oalplus/boolean.hpp>
#include <oalplus/math/angle.hpp>
#include <oalplus/math/vector.hpp>
#include <oalplus/error/al.hpp>
#include <oalplus/object/wrapper.hpp>
#include <oalplus/object/sequence.hpp>
#include <oalplus/buffer.hpp>
#include <oalplus/source_state.hpp>
#include <oalplus/source_type.hpp>

#include <cassert>

namespace oalplus {

/// Class wrapping AL source construction/destruction functions
/** @note Do not use this class directly, use Source instead.
 *
 *  @alsymbols
 *  @alfunref{GenSources}
 *  @alfunref{DeleteSources}
 *  @alfunref{IsSource}
 */
template <>
class ObjGenDelOps<tag::Source>
{
protected:
	static void Gen(tag::Generate, ALsizei count, ALuint* names)
	{
		assert(names != nullptr);
		OALPLUS_ALFUNC(GenSources)(count, names);
		OALPLUS_CHECK_SIMPLE(GenSources);
	}

	static void Delete(ALsizei count, ALuint* names)
	{
		assert(names != nullptr);
		OALPLUS_ALFUNC(DeleteSources)(count, names);
		OALPLUS_VERIFY_SIMPLE(DeleteSources);
	}

	static ALboolean IsA(ALuint name)
	{
		assert(name != 0);
		ALboolean result = OALPLUS_ALFUNC(IsSource)(name);
		OALPLUS_VERIFY_SIMPLE(IsSource);
		return result;
	}
};

/// Wrapper for OpenAL source operations
/**
 *  @note Do not use this class directly, use Source instead
 *
 *  @alsymbols
 */
template <>
class ObjectOps<tag::DirectState, tag::Source>
 : public SourceName
{
protected:
	ObjectOps(SourceName name)
	OALPLUS_NOEXCEPT(true)
	 : SourceName(name)
	{ }
public:
#if !OGLPLUS_NO_DEFAULTED_FUNCTIONS
	ObjectOps(ObjectOps&&) = default;
	ObjectOps(const ObjectOps&) = default;
	ObjectOps& operator = (ObjectOps&&) = default;
	ObjectOps& operator = (const ObjectOps&) = default;
#else
	typedef SourceName _base;

	ObjectOps(ObjectOps&& temp)
	OGLPLUS_NOEXCEPT(true)
	 : _base(static_cast<_base&&>(temp))
	{ }

	ObjectOps(const ObjectOps& that)
	OGLPLUS_NOEXCEPT(true)
	 : _base(static_cast<const _base&>(that))
	{ }

	ObjectOps& operator = (ObjectOps&& temp)
	OGLPLUS_NOEXCEPT(true)
	{
		_base::operator = (static_cast<_base&&>(temp));
		return *this;
	}

	ObjectOps& operator = (const ObjectOps& that)
	OGLPLUS_NOEXCEPT(true)
	{
		_base::operator = (static_cast<const _base&>(that));
		return *this;
	}
#endif
	/// Starts the audio playback
	/**
	 *  @alsymbols
	 *  @alfunref{SourcePlay}
	 */
	void Play(void)
	{
		OALPLUS_ALFUNC(SourcePlay)(_obj_name());
		OALPLUS_VERIFY(
			SourcePlay,
			ObjectError,
			Object(*this)
		);
	}

	/// Pauses the audio playback
	/**
	 *  @alsymbols
	 *  @alfunref{SourcePause}
	 */
	void Pause(void)
	{
		OALPLUS_ALFUNC(SourcePause)(_obj_name());
		OALPLUS_VERIFY(
			SourcePause,
			ObjectError,
			Object(*this)
		);
	}

	/// Stops the audio playback
	/**
	 *  @alsymbols
	 *  @alfunref{SourceStop}
	 */
	void Stop(void)
	{
		OALPLUS_ALFUNC(SourceStop)(_obj_name());
		OALPLUS_VERIFY(
			SourceStop,
			ObjectError,
			Object(*this)
		);
	}

	/// Rewinds the audio track
	/**
	 *  @alsymbols
	 *  @alfunref{SourceRewind}
	 */
	void Rewind(void)
	{
		OALPLUS_ALFUNC(SourceRewind)(_obj_name());
		OALPLUS_VERIFY(
			SourceRewind,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the source state
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourceiv}
	 *  @aldefref{SOURCE_STATE}
	 */
	SourceState State(void) const
	{
		ALint result = 0;
		OALPLUS_ALFUNC(GetSourceiv)(
			_obj_name(),
			AL_SOURCE_STATE,
			&result
		);
		OALPLUS_VERIFY(
			GetSourceiv,
			ObjectError,
			Object(*this)
		);
		return SourceState(result);
	}

	/// Sets the source to relative or absoulte state
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcei}
	 *  @aldefref{SOURCE_RELATIVE}
	 */
	void Relative(Boolean value)
	{
		OALPLUS_ALFUNC(Sourcei)(
			_obj_name(),
			AL_SOURCE_RELATIVE,
			value._get()
		);
		OALPLUS_VERIFY(
			Sourcei,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns true if the source is relative
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourceiv}
	 *  @aldefref{SOURCE_RELATIVE}
	 */
	Boolean Relative(void) const
	{
		Boolean result;
		OALPLUS_ALFUNC(GetSourceiv)(
			_obj_name(),
			AL_SOURCE_RELATIVE,
			result._ptr()
		);
		OALPLUS_VERIFY(
			GetSourceiv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the source type
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcei}
	 *  @aldefref{SOURCE_TYPE}
	 */
	void Type(SourceType type)
	{
		OALPLUS_ALFUNC(Sourcei)(
			_obj_name(),
			AL_SOURCE_TYPE,
			ALenum(type)
		);
		OALPLUS_VERIFY(
			Sourcei,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the source type
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourceiv}
	 *  @aldefref{SOURCE_TYPE}
	 */
	SourceType Type(void) const
	{
		ALint result;
		OALPLUS_ALFUNC(GetSourceiv)(
			_obj_name(),
			AL_SOURCE_TYPE,
			&result
		);
		OALPLUS_VERIFY(
			GetSourceiv,
			ObjectError,
			Object(*this)
		);
		return SourceType(ALenum(result));
	}

	/// Sets the looping mode
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcei}
	 *  @aldefref{LOOPING}
	 */
	void Looping(Boolean value)
	{
		OALPLUS_ALFUNC(Sourcei)(
			_obj_name(),
			AL_LOOPING,
			value._get()
		);
		OALPLUS_VERIFY(
			Sourcei,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns true if the source is in looping mode
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourceiv}
	 *  @aldefref{LOOPING}
	 */
	Boolean Looping(void) const
	{
		Boolean result;
		OALPLUS_ALFUNC(GetSourceiv)(
			_obj_name(),
			AL_LOOPING,
			result._ptr()
		);
		OALPLUS_VERIFY(
			GetSourceiv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Assigns an audio buffer to the source
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcei}
	 *  @aldefref{BUFFER}
	 */
	void Buffer(const BufferName& buffer)
	{
		OALPLUS_ALFUNC(Sourcei)(
			_obj_name(),
			AL_BUFFER,
			GetALName(buffer)
		);
		OALPLUS_VERIFY(
			Sourcei,
			ObjectError,
			Object(*this)
		);
	}

	/// Detaches all queued buffers from the source
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcei}
	 *  @aldefref{BUFFER}
	 */
	void DetachBuffers(void)
	{
		OALPLUS_ALFUNC(Sourcei)(
			_obj_name(),
			AL_BUFFER,
			0
		);
		OALPLUS_VERIFY(
			Sourcei,
			ObjectError,
			Object(*this)
		);
	}

	/// Enqueues multiple buffers to the source
	/**
	 *  @alsymbols
	 *  @alfunref{SourceQueueBuffers}
	 */
	void QueueBuffers(const Sequence<BufferName>& buffers)
	{
		OALPLUS_ALFUNC(SourceQueueBuffers)(
			_obj_name(),
			ALsizei(buffers.size()),
			const_cast<ALuint*>(GetALNames(buffers))
		);
		OALPLUS_VERIFY(
			SourceQueueBuffers,
			ObjectError,
			Object(*this)
		);
	}

	/// Removes multiple buffers from the sources queue
	/**
	 *  @alsymbols
	 *  @alfunref{SourceUnqueueBuffers}
	 */
	void UnqueueBuffers(const Sequence<BufferName>& buffers)
	{
		OALPLUS_ALFUNC(SourceUnqueueBuffers)(
			_obj_name(),
			ALsizei(buffers.size()),
			const_cast<ALuint*>(GetALNames(buffers))
		);
		OALPLUS_VERIFY(
			SourceUnqueueBuffers,
			ObjectError,
			Object(*this)
		);
	}

	/// Sets the value of gain
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{GAIN}
	 */
	void Gain(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_GAIN,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the value of gain
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{GAIN}
	 */
	ALfloat Gain(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_GAIN,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the minimal value of gain
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{MIN_GAIN}
	 */
	void MinGain(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_MIN_GAIN,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the minimal value of gain
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{MIN_GAIN}
	 */
	ALfloat MinGain(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_MIN_GAIN,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the maximum value of gain
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{MAX_GAIN}
	 */
	void MaxGain(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_MAX_GAIN,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the maximum value of gain
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{MAX_GAIN}
	 */
	ALfloat MaxGain(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_MAX_GAIN,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the reference distance
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{REFERENCE_DISTANCE}
	 */
	void ReferenceDistance(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_REFERENCE_DISTANCE,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the reference distance
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{REFERENCE_DISTANCE}
	 */
	ALfloat ReferenceDistance(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_REFERENCE_DISTANCE,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the value of rolloff factor
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{ROLLOFF_FACTOR}
	 */
	void RolloffFactor(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_ROLLOFF_FACTOR,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the value of rolloff factor
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{ROLLOFF_FACTOR}
	 */
	ALfloat RolloffFactor(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_ROLLOFF_FACTOR,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the value of max distance used with some distance models
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{MAX_DISTANCE}
	 */
	void MaxDistance(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_MAX_DISTANCE,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the value of max distance used with some distance models
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{MAX_DISTANCE}
	 */
	ALfloat MaxDistance(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_MAX_DISTANCE,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the value of pitch
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{PITCH}
	 */
	void Pitch(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_PITCH,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the value of pitch
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{PITCH}
	 */
	ALfloat Pitch(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_PITCH,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the position of the source
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcefv}
	 *  @aldefref{POSITION}
	 */
	void Position(const Vec3f& dir)
	{
		OALPLUS_ALFUNC(Sourcefv)(
			_obj_name(),
			AL_POSITION,
			dir.Data()
		);
		OALPLUS_CHECK(
			Sourcefv,
			ObjectError,
			Object(*this)
		);
	}

	/// Sets the position of the source
	/**
	 *  @alsymbols
	 *  @alfunref{Source3f}
	 *  @aldefref{POSITION}
	 */
	void Position(ALfloat x, ALfloat y, ALfloat z)
	{
		OALPLUS_ALFUNC(Source3f)(
			_obj_name(),
			AL_POSITION,
			x, y, z
		);
		OALPLUS_CHECK(
			Source3f,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the position of the source
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{POSITION}
	 */
	Vec3f Position(void) const
	{
		ALfloat result[3];
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_POSITION,
			result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return Vec3f(result);
	}

	/// Sets the velocity of the source
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcefv}
	 *  @aldefref{VELOCITY}
	 */
	void Velocity(const Vec3f& dir)
	{
		OALPLUS_ALFUNC(Sourcefv)(
			_obj_name(),
			AL_VELOCITY,
			dir.Data()
		);
		OALPLUS_CHECK(
			Sourcefv,
			ObjectError,
			Object(*this)
		);
	}

	/// Sets the velocity of the source
	/**
	 *  @alsymbols
	 *  @alfunref{Source3f}
	 *  @aldefref{VELOCITY}
	 */
	void Velocity(ALfloat x, ALfloat y, ALfloat z)
	{
		OALPLUS_ALFUNC(Source3f)(
			_obj_name(),
			AL_VELOCITY,
			x, y, z
		);
		OALPLUS_CHECK(
			Source3f,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the velocity of the source
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{VELOCITY}
	 */
	Vec3f Velocity(void) const
	{
		ALfloat result[3];
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_VELOCITY,
			result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return Vec3f(result);
	}

	/// Sets the direction of the source
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcefv}
	 *  @aldefref{DIRECTION}
	 */
	void Direction(const Vec3f& dir)
	{
		OALPLUS_ALFUNC(Sourcefv)(
			_obj_name(),
			AL_DIRECTION,
			dir.Data()
		);
		OALPLUS_CHECK(
			Sourcefv,
			ObjectError,
			Object(*this)
		);
	}

	/// Sets the direction of the source
	/**
	 *  @alsymbols
	 *  @alfunref{Source3f}
	 *  @aldefref{DIRECTION}
	 */
	void Direction(ALfloat x, ALfloat y, ALfloat z)
	{
		OALPLUS_ALFUNC(Source3f)(
			_obj_name(),
			AL_DIRECTION,
			x, y, z
		);
		OALPLUS_CHECK(
			Source3f,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the direction of the source
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{DIRECTION}
	 */
	Vec3f Direction(void) const
	{
		ALfloat result[3];
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_DIRECTION,
			result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return Vec3f(result);
	}

	/// Sets the sound cone's inner angle
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{CONE_INNER_ANGLE}
	 */
	void ConeInnerAngle(Anglef angle)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_CONE_INNER_ANGLE,
			angle.ValueInDegrees()
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the sound cone's inner angle
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{CONE_INNER_ANGLE}
	 */
	Anglef ConeInnerAngle(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_CONE_INNER_ANGLE,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return Anglef::Degrees(result);
	}

	/// Sets the sound cone's outer angle
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{CONE_OUTER_ANGLE}
	 */
	void ConeOuterAngle(Anglef angle)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_CONE_OUTER_ANGLE,
			angle.ValueInDegrees()
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the sound cone's outer angle
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{CONE_OUTER_ANGLE}
	 */
	Anglef ConeOuterAngle(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_CONE_OUTER_ANGLE,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return Anglef::Degrees(result);
	}

	/// Sets the sound cone's outer gain value
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{CONE_OUTER_GAIN}
	 */
	void ConeOuterGain(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_CONE_OUTER_GAIN,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Returns the sound cone's outer gain value
	/**
	 *  @alsymbols
	 *  @alfunref{GetSourcefv}
	 *  @aldefref{CONE_OUTER_GAIN}
	 */
	ALfloat ConeOuterGain(void) const
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetSourcefv)(
			_obj_name(),
			AL_CONE_OUTER_GAIN,
			&result
		);
		OALPLUS_VERIFY(
			GetSourcefv,
			ObjectError,
			Object(*this)
		);
		return result;
	}

	/// Sets the Sec-offset value
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{SEC_OFFSET}
	 */
	void SecOffset(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_SEC_OFFSET,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Sets the sample-offset value
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{SAMPLE_OFFSET}
	 */
	void SampleOffset(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_SAMPLE_OFFSET,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

	/// Sets the byte-offset value
	/**
	 *  @alsymbols
	 *  @alfunref{Sourcef}
	 *  @aldefref{BYTE_OFFSET}
	 */
	void ByteOffset(ALfloat value)
	{
		OALPLUS_ALFUNC(Sourcef)(
			_obj_name(),
			AL_BYTE_OFFSET,
			value
		);
		OALPLUS_CHECK(
			Sourcef,
			ObjectError,
			Object(*this)
		);
	}

};

} // namespace oalplus
namespace oglplus {

template <>
class ObjGenDelOps<oalplus::tag::Source>
 : public oalplus::ObjGenDelOps<oalplus::tag::Source>
{ };

template <typename OpsTag>
class ObjectOps<OpsTag, oalplus::tag::Source>
 : public oalplus::ObjectOps<OpsTag, oalplus::tag::Source>
{
protected:
	ObjectOps(ObjectName<oalplus::tag::Source> name)
	OALPLUS_NOEXCEPT(true)
	 : oalplus::ObjectOps<OpsTag, oalplus::tag::Source>(name)
	{ }
};

} // namespace oglplus
namespace oalplus {

typedef oglplus::ObjectOps<tag::DirectState, tag::Source>
	SourceOps;

/// An @ref oalplus_object encapsulating the OpenAL source functionality
/**
 *  @ingroup oalplus_objects
 */
typedef Object<SourceOps> Source;

} // namespace oalplus

#endif // include guard
