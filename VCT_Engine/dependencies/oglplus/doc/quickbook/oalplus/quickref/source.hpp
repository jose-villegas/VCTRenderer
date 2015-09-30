/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oalplus_source_common_1

template <>
class __ObjectOps<__tag_DirectState, __tag_Source>
 : public __SourceName
{
public:
	void Play(void); /*<
	Starts the audio playback.
	See [alfunc SourcePlay].
	>*/
	void Pause(void); /*<
	Pauses the audio playback.
	See [alfunc SourcePause].
	>*/
	void Stop(void); /*<
	Stops the audio playback.
	See [alfunc SourceStop].
	>*/
	void Rewind(void); /*<
	Rewinds the audio track.
	See [alfunc SourceRewind].
	>*/

	__SourceState State(void) const; /*<
	Returns the source state.
	See [alfunc GetSourceiv], [alconst SOURCE_STATE].
	>*/

	void Relative(__Boolean value); /*<
	Sets the source to relative or absoulte state.
	See [alfunc Sourcei], [alconst SOURCE_RELATIVE].
	>*/
	__Boolean Relative(void) const; /*<
	Returns true if the source is relative.
	See [alfunc GetSourceiv], [alconst SOURCE_RELATIVE].
	>*/

	void Type(__SourceType type); /*<
	Sets the source type.
	See [alfunc Sourcei], [alconst SOURCE_TYPE].
	>*/
	__SourceType Type(void) const; /*<
	Returns the source type.
	See [alfunc GetSourceiv], [alconst SOURCE_TYPE].
	>*/

	void Looping(__Boolean value); /*<
	Sets the looping mode.
	See [alfunc Sourcei], [alconst LOOPING].
	>*/
	__Boolean Looping(void) const; /*<
	Returns true if the source is in looping mode.
	See [alfunc GetSourceiv], [alconst SOURCE_LOOPING].
	>*/

	void Buffer(const __BufferName& buffer); /*<
	Assigns an audio buffer to the source.
	See [alfunc Sourcei], [alconst BUFFER].
	>*/
	void DetachBuffers(void); /*<
	Detaches all queued buffers from the source.
	See [alfunc Sourcei], [alconst BUFFER].
	>*/
	void QueueBuffers(const __Sequence<__BufferName>& buffers); /*<
	Enqueues multiple buffers to the source.
	See [alfunc SourceQueueBuffers].
	>*/
	void UnqueueBuffers(const __Sequence<__BufferName>& buffers); /*<
	Enqueues multiple buffers to the source.
	See [alfunc SourceUnqueueBuffers].
	>*/
//]
//[oalplus_source_common_2

	void Gain(ALfloat value); /*<
	Sets the value of gain.
	See [alfunc Sourcef], [alconst GAIN].
	>*/
	ALfloat Gain(void) const; /*<
	Returns the value of gain.
	See [alfunc GetSourcefv], [alconst GAIN].
	>*/

	void MinGain(ALfloat value); /*<
	Sets the minimal value of gain.
	See [alfunc Sourcef], [alconst MIN_GAIN].
	>*/
	ALfloat MinGain(void) const; /*<
	Returns the minimal value of gain.
	See [alfunc GetSourcefv], [alconst MIN_GAIN].
	>*/

	void MaxGain(ALfloat value); /*<
	Sets the maximal value of gain.
	See [alfunc Sourcef], [alconst MAX_GAIN].
	>*/
	ALfloat MaxGain(void) const; /*<
	Returns the maximal value of gain.
	See [alfunc GetSourcefv], [alconst MAX_GAIN].
	>*/

	void ReferenceDistance(ALfloat value); /*<
	Sets the reference distance.
	See [alfunc Sourcef], [alconst REFERENCE_DISTANCE].
	>*/
	ALfloat ReferenceDistance(void) const; /*<
	Returns the reference distance.
	See [alfunc GetSourcefv], [alconst REFERENCE_DISTANCE].
	>*/

	void RolloffFactor(ALfloat value); /*<
	Sets the value of the rolloff factor.
	See [alfunc Sourcef], [alconst ROLLOFF_FACTOR].
	>*/
	ALfloat RolloffFactor(void) const; /*<
	Returns the value of the rolloff factor.
	See [alfunc GetSourcefv], [alconst ROLLOFF_FACTOR].
	>*/

	void MaxDistance(ALfloat value); /*<
	Sets the value of max distance used with some distance models.
	See [alfunc Sourcef], [alconst MAX_DISTANCE].
	>*/
	ALfloat MaxDistance(void) const; /*<
	Returns the value of max distance used with some distance models.
	See [alfunc GetSourcefv], [alconst MAX_DISTANCE].
	>*/

	void Pitch(ALfloat value); /*<
	Sets the value of pitch.
	See [alfunc Sourcef], [alconst PITCH].
	>*/
	ALfloat Pitch(void) const; /*<
	Returns the value of pitch.
	See [alfunc GetSourcefv], [alconst PITCH].
>*/
//]
//[oalplus_source_common_3

	void Position(const __Vec3f& dir); /*<
	Sets the position of the source.
	See [alfunc Sourcefv], [alconst POSITION].
	>*/
	void Position(ALfloat x, ALfloat y, ALfloat z);
	__Vec3f Position(void) const; /*<
	Returns the position of the source.
	See [alfunc GetSourcefv], [alconst POSITION].
	>*/

	void Velocity(const __Vec3f& dir); /*<
	Sets the velocity vector of the source.
	See [alfunc Sourcefv], [alconst VELOCITY].
	>*/
	void Velocity(ALfloat x, ALfloat y, ALfloat z);
	__Vec3f Velocity(void) const; /*<
	Returns the velocity vector of the source.
	See [alfunc GetSourcefv], [alconst VELOCITY].
	>*/

	void Direction(const __Vec3f& dir); /*<
	Sets the direction vector of the source.
	See [alfunc Sourcefv], [alconst DIRECTION].
	>*/
	void Direction(ALfloat x, ALfloat y, ALfloat z);
	__Vec3f Direction(void) const; /*<
	Returns the direction vector of the source.
	See [alfunc GetSourcefv], [alconst DIRECTION].
	>*/

	void ConeInnerAngle(__Anglef angle); /*<
	Sets the sound cone's inner angle.
	See [alfunc Sourcef], [alconst CONE_INNER_ANGLE].
	>*/
	__Anglef ConeInnerAngle(void) const; /*<
	Returns the sound cone's inner angle.
	See [alfunc GetSourcefv], [alconst CONE_INNER_ANGLE].
	>*/

	void ConeOuterAngle(__Anglef angle); /*<
	Sets the sound cone's outer angle.
	See [alfunc Sourcef], [alconst CONE_OUTER_ANGLE].
	>*/
	__Anglef ConeOuterAngle(void) const; /*<
	Returns the sound cone's outer angle.
	See [alfunc GetSourcefv], [alconst CONE_OUTER_ANGLE].
	>*/

	void ConeOuterGain(ALfloat value); /*<
	Sets the sound cone's outer gain value.
	See [alfunc Sourcef], [alconst CONE_OUTER_GAIN].
	>*/
	ALfloat ConeOuterGain(void) const; /*<
	Returns the sound cone's outer gain value.
	See [alfunc GetSourcefv], [alconst CONE_OUTER_GAIN].
	>*/

	void SecOffset(ALfloat value); /*<
	Sets the Second-offset value.
	See [alfunc Sourcef], [alconst SEC_OFFSET].
	>*/
	void SampleOffset(ALfloat value); /*<
	Sets the sample-offset value.
	See [alfunc Sourcef], [alconst SAMPLE_OFFSET].
	>*/
	void ByteOffset(ALfloat value); /*<
	Sets the byte-offset value.
	See [alfunc Sourcef], [alconst BYTE_OFFSET].
	>*/
};
//]
//[oalplus_source_def

typedef ObjectOps<__tag_DirectState, __tag_Source>
	SourceOps;

typedef __Object<SourceOps> Source;

//]

