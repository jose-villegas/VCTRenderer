/**
 *  .file test/oglplus/quaternion.cpp
 *  .brief Test case for Quaternion class and related functionality.
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_Quaternion
#include <boost/test/unit_test.hpp>

#include <oglplus/gl.hpp>
#include <oglplus/math/quaternion.hpp>

BOOST_AUTO_TEST_SUITE(Quaternion)

BOOST_AUTO_TEST_CASE(Quaternion_construction)
{
	typedef oglplus::Quaternion<float> Quatf;
	Quatf q1(1, 0, 0, 0);
	Quatf q2(oglplus::Vec3f::Unit(0), oglplus::Degrees(45));
	Quatf q3(0, oglplus::Vec3f::Unit(1));
	Quatf q4 = q3;
}

BOOST_AUTO_TEST_CASE(Quaternion_real_imag)
{
	typedef oglplus::Quaternion<float> Quatf;
	Quatf q(1, 2, 3, 4);

	BOOST_CHECK(q.Real() == 1);
	BOOST_CHECK(q.Imag().x() == 2);
	BOOST_CHECK(q.Imag().y() == 3);
	BOOST_CHECK(q.Imag().z() == 4);
}

BOOST_AUTO_TEST_CASE(Quaternion_real_imag_at)
{
	typedef oglplus::Quaternion<float> Quatf;
	Quatf q(1, 2, 3, 4);

	BOOST_CHECK(q.Real() == q.At(0));
	BOOST_CHECK(q.Imag().x() == q.At(1));
	BOOST_CHECK(q.Imag().y() == q.At(2));
	BOOST_CHECK(q.Imag().z() == q.At(3));
}

BOOST_AUTO_TEST_CASE(Quaternion_equality)
{
	typedef oglplus::Quaternion<float> Quatf;
	Quatf q1(1, 0, 0, 0);
	Quatf q2 = q1;
	Quatf q3(1, 2, 3, 4);

	BOOST_CHECK(Equal(q1, q2));
	BOOST_CHECK(Equal(q2, q1));
	BOOST_CHECK(q1 == q2);
	BOOST_CHECK(q2 == q1);

	BOOST_CHECK(!Equal(q1, q3));
	BOOST_CHECK(!Equal(q3, q2));
	BOOST_CHECK(q1 != q3);
	BOOST_CHECK(q3 != q2);
}

BOOST_AUTO_TEST_CASE(Quaternion_dot)
{
	typedef oglplus::Quaternion<float> Quatf;
	Quatf q0(1, 2, 3, 4);
	Quatf q1(1, 0, 0, 0);
	Quatf q2(0, 1, 0, 0);
	Quatf q3(0, 0, 1, 0);
	Quatf q4(0, 0, 0, 1);
	BOOST_CHECK(Dot(q0, q0) == 1*1+2*2+3*3+4*4);
	BOOST_CHECK(Dot(q0, q1) == 1);
	BOOST_CHECK(Dot(q0, q2) == 2);
	BOOST_CHECK(Dot(q0, q3) == 3);
	BOOST_CHECK(Dot(q0, q4) == 4);
	BOOST_CHECK(Dot(q1, q2) == 0);
	BOOST_CHECK(Dot(q2, q3) == 0);
	BOOST_CHECK(Dot(q3, q4) == 0);
	BOOST_CHECK(Dot(q4, q1) == 0);
}

BOOST_AUTO_TEST_CASE(Quaternion_is_degenerate)
{
	typedef oglplus::Quaternion<float> Quatf;
	BOOST_CHECK(Quatf(0, 0, 0, 0).IsDegenerate());
}

BOOST_AUTO_TEST_CASE(Quaternion_is_normal)
{
	typedef oglplus::Quaternion<float> Quatf;
	using oglplus::Vec3f;
	using oglplus::Degrees;
	int deg[5] = {0, 90, 180, 270, 360};
	float eps = 1e-7f;
	for(int i=0; i!=5; ++i)
	{
		BOOST_CHECK(Quatf(Vec3f::Unit(0), Degrees(deg[i])).IsNormal(eps));
		BOOST_CHECK(Quatf(Vec3f::Unit(1), Degrees(deg[i])).IsNormal(eps));
		BOOST_CHECK(Quatf(Vec3f::Unit(2), Degrees(deg[i])).IsNormal(eps));
	}
	for(int i=0; i!=1000; ++i)
	{
		float rdeg = (float(std::rand())/RAND_MAX-0.5f);
		BOOST_CHECK(Quatf(Vec3f::Unit(0), Degrees(rdeg)).IsNormal(eps));
		BOOST_CHECK(Quatf(Vec3f::Unit(1), Degrees(rdeg)).IsNormal(eps));
		BOOST_CHECK(Quatf(Vec3f::Unit(2), Degrees(rdeg)).IsNormal(eps));
	}
}

BOOST_AUTO_TEST_CASE(Quaternion_normalize)
{
	typedef oglplus::Quaternion<double> Quatd;
	double eps = 1e-15;
	for(int i=0; i<1000; ++i)
	{
		Quatd q(
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5)
		);
		if(!q.IsDegenerate())
		{
			q.Normalize();
			BOOST_CHECK(q.IsNormal(eps));
		}
	}
}

BOOST_AUTO_TEST_CASE(Quaternion_conjugate)
{
	typedef oglplus::Quaternion<float> Quatf;
	for(int i=0; i<1000; ++i)
	{
		Quatf q(
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f)
		);
		BOOST_CHECK(q.Real() ==  Conjugate(q).Real());
		BOOST_CHECK(q.Imag() == -Conjugate(q).Imag());
		BOOST_CHECK(Equal(q, ~~q));
	}
}

BOOST_AUTO_TEST_CASE(Quaternion_addition)
{
	typedef oglplus::Quaternion<float> Quatf;
	float eps = 1e-11;
	for(int i=0; i<1000; ++i)
	{
		Quatf q1(
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f)
		);
		Quatf q2(
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f)
		);
		BOOST_CHECK_CLOSE((q1+q2).Real(), (q1.Real()+q2.Real()), eps);
		BOOST_CHECK((q1+q2).Imag() == (q1.Imag()+q2.Imag()));
		BOOST_CHECK((q1+q2) == (q2+q1));
	}
}

BOOST_AUTO_TEST_CASE(Quaternion_multiplication)
{
	typedef oglplus::Quaternion<double> Quatd;
	double eps = 1e-11;
	for(int i=0; i<1000; ++i)
	{
		Quatd q1(
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5)
		);
		Quatd q2(
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5)
		);
		Quatd q3(
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5)
		);
		Quatd q4(
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5),
			(double(std::rand())/RAND_MAX-0.5)
		);
		BOOST_CHECK(Close(q1*q2*q3*q4, (q1*q2)*(q3*q4), eps));
		BOOST_CHECK(Close(q1*q2*q3*q4, q1*(q2*q3)*q4, eps));
	}
}

BOOST_AUTO_TEST_SUITE_END()
