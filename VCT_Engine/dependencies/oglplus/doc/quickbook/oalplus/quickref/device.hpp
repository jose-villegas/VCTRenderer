/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oalplus_device_common
namespace tag {

struct Playback { };
struct Capture { };

} // namespace tag

class DevCommonOps
{
public:
	DevCommonOps(DevCommonOps&&) = default;

	static __Range<__StrCRef> Extensions(void); /*<
	Returns a range of ALC extension strings for
	[^this] devices.
	See [alcfunc GetString], [alcconst EXTENSIONS].
	>*/
};

template <typename DeviceTag>
class DeviceOps;

//]
//[oalplus_device_playback_ops

template <>
class DeviceOps<tag::Playback>
 : public DevCommonOps
{
public:
	__StrCRef Specifier(void) const; /*<
	Returns the device specifier string.
	See [alcfunc GetString], [alcconst DEVICE_SPECIFIER].
	>*/

	static __Range<__StrCRef> Specifiers(void); /*<
	Returns a range of specifier strings for available
	audio playback devices.
	See [alcfunc GetString], [alcconst DEVICE_SPECIFIER].
	>*/
};

//]
//[oalplus_device_capture_ops

template <>
class DeviceOps<tag::Capture>
 : public DevCommonOps
{
public:
	__StrCRef Specifier(void) const; /*<
	Returns the device specifier string.
	See [alcfunc GetString], [alcconst CAPTURE_DEVICE_SPECIFIER].
	>*/

	static __Range<__StrCRef> Specifiers(void); /*<
	Returns a range of specifier strings for available
	audio capture devices.
	See [alcfunc GetString], [alcconst CAPTURE_DEVICE_SPECIFIER].
	>*/

	void Start(void); /*<
	Starts audio capture on [^this] device.
	See [alcfunc CaptureStart].
	>*/
	void Stop(void); /*<
	Stops audio capture on [^this] device.
	See [alcfunc CaptureStop].
	>*/

	ALCsizei Samples(void) const; /*<
	Gets the number of samples captured on [^this] device.
	See [alcfunc GetInteger], [alcconst CAPTURE_SAMPLES].
	>*/
	void Samples(ALCvoid* buffer, ALCsizei samples) const; /*<
	Gets the samples captured on [^this] device.
	See [alcfunc CaptureSamples].
	>*/
};

//]
//[oalplus_playback_device

class Device
 : public DeviceOps<tag::Playback>
{
public:
	Device(void); /*<
	Constructs an object referencing the default audio playback device.
	See [alcfunc OpenDevice].
	>*/

	Device(__StrCRef dev_spec); /*<
	Constructs an object referencing the specified audio device.
	See [alcfunc OpenDevice].
	>*/
};

//]
//[oalplus_capture_device

class CaptureDevice
 : public DeviceOps<tag::Capture>
{
public:
	CaptureDevice(
		ALCuint frequency,
		__DataFormat format,
		ALCsizei bufsize
	); /*<
	Constructs an object referencing the default audio capture device.
	See [alcfunc CaptureOpenDevice].
	>*/

	CaptureDevice(
		StrCRef dev_spec,
		ALCuint frequency,
		__DataFormat format,
		ALCsizei bufsize
	); /*<
	Constructs an object referencing the specified audio capture device.
	See [alcfunc CaptureOpenDevice].
	>*/
};

//]

