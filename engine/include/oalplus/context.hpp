/**
 *  @file oalplus/context.hpp
 *  @brief Wrapper around a OpenAL context
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_CONTEXT_1303201648_HPP
#define OALPLUS_CONTEXT_1303201648_HPP

#include <oalplus/config.hpp>
#include <oalplus/alfunc.hpp>
#include <oalplus/device.hpp>
#include <oalplus/distance_model.hpp>
#include <oalplus/context_attrib.hpp>
#include <oalplus/string_query.hpp>
#include <oalplus/attrib_list.hpp>
#include <oalplus/error/al.hpp>
#include <oalplus/error/alc.hpp>

#include <oalplus/detail/sep_str_range.hpp>

namespace oalplus {

struct ContextValueTypeToContextAttrib
{
	static std::integral_constant<int, -1> MaxValueType(void);
};


/// Attribute list for context attributes
typedef AttributeList<
	ContextAttrib,
	ContextValueTypeToContextAttrib,
	AttributeListTraits
> ContextAttribs;

/// Finished list of context attribute values
typedef FinishedAttributeList<
	ContextAttrib,
	AttributeListTraits
> FinishedContextAttribs;

/// Base wrapper for OpenAL context operations
/**
 *  @note Do not use this class directly, use Context instead
 *
 *  @see Context
 */
class ContextOps
{
protected:
	::ALCdevice* _device;
	::ALCcontext* _context;

	ContextOps(
		::ALCdevice* device,
		::ALCcontext* context
	): _device(device)
	 , _context(context)
	{
		assert(_device);
		assert(_context);
	}
public:
	/// Returns the current OpenAL context
	/**
	 *  @alsymbols
	 *  @alcfunref{GetCurrentContext}
	 *  @alcfunref{GetContextsDevice}
	 */
	static ContextOps Current(void);

	/// Queries a string from the current OpenAL context
	/**
	 *  @alsymbols
	 *  @alfunref{GetString}
	 */
	static const ALchar* GetString(StringQuery query)
	{
		const ALchar* str = OALPLUS_ALFUNC(GetString)(ALenum(query));
		OALPLUS_VERIFY_SIMPLE(GetString);
		return str;
	}

	/// Returns the vendor name
	/**
	 *  @alsymbols
	 *  @alfunref{GetString}
	 *  @aldefref{VENDOR}
	 */
	static const char* Vendor(void)
	{
		return reinterpret_cast<const char*>(
			GetString(StringQuery::Vendor)
		);
	}

	/// Returns the version string
	/**
	 *  @alsymbols
	 *  @alfunref{GetString}
	 *  @aldefref{VERSION}
	 */
	static const char* Version(void)
	{
		return reinterpret_cast<const char*>(
			GetString(StringQuery::Version)
		);
	}

	/// Returns the renderer name
	/**
	 *  @alsymbols
	 *  @alfunref{GetString}
	 *  @aldefref{RENDERER}
	 */
	static const char* Renderer(void)
	{
		return reinterpret_cast<const char*>(
			GetString(StringQuery::Renderer)
		);
	}

#if OALPLUS_DOCUMENTATION_ONLY
	/// Returns a range of extension strings
	/**
	 *  @alsymbols
	 *  @alfunref{GetString}
	 *  @aldefref{EXTENSIONS}
	 */
	static Range<String> Extensions(void);
#else
	static aux::SepStrRange Extensions(void)
	{
		return aux::SepStrRange(
			reinterpret_cast<const char*>(
				GetString(StringQuery::Extensions)
			)
		);
	}
#endif


	/// Returns the device of this context
	DeviceOps<tag::Playback> ContextsDevice(void) const
	{
		return DeviceOps<tag::Playback>(_device);
	}

	/// Makes this context current
	/**
	 *  @alsymbols
	 *  @alcfunref{MakeContextCurrent}
	 */
	bool MakeCurrent(void)
	{
		bool result = OALPLUS_ALCFUNC(MakeContextCurrent)(_context);
		OALPLUS_VERIFY_SIMPLE_ALC(_device, MakeContextCurrent);
		return result;
	}

	/// Processes this context
	/**
	 *  @alsymbols
	 *  @alcfunref{ProcessContext}
	 */
	void Process(void)
	{
		OALPLUS_ALCFUNC(ProcessContext)(_context);
		OALPLUS_VERIFY_SIMPLE_ALC(_device, ProcessContext);
	}

	/// Suspends this context
	/**
	 *  @alsymbols
	 *  @alcfunref{SuspendContext}
	 */
	void Suspend(void)
	{
		OALPLUS_ALCFUNC(SuspendContext)(_context);
		OALPLUS_VERIFY_SIMPLE_ALC(_device, SuspendContext);
	}

	/// Sets the distance model to be used by the current context
	/**
	 *  @alsymbols
	 *  @alfunref{DistanceModel}
	 */
	static void DistanceModel(oalplus::DistanceModel dist_model)
	{
		OALPLUS_ALFUNC(DistanceModel(ALenum(dist_model)));
		OALPLUS_VERIFY_SIMPLE(DistanceModel);
	}

