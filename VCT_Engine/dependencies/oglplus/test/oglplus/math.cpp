/**
 *  .file test/oglplus/math.cpp
 *  .brief Test case for math utilities.
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_math
#include <boost/test/unit_test.hpp>

#include <oglplus/gl.hpp>
#include <oglplus/math/constants.hpp>

BOOST_AUTO_TEST_SUITE(math)

BOOST_AUTO_TEST_CASE(math_Pi)
{
	BOOST_CHECK_CLOSE(oglplus::math::Pi(), 3.14159265, 0.0001);
}

BOOST_AUTO_TEST_CASE(math_2Pi_eq_TwoPi)
{
	BOOST_CHECK(2*oglplus::math::Pi() == oglplus::math::TwoPi());
}

BOOST_AUTO_TEST_CASE(math_2HalfPi_eq_Pi)
{
	BOOST_CHECK(2*oglplus::math::HalfPi() == oglplus::math::Pi());
}

BOOST_AUTO_TEST_CASE(math_4HalfPi_eq_TwoPi)
{
	BOOST_CHECK(4*oglplus::math::HalfPi() == oglplus::math::TwoPi());
}

BOOST_AUTO_TEST_SUITE_END()
