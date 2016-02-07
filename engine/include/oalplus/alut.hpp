/**
 *  @file oalplus/alut.hpp
 *  @brief Wrapper around ALUT
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_ALUT_1303201721_HPP
#define OALPLUS_ALUT_1303201721_HPP

#include <oalplus/config.hpp>
#include <oalplus/fwd.hpp>
#include <oalplus/alfunc.hpp>
#include <oalplus/boolean.hpp>
#include <oalplus/string.hpp>
#include <oalplus/error/alut.hpp>

#include <oalplus/data_format.hpp>

#include <oalplus/buffer.hpp>

#include <oalplus/utils/type_tag.hpp>

#include <vector>

namespace oalplus {

/// Wrapper for the ALUT library
class ALUtilityToolkit
{
private:
	bool _initialized;
	ALUtilityToolkit(const ALUtilityToolkit&);

	static bool _initialize(bool with_context, int argc, char** argv)
	{
		Boolean result(
			with_context
			?(OALPLUS_ALUTFUNC(Init)(&argc, argv))
			:(OALPLUS_ALUTFUNC(InitWithoutContext)(&argc, argv)),
			std::nothrow
		);
		OALPLUS_CHECK_SIMPLE_ALUT(Init);
		return result;

	}

	static bool _initialize(bool with_context)
	{
		char buf[2] = {'\0'};
		char* arg = buf;
		return _initialize(with_context, 1, &arg);
	}
public:
	/// Initializes the alut library, optionally with a context
	/**
	 *  @alsymbols
	 *  @alutfunref{Init}
	 *  @alutfunref{InitWithoutContext}
	 */
	ALUtilityToolkit(bool with_context, int argc, char** argv)
	 : _initialized(_initialize(with_context, argc, argv))
	{ }

	/// Initializes the alut library, optionally with a context
	/**
	 *  @alsymbols
	 *  @alutfunref{Init}
	 *  @alutfunref{InitWithoutContext}
	 */
	ALUtilityToolkit(bool with_context)
	 : _initialized(_initialize(with_context))
	{ }

	/// Move construction
	ALUtilityToolkit(ALUtilityToolkit&& tmp)
	 : _initialized(tmp._initialized)
	{
		tmp._initialized = false;
	}

	/// Cleans up the ALUT library
	/**
	 *  @alsymbols
	 *  @alutfunref{Exit}
	 */
	~ALUtilityToolkit(void)
	{
		if(_initialized)
		{
			OALPLUS_ALUTFUNC(Exit)();
		}
	}

private:
	static ALfloat _normalize_sample(ALubyte v)
	{
		return (ALfloat(v)-128.0f)/128.0f;
	}

	static ALfloat _normalize_sample(ALshort v)
	{
		return ALfloat(v)/32768.0f;
	}

	template <typename T>
	void _do_load_mem_norm(
		std::vector<ALfloat>& result,
		TypeTag<T>,
		::ALvoid* raw_data,
		::ALsizei size
	) const
	{
		std::size_t n = size/sizeof(T);
		result.resize(n);
		T* data = static_cast<T*>(raw_data);
		for(std::size_t i=0; i!=n; ++i)
		{
			result[i] = _normalize_sample(data[i]);
		}
	}

	struct _free_on_scope_exit
	{
		::ALvoid* ptr;

		~_free_on_scope_exit(void)
		{
			free(ptr);
		}
	};

	std::vector<ALfloat> _load_mem_norm(
		::ALvoid* raw_data,
		::ALenum format,
		::ALsizei size
	) const
	{
		std::vector<ALfloat> result;

		if(format == AL_FORMAT_MONO8)
		{
			_do_load_mem_norm(
				result,
				TypeTag<ALubyte>(),
				raw_data,
				size
			);
		}
		else if(format == AL_FORMAT_MONO16)
		{
			_do_load_mem_norm(
				result,
				TypeTag<ALshort>(),
				raw_data,
				size
			);
		}
		else if(format == AL_FORMAT_STEREO8)
		{
			_do_load_mem_norm(
				result,
				TypeTag<ALubyte>(),
				raw_data,
				size
			);
		}
		else if(format == AL_FORMAT_STEREO16)
		{
			_do_load_mem_norm(
				result,
				TypeTag<ALshort>(),
				raw_data,
				size
			);
		}

		return std::move(result);
	}

	std::vector<ALubyte> _load_memory(
		::ALvoid* raw_data,
		::ALsizei size
	) const
	{
		::ALubyte* data = reinterpret_cast<ALubyte*>(raw_data);
		return std::vector<ALubyte>(data, data+size);
	}