	/// Returns the distance model used by the current context
	/**
	 *  @alsymbols
	 *  @alfunref{GetIntegerv}
	 *  @aldefref{DISTANCE_MODEL}
	 */
	static oalplus::DistanceModel DistanceModel(void)
	{
		ALint result;
		OALPLUS_ALFUNC(GetIntegerv(
			AL_DISTANCE_MODEL,
			&result
		));
		OALPLUS_VERIFY_SIMPLE(GetIntegerv);
		return oalplus::DistanceModel(result);
	}

	/// Sets the doppler factor for the current context
	/**
	 *  @alsymbols
	 *  @alfunref{DopplerFactor}
	 */
	static void DopplerFactor(ALfloat doppler_factor)
	{
		OALPLUS_ALFUNC(DopplerFactor(doppler_factor));
		OALPLUS_CHECK_SIMPLE(DopplerFactor);
	}

	/// Returns the doppler factor used by the current context
	/**
	 *  @alsymbols
	 *  @alfunref{GetFloatv}
	 *  @aldefref{DOPPLER_FACTOR}
	 */
	static ALfloat DopplerFactor(void)
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetFloatv(
			AL_DOPPLER_FACTOR,
			&result
		));
		OALPLUS_VERIFY_SIMPLE(GetFloatv);
		return result;
	}

	/// Sets the value of speed of sound for the current context
	/**
	 *  @alsymbols
	 *  @alfunref{SpeedOfSound}
	 */
	static void SpeedOfSound(ALfloat speed_of_sound)
	{
		OALPLUS_ALFUNC(SpeedOfSound(speed_of_sound));
		OALPLUS_CHECK_SIMPLE(SpeedOfSound);
	}

	/// Returns the value of speed of sound used by the current context
	/**
	 *  @alsymbols
	 *  @alfunref{GetFloatv}
	 *  @aldefref{SPEED_OF_SOUND}
	 */
	static ALfloat SpeedOfSound(void)
	{
		ALfloat result;
		OALPLUS_ALFUNC(GetFloatv(
			AL_SPEED_OF_SOUND,
			&result
		));
		OALPLUS_VERIFY_SIMPLE(GetFloatv);
		return result;
	}
};

/// Wrapper for OpenAL context operations
class Context
 : public ContextOps
{
private:
	Context(const Context&);
public:
	/// Construct a context using the specified device
	/**
	 *  @alsymbols
	 *  @alcfunref{CreateContext}
	 */
	Context(const Device& device)
	 : ContextOps(
		device._device,
		OALPLUS_ALCFUNC(CreateContext)(device._device, nullptr)
	)
	{
		OALPLUS_CHECK_SIMPLE_ALC(_device, CreateContext);
	}

	/// Construct a context with the specified attributes using the device
	/**
	 *  @alsymbols
	 *  @alcfunref{CreateContext}
	 */
	Context(
		const Device& device,
		const FinishedContextAttribs& attribs
	): ContextOps(
		device._device,
		OALPLUS_ALCFUNC(CreateContext)(
			device._device,
			attribs.Get()
		)
	)
	{
		OALPLUS_CHECK_SIMPLE_ALC(_device, CreateContext);
	}

	/// Contexts are move-only
	Context(Context&& tmp)
	 : ContextOps(tmp._device, tmp._context)
	{
		tmp._context = nullptr;
	}

	/// Destroys this context
	/**
	 *  @alsymbols
	 *  @alcfunref{MakeContextCurrent}
	 *  @alcfunref{DestroyContext}
	 */
	~Context(void)
	{
		if(_context)
		{
			OALPLUS_ALCFUNC(MakeContextCurrent)(nullptr);
			OALPLUS_ALCFUNC(DestroyContext)(_context);
		}
	}
};

/// A context that is made current right after construction
class ContextMadeCurrent
 : public Context
{
public:
	/// Creates a new context and makes it current
	/**
	 *  @alsymbols
	 *  @alcfunref{CreateContext}
	 *  @alcfunref{MakeContextCurrent}
	 */
	ContextMadeCurrent(const Device& device)
	 : Context(device)
	{
		MakeCurrent();
	}

	/// Creates a new context and makes it current
	/**
	 *  @alsymbols
	 *  @alcfunref{CreateContext}
	 *  @alcfunref{MakeContextCurrent}
	 */
	ContextMadeCurrent(
		const Device& device,
		const FinishedContextAttribs& attribs
	): Context(device, attribs)
	{
		MakeCurrent();
	}

	/// ContextMadeCurrent is move-constructible
	/**
	 *  @alsymbols
	 *  @alcfunref{MakeContextCurrent}
	 */
	ContextMadeCurrent(ContextMadeCurrent&& tmp)
	 : Context(static_cast<Context&&>(tmp))
	{
		if(_context) MakeCurrent();
	}
};

} // namespace oalplus

#endif // include guard
