/**
 *  @file eglplus/configs.hpp
 *  @brief EGL configurations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_CONFIGS_1303292057_HPP
#define EGLPLUS_CONFIGS_1303292057_HPP

#include <eglplus/bitfield.hpp>
#include <eglplus/config_attrib.hpp>
#include <eglplus/color_buffer_type.hpp>
#include <eglplus/transparent_type.hpp>
#include <eglplus/config_caveat.hpp>
#include <eglplus/renderable_type_bit.hpp>
#include <eglplus/surface_type_bit.hpp>
#include <eglplus/attrib_list.hpp>
#include <eglplus/boolean.hpp>
#include <eglplus/display.hpp>

#include <cassert>

namespace eglplus {

class Config;
::EGLConfig GetEGLHandle(const Config&)
OGLPLUS_NOEXCEPT(true);

/// A wrapper for EGL configuration
class Config
{
private:
	Display _display;
	::EGLConfig _handle;

	friend ::EGLConfig GetEGLHandle(const Config&)
	OGLPLUS_NOEXCEPT(true);
public:
	Config(const Display& display, ::EGLConfig handle)
	 : _display(display)
	 , _handle(handle)
	{ }

	/// Get the value of a specified attribute
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 */
	EGLint GetAttrib(ConfigAttrib attrib) const
	{
		EGLint result = 0;
		EGLPLUS_EGLFUNC(GetConfigAttrib)(
			GetEGLHandle(_display),
			this->_handle,
			EGLint(EGLenum(attrib)),
			&result
		);
		EGLPLUS_VERIFY_SIMPLE(GetConfigAttrib);
		return result;
	}

	/// Get the unique configuration identifier
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{CONFIG_ID}
	 */
	EGLint ConfigId(void) const
	{
		return GetAttrib(ConfigAttrib::ConfigId);
	}

	/// Get the unique configuration identifier
	/** Synonym for ConfigId()
	 *
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{CONFIG_ID}
	 */
	EGLint Id(void) const
	{
		return ConfigId();
	}

	/// Return the total component bits of the color buffer
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{BUFFER_SIZE}
	 */
	EGLint BufferSize(void) const
	{
		return GetAttrib(ConfigAttrib::BufferSize);
	}

	/// Returns the number of bits of the red color component
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{RED_SIZE}
	 */
	EGLint RedSize(void) const
	{
		return GetAttrib(ConfigAttrib::RedSize);
	}

	/// Returns the number of bits of the green color component
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{GREEN_SIZE}
	 */
	EGLint GreenSize(void) const
	{
		return GetAttrib(ConfigAttrib::GreenSize);
	}

	/// Returns the number of bits of the blue color component
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{BLUE_SIZE}
	 */
	EGLint BlueSize(void) const
	{
		return GetAttrib(ConfigAttrib::BlueSize);
	}

	/// Returns the number of bits of the luminance component
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{LUMINANCE_SIZE}
	 */
	EGLint LuminanceSize(void) const
	{
		return GetAttrib(ConfigAttrib::LuminanceSize);
	}

	/// Returns the number of bits of the alpha component
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{ALPHA_SIZE}
	 */
	EGLint AlphaSize(void) const
	{
		return GetAttrib(ConfigAttrib::AlphaSize);
	}

	/// Returns the number of bits of the depth buffer
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{DEPTH_SIZE}
	 */
	EGLint DepthSize(void) const
	{
		return GetAttrib(ConfigAttrib::DepthSize);
	}

	/// Returns the number of bits of the stencil buffer
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{STENCIL_SIZE}
	 */
	EGLint StencilSize(void) const
	{
		return GetAttrib(ConfigAttrib::StencilSize);
	}

	/// Returns the maximum width of Pbuffers in pixels
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{MAX_PBUFFER_WIDTH}
	 */
	EGLint MaxPbufferWidth(void) const
	{
		return GetAttrib(ConfigAttrib::MaxPbufferWidth);
	}

	/// Returns the maximum height of Pbuffers in pixels
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{MAX_PBUFFER_HEIGHT}
	 */
	EGLint MaxPbufferHeight(void) const
	{
		return GetAttrib(ConfigAttrib::MaxPbufferHeight);
	}

	/// Returns the maximum size (number of pixels) of Pbuffers
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{MAX_PBUFFER_PIXELS}
	 */
	EGLint MaxPbufferPixels(void) const
	{
		return GetAttrib(ConfigAttrib::MaxPbufferPixels);
	}

	/// Returns the minimum swap interval
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{MIN_SWAP_INTERVAL}
	 */
	EGLint MinSwapInterval(void) const
	{
		return GetAttrib(ConfigAttrib::MinSwapInterval);
	}

	/// Returns the maximum swap interval
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{MAX_SWAP_INTERVAL}
	 */
	EGLint MaxSwapInterval(void) const
	{
		return GetAttrib(ConfigAttrib::MaxSwapInterval);
	}

	/// Returns true if native rendering API can render to surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{NATIVE_RENDERABLE}
	 */
	Boolean NativeRenderable(void) const
	{
		return Boolean(
			EGLBoolean(GetAttrib(ConfigAttrib::NativeRenderable)),
			std::nothrow
		);
	}

	/// Returns the caveat for this config
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{CONFIG_CAVEAT}
	 */
	eglplus::ConfigCaveat ConfigCaveat(void) const
	{
		return eglplus::ConfigCaveat(
			EGLenum(GetAttrib(ConfigAttrib::ConfigCaveat))
		);
	}

	/// Returns the caveat for this config
	/** Synonym for ConfigCaveat()
	 *
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{CONFIG_CAVEAT}
	 */
	eglplus::ConfigCaveat Caveat(void) const
	{
		return this->ConfigCaveat();
	}

	/// Returns the color buffer type
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{COLOR_BUFFER_TYPE}
	 */
	eglplus::ColorBufferType ColorBufferType(void) const
	{
		return eglplus::ColorBufferType(
			EGLenum(GetAttrib(ConfigAttrib::ColorBufferType))
		);
	}

	/// Returns the transparency type
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{TRANSPARENT_TYPE}
	 */
	eglplus::TransparentType TransparentType(void) const
	{
		return eglplus::TransparentType(
			EGLenum(GetAttrib(ConfigAttrib::TransparentType))
		);
	}

	/// Returns the renderable type bits
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{RENDERABLE_TYPE}
	 */
	Bitfield<eglplus::RenderableTypeBit> RenderableTypes(void) const
	{
		return Bitfield<eglplus::RenderableTypeBit>(
			EGLenum(GetAttrib(ConfigAttrib::RenderableType))
		);
	}

	/// Returns true if a specified renderable type is supported
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{RENDERABLE_TYPE}
	 */
	bool HasRenderableType(eglplus::RenderableTypeBit type) const
	{
		EGLenum a = GetAttrib(ConfigAttrib::RenderableType);
		EGLenum b = EGLenum(type);
		return (a & b) == b;
	}

	/// Returns the surface type bits
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{SURFACE_TYPE}
	 */
	Bitfield<eglplus::SurfaceTypeBit> SurfaceTypes(void) const
	{
		return Bitfield<eglplus::SurfaceTypeBit>(
			EGLenum(GetAttrib(ConfigAttrib::SurfaceType))
		);
	}

	/// Returns true if a specified surface type is supported
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{SURFACE_TYPE}
	 */
	bool HasSurfaceType(eglplus::SurfaceTypeBit type) const
	{
		EGLenum a = GetAttrib(ConfigAttrib::SurfaceType);
		EGLenum b = EGLenum(type);
		return (a & b) == b;
	}

	/// Returns the conformant bits for this config
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{CONFORMANT}
	 */
	Bitfield<eglplus::RenderableTypeBit> Conformant(void) const
	{
		return Bitfield<eglplus::RenderableTypeBit>(
			GetAttrib(ConfigAttrib::Conformant)
		);
	}

	/// Returns true if the config is conformant to a renderable type
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetConfigAttrib}
	 *  @egldefref{CONFORMANT}
	 */
	bool IsConformantTo(eglplus::RenderableTypeBit type) const
	{
		EGLenum a = GetAttrib(ConfigAttrib::Conformant);
		EGLenum b = EGLenum(type);
		return (a & b) == b;
	}
};

