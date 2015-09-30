/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oalplus_context_attribs

typedef __AttributeList<__ContextAttrib, ...> ContextAttribs; /*<
Attribute list type for context attributes.
>*/

typedef __FinishedAttributeList<__ContextAttrib, ...> FinishedContextAttribs; /*<
Finished list of context attribute values.
>*/

//]
//[oalplus_context_ops_1

class ContextOps
{
public:
	static ContextOps Current(void); /*<
	Returns the current OpenAL context.
	See [alcfunc GetCurrentContext], [alcfunc GetContextsDevice].
	>*/

	static StrCRef GetString(__StringQuery query); /*<
	Queries a string from the current OpenAL context.
	See [alcfunc GetString].
	>*/

	static const char* Vendor(void); /*<
	Returns the vendor name.
	See [alcfunc GetString], [alcconst VENDOR].
	>*/
	static const char* Version(void); /*<
	Returns the version string.
	See [alcfunc GetString], [alcconst VENDOR].
	>*/
	static const char* Renderer(void); /*<
	Returns the renderer name.
	See [alcfunc GetString], [alcconst VENDOR].
	>*/

	static __Range<__StrCRef> Extensions(void); /*<
	Returns a range of extension strings.
	See [alcfunc GetString], [alcconst EXTENSIONS].
	>*/


	DeviceOps<tag::Playback> ContextsDevice(void) const; /*<
	Returns the audio device of [^this] context.
	>*/

	bool MakeCurrent(void); /*<
	Makes this context current.
	See [alcfunc MakeContextCurrent].
	>*/

	void Process(void); /*<
	Processes this context.
	See [alcfunc ProcessContext].
	>*/
	void Suspend(void); /*<
	Suspends this context.
	See [alcfunc SuspendContext].
	>*/
//]
//[oalplus_context_ops_2

	static void DistanceModel(__DistanceModel dist_model); /*<
	Sets the distance model to be used by the current context.
	See [alcfunc DistanceModel].
	>*/
	static __DistanceModel DistanceModel(void); /*<
	Returns the distance model used by the current context.
	See [alcfunc GetIntegerv], [alcconst DISTANCE_MODEL].
	>*/

	static void DopplerFactor(ALfloat doppler_factor); /*<
	Sets the doppler factor for the current context.
	See [alcfunc DopplerFactor].
	>*/
	static ALfloat DopplerFactor(void); /*<
	Returns the doppler factor used by the current context.
	See [alcfunc GetFloatv], [alcconst DOPPLER_FACTOR].
	>*/

	static void SpeedOfSound(ALfloat speed_of_sound); /*<
	Sets the value of speed of sound for the current context.
	See [alcfunc SpeedOfSound].
	>*/
	static ALfloat SpeedOfSound(void); /*<
	Returns the value of speed of sound used by the current context.
	See [alcfunc GetFloatv], [alcconst SPEED_OF_SOUND].
	>*/
};

//]
//[oalplus_context

class Context
 : public ContextOps
{
public:
	Context(const Context&) = delete;
	Context(Context&&);

	Context(const __Device& device); /*<
	Constructs a context using the specified device.
	See [alcfunc CreateContext].
	>*/

	Context(
		const __Device& device,
		const __FinishedContextAttribs& attribs
	); /*<
	Construct a context with the specified attributes using the device.
	See [alcfunc CreateContext].
	>*/

	~Context(void); /*<
	Destroys this context.
	See [alcfunc MakeContextCurrent], [alcfunc DestroyContext].
	>*/
};

//]
//[oalplus_context_made_current

class ContextMadeCurrent
 : public __Context
{
public:
	ContextMadeCurrent(const ContextMadeCurrent&) = delete;
	ContextMadeCurrent(ContextMadeCurrent&&);

	ContextMadeCurrent(const __Device& device); /*<
	Constructs a context using the specified device
	and makes it current.
	See [alcfunc CreateContext], [alcfunc MakeCurrent].
	>*/

	ContextMadeCurrent(
		const __Device& device,
		const __FinishedContextAttribs& attribs
	); /*<
	Construct a context with the specified attributes using
	the specified device and makes it current
	See [alcfunc CreateContext], [alcfunc MakeCurrent].
	>*/
};

//]