public:
	/// Create a buffer containing the samples of a 'Hello World' sound
	/**
	 *  @see CreateBufferFromFile
	 *
	 *  @alsymbols
	 *  @alutfunref{CreateBufferHelloWorld}
	 */
	Buffer CreateBufferHelloWorld(void) const
	{
		assert(_initialized);
		ALuint name = OALPLUS_ALUTFUNC(CreateBufferHelloWorld)();
		OALPLUS_VERIFY_SIMPLE_ALUT(CreateBufferHelloWorld);
		return Buffer::FromRawName(BufferName(name));
	}

	/// Create a buffer containing the samples from a specified sound file
	/**
	 *  @see CreateBufferHelloWorld
	 *
	 *  @alsymbols
	 *  @alutfunref{CreateBufferFromFile}
	 */
	Buffer CreateBufferFromFile(const StrCRef& file_path) const
	{
		assert(_initialized);
		ALuint name = OALPLUS_ALUTFUNC(CreateBufferFromFile)(
			file_path.is_nts()?
			file_path.c_str():
			file_path.str().c_str()
		);
		OALPLUS_VERIFY_SIMPLE_ALUT(CreateBufferFromFile);
		return Buffer::FromRawName(BufferName(name));
	}

	/// Loads samples of a 'Hello World' sound into a buffer
	/** This version normalizes the sound samples.
	 *
	 *  @see LoadMemoryFromFileNormalized
	 *
	 *  @alsymbols
	 *  @alutfunref{LoadMemoryHelloWorld}
	 */
	std::vector<ALfloat> LoadMemoryHelloWorldNormalized(
		DataFormat* data_format,
		ALfloat* frequency
	) const
	{
		::ALenum format = 0;
		::ALsizei size = 0;
		::ALvoid* ptr = OALPLUS_ALUTFUNC(LoadMemoryHelloWorld)(
			&format,
			&size,
			frequency
		);
		OALPLUS_CHECK_SIMPLE_ALUT(LoadMemoryHelloWorld);

		_free_on_scope_exit cleaner = { ptr };
		OALPLUS_FAKE_USE(cleaner);

		if(data_format) *data_format = DataFormat(format);

		return _load_mem_norm(ptr, format, size);
	}

	/// Loads samples from a sound file into a buffer
	/** This version normalizes the sound samples.
	 *
	 *  @see LoadMemoryHelloWorldNormalized
	 *  @see LoadMemoryFromFile
	 *
	 *  @alsymbols
	 *  @alutfunref{LoadMemoryFromFile}
	 */
	std::vector<ALfloat> LoadMemoryFromFileNormalized(
		const StrCRef& file_path,
		DataFormat* data_format,
		ALfloat* frequency
	) const
	{
		::ALenum format = 0;
		::ALsizei size = 0;
		::ALvoid* ptr = OALPLUS_ALUTFUNC(LoadMemoryFromFile)(
			file_path.is_nts()?
			file_path.c_str():
			file_path.str().c_str(),
			&format,
			&size,
			frequency
		);
		OALPLUS_CHECK_SIMPLE_ALUT(LoadMemoryFromFile);

		_free_on_scope_exit cleaner = { ptr };
		OALPLUS_FAKE_USE(cleaner);

		if(data_format) *data_format = DataFormat(format);

		return _load_mem_norm(ptr, format, size);
	}

	/// Loads samples from a sound file into a buffer
	/**
	 *  @see LoadMemoryFromFileNormalized
	 *  @see LoadMemoryHelloWorldNormalized
	 *
	 *  @alsymbols
	 *  @alutfunref{LoadMemoryFromFile}
	 */
	std::vector<ALubyte> LoadMemoryFromFile(
		const StrCRef& file_path,
		DataFormat* data_format,
		ALfloat* frequency
	) const
	{
		::ALenum format = 0;
		::ALsizei size = 0;
		::ALvoid* ptr = OALPLUS_ALUTFUNC(LoadMemoryFromFile)(
			file_path.is_nts()?
			file_path.c_str():
			file_path.str().c_str(),
			&format,
			&size,
			frequency
		);
		OALPLUS_CHECK_SIMPLE_ALUT(LoadMemoryFromFile);

		_free_on_scope_exit cleaner = { ptr };
		OALPLUS_FAKE_USE(cleaner);

		if(data_format) *data_format = DataFormat(format);

		return _load_memory(ptr, size);
	}

	/// Loads samples from a sound file into a buffer
	/** This version loads both the raw data and the normalized samples
	 *  into two buffers and also returns the format and the sampling
	 *  frequency.
	 *
	 *  @see LoadMemoryFromFileNormalized
	 *  @see LoadMemoryHelloWorldNormalized
	 *
	 *  @alsymbols
	 *  @alutfunref{LoadMemoryFromFile}
	 */
	void LoadMemoryFromFile(
		std::vector<ALubyte>& raw,
		std::vector<ALfloat>& norm,
		const StrCRef& file_path,
		DataFormat* data_format,
		ALfloat* frequency
	) const
	{
		::ALenum format = 0;
		::ALsizei size = 0;
		::ALvoid* ptr = OALPLUS_ALUTFUNC(LoadMemoryFromFile)(
			file_path.is_nts()?
			file_path.c_str():
			file_path.str().c_str(),
			&format,
			&size,
			frequency
		);
		OALPLUS_CHECK_SIMPLE_ALUT(LoadMemoryFromFile);

		_free_on_scope_exit cleaner = { ptr };
		OALPLUS_FAKE_USE(cleaner);

		if(data_format) *data_format = DataFormat(format);

		raw = _load_memory(ptr, size);
		norm = _load_mem_norm(ptr, format, size);
	}
};

} // namespace oalplus

#endif // include guard