inline
::EGLConfig GetEGLHandle(const Config& config)
OGLPLUS_NOEXCEPT(true)
{
	return config._handle;
}

struct ConfigValueTypeToConfigAttrib
{
	static ColorBufferType
	ValueType(std::integral_constant<int, 0>);
	ConfigAttrib operator()(ColorBufferType) const
	{
		return ConfigAttrib::ColorBufferType;
	}

	static ConfigCaveat
	ValueType(std::integral_constant<int, 1>);
	ConfigAttrib operator()(ConfigCaveat) const
	{
		return ConfigAttrib::ConfigCaveat;
	}

	static RenderableTypeBit
	ValueType(std::integral_constant<int, 2>);
	ConfigAttrib operator()(RenderableTypeBit) const
	{
		return ConfigAttrib::RenderableType;
	}

	static SurfaceTypeBit
	ValueType(std::integral_constant<int, 3>);
	ConfigAttrib operator()(SurfaceTypeBit) const
	{
		return ConfigAttrib::SurfaceType;
	}

	static TransparentType
	ValueType(std::integral_constant<int, 4>);
	ConfigAttrib operator()(TransparentType) const
	{
		return ConfigAttrib::TransparentType;
	}

	static std::integral_constant<int, 4> MaxValueType(void);
};

/// Attribute list for configuration attributes
typedef AttributeList<
	ConfigAttrib,
	ConfigValueTypeToConfigAttrib,
	AttributeListTraits
> ConfigAttribs;

/// Finished list of configuration attribute values
typedef FinishedAttributeList<
	ConfigAttrib,
	AttributeListTraits
> FinishedConfigAttribs;

/// A provides access to all configurations of a Display
class Configs
{
private:
	Display _display;
	std::vector<EGLConfig> _configs;

	struct _config_range_conv
	{
		Display _display;

		_config_range_conv(const Display& display)
		 : _display(display)
		{ }

		Config operator()(EGLConfig handle) const
		{
			return Config(_display, handle);
		}
	};

	void _get_all(void);
	void _choose(const FinishedConfigAttribs& attribs);
public:
	/// Gets the configurations for the specified display
	Configs(const Display& display)
	 : _display(display)
	{
		_get_all();
	}

	/// Gets configurations matching the specified attribute values
	Configs(const Display& display, const FinishedConfigAttribs& attribs)
	 : _display(display)
	{
		_choose(attribs);
	}

	typedef aux::ConvIterRange<
		std::vector<EGLConfig>::const_iterator,
		Config,
		_config_range_conv
	> ConfigRange;

	/// Returns a range of all supported configurations
	ConfigRange All(void) const
	{
		return ConfigRange(
			_config_range_conv(_display),
			_configs.begin(),
			_configs.end()
		);
	}

	/// Returns true if there are no matching configs
	bool Empty(void) const
	{
		return _configs.empty();
	}

	/// Returns the first config in this set of configurations
	/**
	 *  @pre !Empty()
	 */
	Config First(void) const
	{
		assert(!Empty());
		return Config(_display, _configs.front());
	}
};

} // namespace eglplus

#if !EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)
#include <eglplus/configs.ipp>
#endif

#endif // include guard
