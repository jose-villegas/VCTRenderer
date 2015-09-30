/**
 *  .file test/oglplus/angle.cpp
 *  .brief Test case for Angle class and related functionality.
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_Angle
#include <boost/test/unit_test.hpp>

#include <oglplus/gl.hpp>
#include <oglplus/math/angle.hpp>

BOOST_AUTO_TEST_SUITE(Angle)

BOOST_AUTO_TEST_CASE(Angle_default_construction)
{
	oglplus::Angle<float> af;
	oglplus::Angle<double> ad;
}

BOOST_AUTO_TEST_CASE(Angle_construction)
{
	typedef oglplus::Angle<float> Anglef;
	Anglef a1 = Anglef::Degrees(90);
	Anglef a2 = Anglef::Radians(oglplus::math::HalfPi());
	Anglef a3 = oglplus::Degrees(90);
	Anglef a4 = oglplus::Radians(oglplus::math::HalfPi());
	Anglef a5 = oglplus::RightAngles(1.0f);
	Anglef a6 = oglplus::FullCircles(1.0f);
	Anglef a7 = a6;
}

BOOST_AUTO_TEST_CASE(Angle_value)
{
	typedef oglplus::Angle<float> Anglef;

	Anglef a1 = Anglef::Radians(oglplus::math::HalfPi());
	Anglef a2 = Anglef::Radians(oglplus::math::Pi());
	Anglef a3 = Anglef::Radians(oglplus::math::TwoPi());

	BOOST_CHECK_EQUAL(a1.ValueInDegrees(), 90);
	BOOST_CHECK_EQUAL(a2.ValueInDegrees(), 180);
	BOOST_CHECK_EQUAL(a3.ValueInDegrees(), 360);
	BOOST_CHECK_EQUAL(a1.Value(), float(oglplus::math::HalfPi()));
	BOOST_CHECK_EQUAL(a2.Value(), float(oglplus::math::Pi()));
	BOOST_CHECK_EQUAL(a3.Value(), float(oglplus::math::TwoPi()));

	Anglef a4 = oglplus::RightAngles(1.0f);
	Anglef a5 = oglplus::FullCircles(1.0f);

	BOOST_CHECK_EQUAL(a1.ValueInDegrees(), a4.ValueInDegrees());
	BOOST_CHECK_EQUAL(a3.ValueInDegrees(), a5.ValueInDegrees());
}

BOOST_AUTO_TEST_CASE(Angle_cmp)
{
	typedef oglplus::Angle<float> Anglef;

	Anglef a1 = Anglef::Radians(oglplus::math::HalfPi());
	Anglef a2 = Anglef::Radians(oglplus::math::TwoPi());

	Anglef a3 = oglplus::RightAngles(1.0f);
	Anglef a4 = oglplus::FullCircles(1.0f);

	BOOST_CHECK(a1 == a1);
	BOOST_CHECK(a2 == a2);

	BOOST_CHECK(a1 != a2);
	BOOST_CHECK(a3 != a4);

	BOOST_CHECK(a1 == a3);
	BOOST_CHECK(a2 == a4);
}

BOOST_AUTO_TEST_CASE(Angle_addition)
{
	typedef oglplus::Angle<double> Angled;

	Angled a0;
	Angled a1 = Angled::Radians(oglplus::math::HalfPi());
	Angled a2 = Angled::Radians(oglplus::math::Pi());
	Angled a3 = Angled::Radians(oglplus::math::TwoPi());

	BOOST_CHECK((a0+a0) == a0);
	BOOST_CHECK((a0+a1) == a1);
	BOOST_CHECK((a1+a1) == a2);
	BOOST_CHECK((a2+a2) == a3);
	BOOST_CHECK((a1+a2+a1) == a3);
	BOOST_CHECK((a1+a1+a1+a1) == a3);
}

BOOST_AUTO_TEST_CASE(Angle_subtraction)
{
	typedef oglplus::Angle<double> Angled;

	Angled a0;
	Angled a1 = Angled::Radians(oglplus::math::HalfPi());
	Angled a2 = Angled::Radians(oglplus::math::Pi());
	Angled a3 = Angled::Radians(oglplus::math::TwoPi());

	BOOST_CHECK((a0-a0) == a0);
	BOOST_CHECK((a1-a1) == a0);
	BOOST_CHECK((a1-a0) == a1);
	BOOST_CHECK((a0-a1) ==-a1);
	BOOST_CHECK((a2-a1) == a1);
	BOOST_CHECK((-a1+a2) == a1);
	BOOST_CHECK((a3-a2-a1) == a1);
	BOOST_CHECK((a3-a2-a1-a1) == a0);
	BOOST_CHECK((-a1-a2+a3) == a1);
	BOOST_CHECK((-a1-a1-a1-a1) == -a3);
	BOOST_CHECK(-(-a1-a1-a2) == a3);
}

BOOST_AUTO_TEST_CASE(Angle_multiplication)
{
	typedef oglplus::Angle<double> Angled;

	Angled a0;
	Angled a1 = Angled::Radians(oglplus::math::HalfPi());
	Angled a2 = Angled::Radians(oglplus::math::Pi());
	Angled a3 = Angled::Radians(oglplus::math::TwoPi());

	BOOST_CHECK((2*a0) == a0);
	BOOST_CHECK((0*a1) == a0);
	BOOST_CHECK((2*a1) == a2);
	BOOST_CHECK((2*a2) == a3);
	BOOST_CHECK(a1 == (a2*0.5));
	BOOST_CHECK((4*a1) == (2*a2));
	BOOST_CHECK((2.1*a1) == (a1*2.1));
	BOOST_CHECK((3.0*a1) == (a2*1.5));
	BOOST_CHECK((2.4*a1) == (a2*1.2));
	BOOST_CHECK((4.4*a1) == (a3*1.1));
	BOOST_CHECK((oglplus::math::TwoPi()*a1) == (a2*oglplus::math::Pi()));
}

BOOST_AUTO_TEST_CASE(Angle_division)
{
	typedef oglplus::Angle<double> Angled;

	Angled a0;
	Angled a1 = Angled::Radians(oglplus::math::HalfPi());
	Angled a2 = Angled::Radians(oglplus::math::Pi());
	Angled a3 = Angled::Radians(oglplus::math::TwoPi());

	BOOST_CHECK(a0 == (a0/2));
	BOOST_CHECK(a1 == (a2/2));
	BOOST_CHECK(a2 == (a3/2));
	BOOST_CHECK(a1 == (a3/4));
	BOOST_CHECK((a1/3) == (a2/6));
	BOOST_CHECK((a1/3) == (a3/12));
	BOOST_CHECK((a1/oglplus::math::Pi()) == (a2/oglplus::math::TwoPi()));
}

BOOST_AUTO_TEST_CASE(Angle_arithmetic)
{
	typedef oglplus::Angle<double> Angled;

	Angled a0;
	Angled a1 = Angled::Radians(oglplus::math::HalfPi());
	Angled a2 = Angled::Radians(oglplus::math::Pi());
	Angled a3 = Angled::Radians(oglplus::math::TwoPi());

	BOOST_CHECK((a1+a1+a1) == (3.0*a2)/2.0);
	BOOST_CHECK((a1+a0) == (a1-a0));
	BOOST_CHECK(2.0*(-a1-a2+a3) == a3/2.0);
	BOOST_CHECK((9*a1-4*a2)*3.0+a1 == a3);
}

BOOST_AUTO_TEST_CASE(Angle_sin_cos)
{
	typedef oglplus::Angle<double> Angled;

	Angled a0;
	Angled a1 = Angled::Radians(oglplus::math::HalfPi());
	Angled a2 = Angled::Radians(oglplus::math::Pi());
	Angled a3 = Angled::Radians(oglplus::math::TwoPi());
	double eps = 1e-9;

	BOOST_CHECK_EQUAL(Sin(a0), 0.0);
	BOOST_CHECK_EQUAL(Cos(a0), 1.0);

	BOOST_CHECK_CLOSE(Sin(a1), 1.0, eps);
	BOOST_CHECK_CLOSE(Sin(a2)+1.0, 1.0, eps);
	BOOST_CHECK_CLOSE(Sin(a2)+1.0, Sin(a0)+1.0, eps);
	BOOST_CHECK_CLOSE(Sin(a2+a1), -1.0, eps);

	BOOST_CHECK_CLOSE(Cos(a1)+1.0, 1.0, eps);
	BOOST_CHECK_CLOSE(Cos(a2), -1.0, eps);
	BOOST_CHECK_CLOSE(Cos(a2+a1)+1.0, 1.0, eps);

	BOOST_CHECK_CLOSE(Sin(a1), Sin(100000*a3+a1), eps);
	BOOST_CHECK_CLOSE(Cos(a0), Cos(100000*a3+a0), eps);

	BOOST_CHECK(Sin(a2+a1) == Sin(a3-a1));
	BOOST_CHECK(Cos(a2+a1) == Cos(a3-a1));
}

BOOST_AUTO_TEST_CASE(Angle_sin_cos_2)
{
	typedef oglplus::Angle<double> Angled;
	Angled a3 = Angled::Radians(oglplus::math::TwoPi());

	double eps = 1e-8;
	double d = 1.6181;

	for(unsigned i=0; i!=1000; ++i)
	{
		Angled a = Angled::Radians(i*i*d);
		BOOST_CHECK_CLOSE(Sin(a)+2.0, Sin(a+a3*i)+2.0, eps);
		BOOST_CHECK_CLOSE(Cos(a)+2.0, Cos(a+a3*i)+2.0, eps);
	}
}

BOOST_AUTO_TEST_CASE(Angle_tan)
{
	typedef oglplus::Angle<double> Angled;

	double eps = 1e-9;
	double d = 1.6181;

	for(unsigned i=0; i!=1000; ++i)
	{
		Angled a = Angled::Radians(i*i*d);
		BOOST_CHECK_CLOSE(Sin(a)/Cos(a)+2.0, Tan(a)+2.0, eps);
	}
}

BOOST_AUTO_TEST_CASE(Angle_arc)
{
	using oglplus::ArcSin;
	using oglplus::ArcCos;

	typedef oglplus::Angle<double> Angled;

	double eps = 2;

	for(unsigned i=1; i!=1000; ++i)
	{
		Angled a = Angled::Radians(oglplus::math::HalfPi()/i);
		BOOST_CHECK_CLOSE(a.Value(), ArcSin(Sin(a)).Value(), eps);
		BOOST_CHECK_CLOSE(a.Value(), ArcCos(Cos(a)).Value(), eps);
	}
}

BOOST_AUTO_TEST_SUITE_END()
