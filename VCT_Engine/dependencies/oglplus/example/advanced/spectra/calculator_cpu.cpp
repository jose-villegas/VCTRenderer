/*
 *  .file advanced/spectra/calculator_cpu.cpp
 *  .brief Implements default signal spectrum calculators running on CPU
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "calculator.hpp"

#include <vector>
#include <cassert>

// SpectraNoOpValueTransform
struct SpectraNoOpValueTransform
{
	template <typename T>
	inline T operator()(T v) const
	{
		return v;
	}
};

// SpectraComplexAbsExtract
struct SpectraComplexAbsExtract
{
	template <typename T>
	inline T operator()(std::complex<T> v) const
	{
		return abs(v);
	}
};

// SpectraDefaultMatrixTransf
template <typename ValueType, typename Preprocessor, typename Extractor>
class SpectraDefaultMatrixTransf
 : public SpectraCalculator
{
protected:
	const std::size_t in_size, out_size;
	std::vector<ValueType> mat;
	std::string name;

	Preprocessor preprocess;
	Extractor extract;
public:
	template <typename Generator>
	SpectraDefaultMatrixTransf(
		std::size_t in_sz,
		std::size_t out_sz,
		const std::string& transf_name,
		Generator gen,
		Preprocessor preproc = Preprocessor(),
		Extractor extr = Extractor()
	);

	std::size_t InputSize(void) const;

	std::size_t OutputSize(void) const;

	const char* Name(void) const;

	unsigned MaxConcurrentTransforms(void) const;

	void BeginBatch(void);

	void FinishBatch(void);

	unsigned BeginTransform(
		const float* input,
		std::size_t inbufsize,
		float* output,
		std::size_t outbufsize
	);

	void FinishTransform(
		unsigned tid,
		float* output,
		std::size_t outbufsize
	);
};

// SpectraDefaultMatrixTransf
template <typename ValueType, typename Preprocessor, typename Extractor>
template <typename Generator>
SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
SpectraDefaultMatrixTransf(
	std::size_t in_sz,
	std::size_t out_sz,
	const std::string& transf_name,
	Generator gen,
	Preprocessor preproc,
	Extractor extr
): in_size(in_sz)
 , out_size(out_sz)
 , mat(in_size*out_size)
 , name(transf_name)
 , preprocess(preproc)
 , extract(extr)
{
	auto m = mat.begin();
	for(std::size_t row=0; row!=out_size; ++row)
	{
		for(std::size_t col=0; col!=in_size; ++col)
		{
			*m = gen(col, row, in_size, out_size);
			++m;
		}
	}
	assert(m == mat.end());
}

template <typename ValueType, typename Preprocessor, typename Extractor>
std::size_t SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
InputSize(void) const
{
	return in_size;
}

template <typename ValueType, typename Preprocessor, typename Extractor>
std::size_t SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
OutputSize(void) const
{
	return out_size;
}

template <typename ValueType, typename Preprocessor, typename Extractor>
const char* SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
Name(void) const
{
	return name.c_str();
}

template <typename ValueType, typename Preprocessor, typename Extractor>
unsigned SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
MaxConcurrentTransforms(void) const
{
	return 1;
}

template <typename ValueType, typename Preprocessor, typename Extractor>
void SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
BeginBatch(void)
{
}

template <typename ValueType, typename Preprocessor, typename Extractor>
void SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
FinishBatch(void)
{
}

template <typename ValueType, typename Preprocessor, typename Extractor>
unsigned SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
BeginTransform(
	const float* input,
	std::size_t inbufsize,
	float* output,
	std::size_t outbufsize
)
{
	assert(inbufsize >= in_size);
	assert(outbufsize >= out_size);

	float* o = output;
	for(std::size_t row=0; row!=out_size; ++row)
	{
		const float* i = input;
		auto m = mat.begin()+row*out_size;
		ValueType sum = ValueType(0.0);
		for(std::size_t col=0; col!=in_size; ++col)
		{
			sum += (*m++) * preprocess(ValueType(*i++));
		}
		*o++ = float(extract(sum));
	}
	return 0;
}

template <typename ValueType, typename Preprocessor, typename Extractor>
void SpectraDefaultMatrixTransf<ValueType, Preprocessor, Extractor>::
FinishTransform(
	unsigned tid,
	float* /*output*/,
	std::size_t /*outbufsize*/
)
{
	assert(tid == 0);
}

std::shared_ptr<SpectraCalculator>
SpectraGetDefaultCPUFourierTransf(
	SpectraSharedObjects&,
	std::size_t spectrum_size
)
{
	std::size_t frame_size = spectrum_size*2-1;
	assert(spectrum_size > 2);
	typedef SpectraDefaultMatrixTransf<
		std::complex<double>,
		SpectraNoOpValueTransform,
		SpectraComplexAbsExtract
	> Transf;
	return std::make_shared<Transf>(
		frame_size,
		spectrum_size,
		"Discrete Complex Fourier Transform (CPU)",
		SpectraFourierMatrixGen(frame_size, spectrum_size)
	);
}

