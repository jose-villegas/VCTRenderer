/*
 *  .file advanced/spectra/calculator.cpp
 *  .brief Implements default signal spectrum calculators
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/math/constants.hpp>

#include "calculator.hpp"

#include <cmath>

SpectraFourierMatrixGen::SpectraFourierMatrixGen(std::size_t n, std::size_t)
 : inv_n(1.0/n)
 , inv_sqrt_n(1.0/std::sqrt(double(n)), 0.0)
{ }

std::complex<double> SpectraFourierMatrixGen::operator()(
	std::size_t i,
	std::size_t k,
	std::size_t /*n*/,
	std::size_t /*m*/
) const
{
	const double twopi = oglplus::math::TwoPi();
	typedef std::complex<double> C;
	C x(0.0, -twopi*i*k*inv_n);
	return exp(x)*inv_sqrt_n;
}

extern std::shared_ptr<SpectraCalculator>
SpectraGetDefaultCPUFourierTransf(
	SpectraSharedObjects&,
	std::size_t spectrum_size
);

extern std::shared_ptr<SpectraCalculator>
SpectraGetDefaultGPUFourierTransf(
	SpectraSharedObjects&,
	std::size_t spectrum_size
);


std::shared_ptr<SpectraCalculator>
SpectraGetDefaultSignalTransform(
	SpectraSharedObjects& shared_objects,
	std::size_t spectrum_size
)
{
	try
	{
		return SpectraGetDefaultGPUFourierTransf(
			shared_objects,
			spectrum_size
		);
	}
	catch(...)
	{
		return SpectraGetDefaultCPUFourierTransf(
			shared_objects,
			spectrum_size
		);
	}
}

