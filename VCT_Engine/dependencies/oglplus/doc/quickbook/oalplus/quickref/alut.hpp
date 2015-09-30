/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oalplus_alut_1

class ALUtilityToolkit
{
public:
	ALUtilityToolkit(ALUtilityToolkit&&);

	ALUtilityToolkit(bool with_context, int argc, char** argv); /*<
	Initializes the ALUT library, optionally with a context.
	See [alutfunc Init], [alutfunc InitWithoutContext].
	>*/

	ALUtilityToolkit(bool with_context); /*<
	Initializes the ALUT library, optionally with a context.
	See [alutfunc Init], [alutfunc InitWithoutContext].
	>*/

	~ALUtilityToolkit(void); /*<
	Cleans up the ALUT library.
	See [alutfunc Exit].
	>*/

	__Buffer CreateBufferHelloWorld(void) const; /*<
	Create a buffer containing the samples of a [''Hello World'] sound.
	See [alutfunc CreateBufferHelloWorld].
	>*/

	__Buffer CreateBufferFromFile(const __StrCRef& file_path) const; /*<
	Create a buffer containing the samples from a specified sound file.
	See [alutfunc CreateBufferFromFile].
	>*/

	std::vector<ALfloat> LoadMemoryHelloWorldNormalized(
		__DataFormat* data_format,
		ALfloat* frequency
	) const; /*<
	Loads samples of a [''Hello World'] sound into a buffer
	and normalizes the sound samples.
	See [alutfunc LoadMemoryHelloWorld].
	>*/

	std::vector<ALfloat> LoadMemoryFromFileNormalized(
		const __StrCRef& file_path,
		__DataFormat* data_format,
		ALfloat* frequency
	) const; /*<
	Loads samples from a sound file into a buffer
	and normalizes the sound samples.
	See [alutfunc LoadMemoryFromFile].
	>*/

	std::vector<ALubyte> LoadMemoryFromFile(
		const __StrCRef& file_path,
		__DataFormat* data_format,
		ALfloat* frequency
	) const; /*<
	Loads samples from a sound file into a buffer.
	See [alutfunc LoadMemoryFromFile].
	>*/

	void LoadMemoryFromFile(
		std::vector<ALubyte>& raw,
		std::vector<ALfloat>& norm,
		const __StrCRef& file_path,
		__DataFormat* data_format,
		ALfloat* frequency
	) const; /*<
	Loads samples from a sound file into a buffer.
	This version loads both the raw data and the normalized samples
	into two buffers and also returns the format and the sampling
	frequency.
	See [alutfunc LoadMemoryFromFile].
	>*/
};

//]

