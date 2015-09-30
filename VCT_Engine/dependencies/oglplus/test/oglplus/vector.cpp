/**
 *  .file test/oglplus/vector.cpp
 *  .brief Test case for Vector class and related functionality.
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_Vector
#include <boost/test/unit_test.hpp>

#include <oglplus/gl.hpp>
#include <oglplus/math/vector.hpp>

BOOST_AUTO_TEST_SUITE(Vector)

BOOST_AUTO_TEST_CASE(Vector_default_construction)
{
	oglplus::Vector<float, 2> vec2f;
	oglplus::Vector<float, 3> vec3f;
	oglplus::Vector<float, 4> vec4f;

	oglplus::Vector<double, 2> vec2d;
	oglplus::Vector<double, 3> vec3d;
	oglplus::Vector<double, 4> vec4d;
}

BOOST_AUTO_TEST_CASE(Vector_construction)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	oglplus::Vector<double, 2> vec2d(1.0, 2.0);
	oglplus::Vector<double, 3> vec3d(1.0, 2.0, 3.0);
	oglplus::Vector<double, 4> vec4d(1.0, 2.0, 3.0, 4.0);
}

BOOST_AUTO_TEST_CASE(Vector_construction_2)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(vec2f, 3.0f);
	oglplus::Vector<float, 4> vec4f(vec3f, 4.0f);
	oglplus::Vector<float, 4> vec4f2(vec2f, 3.0f, 4.0f);
}

BOOST_AUTO_TEST_CASE(Vector_construction_3)
{
	float data[4] = {1.0f, 2.0f, 3.0f, 4.0f};
	oglplus::Vector<float, 2> vec2f(data, 2);
	oglplus::Vector<float, 3> vec3f(data, 3);
	oglplus::Vector<float, 4> vec4f(data, 4);
}

BOOST_AUTO_TEST_CASE(Vector_construction_4)
{
	float data_2[2] = {1.0f, 2.0f};
	oglplus::Vector<float, 2> vec2f(data_2);
	float data_3[3] = {1.0f, 2.0f, 3.0f};
	oglplus::Vector<float, 3> vec3f(data_3);
	float data_4[4] = {1.0f, 2.0f, 3.0f, 4.0f};
	oglplus::Vector<float, 4> vec4f(data_4);
}

BOOST_AUTO_TEST_CASE(Vector_construction_5)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	oglplus::Vector<double, 2> vec2d(vec2f);
	oglplus::Vector<double, 3> vec3d(vec3f);
	oglplus::Vector<double, 4> vec4d(vec4f);
}

BOOST_AUTO_TEST_CASE(Vector_construction_6)
{
	oglplus::Vector<double, 2> vec2d(1.0, 2.0);
	oglplus::Vector<double, 3> vec3d(1.0, 2.0, 3.0);
	oglplus::Vector<double, 4> vec4d(1.0, 2.0, 3.0, 4.0);

	oglplus::Vector<float, 2> vec2f(vec2d);
	oglplus::Vector<float, 3> vec3f(vec3d);
	oglplus::Vector<float, 4> vec4f(vec4d);
}

BOOST_AUTO_TEST_CASE(Vector_copy_construction)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	oglplus::Vector<float, 2> vec2f2(vec2f);
	oglplus::Vector<float, 3> vec3f2(vec3f);
	oglplus::Vector<float, 4> vec4f2(vec4f);
}

BOOST_AUTO_TEST_CASE(Vector_assignment)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	oglplus::Vector<float, 2> vec2f2;
	oglplus::Vector<float, 3> vec3f2;
	oglplus::Vector<float, 4> vec4f2;

	vec2f2 = vec2f;
	vec3f2 = vec3f;
	vec4f2 = vec4f;
}

BOOST_AUTO_TEST_CASE(Vector_size)
{
	oglplus::Vector<float, 2> vec2f;
	oglplus::Vector<float, 3> vec3f;
	oglplus::Vector<float, 4> vec4f;

	BOOST_CHECK_EQUAL(vec2f.Size(), 2);
	BOOST_CHECK_EQUAL(vec3f.Size(), 3);
	BOOST_CHECK_EQUAL(vec4f.Size(), 4);

	BOOST_CHECK_EQUAL(Size(vec2f), 2);
	BOOST_CHECK_EQUAL(Size(vec3f), 3);
	BOOST_CHECK_EQUAL(Size(vec4f), 4);
}

BOOST_AUTO_TEST_CASE(Vector_data)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	BOOST_CHECK_EQUAL(vec2f.Data()[0], 1.0f);
	BOOST_CHECK_EQUAL(vec2f.Data()[1], 2.0f);

	BOOST_CHECK_EQUAL(vec3f.Data()[0], 1.0f);
	BOOST_CHECK_EQUAL(vec3f.Data()[1], 2.0f);
	BOOST_CHECK_EQUAL(vec3f.Data()[2], 3.0f);

	BOOST_CHECK_EQUAL(vec4f.Data()[0], 1.0f);
	BOOST_CHECK_EQUAL(vec4f.Data()[1], 2.0f);
	BOOST_CHECK_EQUAL(vec4f.Data()[2], 3.0f);
	BOOST_CHECK_EQUAL(vec4f.Data()[3], 4.0f);
}

BOOST_AUTO_TEST_CASE(Vector_at_1)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	BOOST_CHECK_EQUAL(vec2f.At(0), 1.0f);
	BOOST_CHECK_EQUAL(vec2f.At(1), 2.0f);

	BOOST_CHECK_EQUAL(vec3f.At(0), 1.0f);
	BOOST_CHECK_EQUAL(vec3f.At(1), 2.0f);
	BOOST_CHECK_EQUAL(vec3f.At(2), 3.0f);

	BOOST_CHECK_EQUAL(vec4f.At(0), 1.0f);
	BOOST_CHECK_EQUAL(vec4f.At(1), 2.0f);
	BOOST_CHECK_EQUAL(vec4f.At(2), 3.0f);
	BOOST_CHECK_EQUAL(vec4f.At(3), 4.0f);
}

BOOST_AUTO_TEST_CASE(Vector_at_2)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	BOOST_CHECK_EQUAL(vec2f.At(0, 5.0f), 1.0f);
	BOOST_CHECK_EQUAL(vec2f.At(1, 5.0f), 2.0f);
	BOOST_CHECK_EQUAL(vec2f.At(2, 5.0f), 5.0f);

	BOOST_CHECK_EQUAL(vec3f.At(0, 5.0f), 1.0f);
	BOOST_CHECK_EQUAL(vec3f.At(1, 5.0f), 2.0f);
	BOOST_CHECK_EQUAL(vec3f.At(2, 5.0f), 3.0f);
	BOOST_CHECK_EQUAL(vec3f.At(3, 5.0f), 5.0f);

	BOOST_CHECK_EQUAL(vec4f.At(0, 5.0f), 1.0f);
	BOOST_CHECK_EQUAL(vec4f.At(1, 5.0f), 2.0f);
	BOOST_CHECK_EQUAL(vec4f.At(2, 5.0f), 3.0f);
	BOOST_CHECK_EQUAL(vec4f.At(3, 5.0f), 4.0f);
	BOOST_CHECK_EQUAL(vec4f.At(4, 5.0f), 5.0f);
}

BOOST_AUTO_TEST_CASE(Vector_subscript)
{
	oglplus::Vector<float, 2> vec2f(1.0f, 2.0f);
	oglplus::Vector<float, 3> vec3f(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 4> vec4f(1.0f, 2.0f, 3.0f, 4.0f);

	BOOST_CHECK_EQUAL(vec2f[0], 1.0f);
	BOOST_CHECK_EQUAL(vec2f[1], 2.0f);

	BOOST_CHECK_EQUAL(vec3f[0], 1.0f);
	BOOST_CHECK_EQUAL(vec3f[1], 2.0f);
	BOOST_CHECK_EQUAL(vec3f[2], 3.0f);

	BOOST_CHECK_EQUAL(vec4f[0], 1.0f);
	BOOST_CHECK_EQUAL(vec4f[1], 2.0f);
	BOOST_CHECK_EQUAL(vec4f[2], 3.0f);
	BOOST_CHECK_EQUAL(vec4f[3], 4.0f);

	vec2f[0] = 10.0f;
	vec2f[1] = 20.0f;

	vec3f[0] = 10.0f;
	vec3f[1] = 20.0f;
	vec3f[2] = 30.0f;

	vec4f[0] = 10.0f;
	vec4f[1] = 20.0f;
	vec4f[2] = 30.0f;
	vec4f[3] = 40.0f;

	BOOST_CHECK_EQUAL(vec2f[0], 10.0f);
	BOOST_CHECK_EQUAL(vec2f[1], 20.0f);

	BOOST_CHECK_EQUAL(vec3f[0], 10.0f);
	BOOST_CHECK_EQUAL(vec3f[1], 20.0f);
	BOOST_CHECK_EQUAL(vec3f[2], 30.0f);

	BOOST_CHECK_EQUAL(vec4f[0], 10.0f);
	BOOST_CHECK_EQUAL(vec4f[1], 20.0f);
	BOOST_CHECK_EQUAL(vec4f[2], 30.0f);
	BOOST_CHECK_EQUAL(vec4f[3], 40.0f);
}

BOOST_AUTO_TEST_CASE(Vector_comparison)
{
	oglplus::Vector<float, 4> v1(1.0f, 2.0f, 3.0f, 4.0f);
	oglplus::Vector<float, 4> v2(2.0f, 3.0f, 4.0f, 5.0f);
	oglplus::Vector<float, 4> v3(v1);

	BOOST_CHECK(Equal(v1, v1));
	BOOST_CHECK(Equal(v2, v2));
	BOOST_CHECK(Equal(v1, v3));
	BOOST_CHECK((v1 == v3));
	BOOST_CHECK(!(v1 != v3));

	BOOST_CHECK(!Equal(v1, v2));
	BOOST_CHECK(!(v1 == v2));
	BOOST_CHECK((v1 != v2));

	BOOST_CHECK(Equal(v1, v2) == Equal(v2, v1));
	BOOST_CHECK(Equal(v1, v3) == Equal(v3, v1));

	BOOST_CHECK((v1 == v2) == (v2 == v1));
	BOOST_CHECK((v1 == v3) == (v3 == v1));

	BOOST_CHECK(Equal(v1, v2) == (v1 == v2));
	BOOST_CHECK(Equal(v1, v3) == (v1 == v3));

	BOOST_CHECK((v1 != v2) == (v2 != v1));
	BOOST_CHECK((v1 != v3) == (v3 != v1));

	BOOST_CHECK(!Equal(v1, v2) == (v1 != v2));
	BOOST_CHECK(!Equal(v1, v3) == (v1 != v3));
}

BOOST_AUTO_TEST_CASE(Vector_unit_1)
{
	BOOST_CHECK(Equal(
		oglplus::Vec1f::Unit(0),
		oglplus::Vec1f(1.f)
	));
}

BOOST_AUTO_TEST_CASE(Vector_unit_2)
{
	BOOST_CHECK(Equal(oglplus::Vec2f::Unit(0), oglplus::Vec2f(1.f, 0.f)));
	BOOST_CHECK(Equal(oglplus::Vec2f::Unit(1), oglplus::Vec2f(0.f, 1.f)));
}

BOOST_AUTO_TEST_CASE(Vector_unit_3)
{
	BOOST_CHECK(Equal(
		oglplus::Vec3f::Unit(0),
		oglplus::Vec3f(1.f, 0.f, 0.f)
	));
	BOOST_CHECK(Equal(
		oglplus::Vec3f::Unit(1),
		oglplus::Vec3f(0.f, 1.f, 0.f)
	));
	BOOST_CHECK(Equal(
		oglplus::Vec3f::Unit(2),
		oglplus::Vec3f(0.f, 0.f, 1.f)
	));
}

BOOST_AUTO_TEST_CASE(Vector_unit_4)
{
	BOOST_CHECK(Equal(
		oglplus::Vec4f::Unit(0),
		oglplus::Vec4f(1.f, 0.f, 0.f, 0.f)
	));
	BOOST_CHECK(Equal(
		oglplus::Vec4f::Unit(1),
		oglplus::Vec4f(0.f, 1.f, 0.f, 0.f)
	));
	BOOST_CHECK(Equal(
		oglplus::Vec4f::Unit(2),
		oglplus::Vec4f(0.f, 0.f, 1.f, 0.f)
	));
	BOOST_CHECK(Equal(
		oglplus::Vec4f::Unit(3),
		oglplus::Vec4f(0.f, 0.f, 0.f, 1.f)
	));
}

BOOST_AUTO_TEST_CASE(Vector_negation_2)
{
	oglplus::Vector<float, 2> v1( 1.0f, 2.0f);
	oglplus::Vector<float, 2> v2(-1.0f,-2.0f);

	BOOST_CHECK(v1 ==-v2);
	BOOST_CHECK(v2 ==-v1);
	BOOST_CHECK(v1 ==-(-v1));
	BOOST_CHECK(v2 ==-(-v2));
}

BOOST_AUTO_TEST_CASE(Vector_negation_3)
{
	oglplus::Vector<float, 3> v1( 1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 3> v2(-1.0f,-2.0f,-3.0f);

	BOOST_CHECK(v1 ==-v2);
	BOOST_CHECK(v2 ==-v1);
	BOOST_CHECK(v1 ==-(-v1));
	BOOST_CHECK(v2 ==-(-v2));
}

BOOST_AUTO_TEST_CASE(Vector_negation_4)
{
	oglplus::Vector<float, 4> v1( 1.0f, 2.0f, 3.0f, 4.0f);
	oglplus::Vector<float, 4> v2(-1.0f,-2.0f,-3.0f,-4.0f);

	BOOST_CHECK(v1 ==-v2);
	BOOST_CHECK(v2 ==-v1);
	BOOST_CHECK(v1 ==-(-v1));
	BOOST_CHECK(v2 ==-(-v2));
}

BOOST_AUTO_TEST_CASE(Vector_addition_2)
{
	oglplus::Vector<float, 2> v0(0.0f, 0.0f);
	oglplus::Vector<float, 2> v1(1.0f, 2.0f);
	oglplus::Vector<float, 2> v2(2.0f, 4.0f);
	oglplus::Vector<float, 2> v3(v1+v2);

	BOOST_CHECK((v1+v0) == v1);
	BOOST_CHECK((v1+v1) == v2);
	BOOST_CHECK((v1+v1+v1) == v3);
	BOOST_CHECK((v1+v2) == (v2+v1));
	BOOST_CHECK((v1+v2+v3) == (v3+v2+v1));
}

BOOST_AUTO_TEST_CASE(Vector_addition_3)
{
	oglplus::Vector<float, 3> v0(0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 3> v1(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 3> v2(2.0f, 4.0f, 6.0f);
	oglplus::Vector<float, 3> v3(v1+v2);

	BOOST_CHECK((v1+v0) == v1);
	BOOST_CHECK((v1+v1) == v2);
	BOOST_CHECK((v1+v1+v1) == v3);
	BOOST_CHECK((v1+v2) == (v2+v1));
	BOOST_CHECK((v1+v2+v3) == (v3+v2+v1));
}

BOOST_AUTO_TEST_CASE(Vector_addition_4)
{
	oglplus::Vector<float, 4> v0(0.0f, 0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 4> v1(1.0f, 2.0f, 3.0f, 4.0f);
	oglplus::Vector<float, 4> v2(2.0f, 4.0f, 6.0f, 8.0f);
	oglplus::Vector<float, 4> v3(v1+v2);

	BOOST_CHECK((v1+v0) == v1);
	BOOST_CHECK((v1+v1) == v2);
	BOOST_CHECK((v1+v1+v1) == v3);
	BOOST_CHECK((v1+v2) == (v2+v1));
	BOOST_CHECK((v1+v2+v3) == (v3+v2+v1));
}

BOOST_AUTO_TEST_CASE(Vector_subtraction_2)
{
	oglplus::Vector<float, 2> v0(0.0f, 0.0f);
	oglplus::Vector<float, 2> v1(1.0f, 2.0f);
	oglplus::Vector<float, 2> v2(2.0f, 4.0f);
	oglplus::Vector<float, 2> v3(v1+v2);

	BOOST_CHECK((v1-v0) == v1);
	BOOST_CHECK((v2-v2) == v0);
	BOOST_CHECK((-v1-v1-v1) == -v3);
	BOOST_CHECK((v1-v2) == -(v2-v1));
	BOOST_CHECK((v3-v2-v1) == v0);
}

BOOST_AUTO_TEST_CASE(Vector_subtraction_3)
{
	oglplus::Vector<float, 3> v0(0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 3> v1(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 3> v2(2.0f, 4.0f, 6.0f);
	oglplus::Vector<float, 3> v3(v1+v2);

	BOOST_CHECK((v1-v0) == v1);
	BOOST_CHECK((v2-v2) == v0);
	BOOST_CHECK((-v1-v1-v1) == -v3);
	BOOST_CHECK((v1-v2) == -(v2-v1));
	BOOST_CHECK((v3-v2-v1) == v0);
}

BOOST_AUTO_TEST_CASE(Vector_subtraction_4)
{
	oglplus::Vector<float, 4> v0(0.0f, 0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 4> v1(1.0f, 2.0f, 3.0f, 4.0f);
	oglplus::Vector<float, 4> v2(2.0f, 4.0f, 6.0f, 8.0f);
	oglplus::Vector<float, 4> v3(v1+v2);

	BOOST_CHECK((v1-v0) == v1);
	BOOST_CHECK((v2-v2) == v0);
	BOOST_CHECK((-v1-v1-v1) == -v3);
	BOOST_CHECK((v1-v2) == -(v2-v1));
	BOOST_CHECK((v3-v2-v1) == v0);
}

BOOST_AUTO_TEST_CASE(Vector_multiplication_2)
{
	oglplus::Vector<float, 2> v0(0.0f, 0.0f);
	oglplus::Vector<float, 2> v1(1.0f, 2.0f);
	oglplus::Vector<float, 2> v2(2.0f, 4.0f);

	BOOST_CHECK(v0*10 == v0);
	BOOST_CHECK(v1*10 == 10*v1);
	BOOST_CHECK(v1*22 == v2*11);
	BOOST_CHECK(6*v1 == 3*v2);
}

BOOST_AUTO_TEST_CASE(Vector_multiplication_3)
{
	oglplus::Vector<float, 3> v0(0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 3> v1(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 3> v2(2.0f, 4.0f, 6.0f);

	BOOST_CHECK(v0*10 == v0);
	BOOST_CHECK(v1*10 == 10*v1);
	BOOST_CHECK(v1*22 == v2*11);
	BOOST_CHECK(6*v1 == 3*v2);
}

BOOST_AUTO_TEST_CASE(Vector_multiplication_4)
{
	oglplus::Vector<float, 4> v0(0.0f, 0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 4> v1(1.0f, 2.0f, 3.0f, 4.0f);
	oglplus::Vector<float, 4> v2(2.0f, 4.0f, 6.0f, 8.0f);

	BOOST_CHECK(v0*10 == v0);
	BOOST_CHECK(v1*10 == 10*v1);
	BOOST_CHECK(v1*22 == v2*11);
	BOOST_CHECK(6*v1 == 3*v2);
}

BOOST_AUTO_TEST_CASE(Vector_division_2)
{
	oglplus::Vector<float, 2> v0(0.0f, 0.0f);
	oglplus::Vector<float, 2> v1(1.0f, 2.0f);
	oglplus::Vector<float, 2> v2(2.0f, 4.0f);

	BOOST_CHECK(v0/10.0f == v0);
	BOOST_CHECK(v1*0.5f == v1/2.0f);
	BOOST_CHECK(v1 == v2/2.0f);
}

BOOST_AUTO_TEST_CASE(Vector_division_3)
{
	oglplus::Vector<float, 3> v0(0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 3> v1(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 3> v2(2.0f, 4.0f, 6.0f);

	BOOST_CHECK(v0/10.0f == v0);
	BOOST_CHECK(v1*0.5f == v1/2.0f);
	BOOST_CHECK(v1 == v2/2.0f);
}

BOOST_AUTO_TEST_CASE(Vector_division_4)
{
	oglplus::Vector<float, 4> v0(0.0f, 0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 4> v1(1.0f, 2.0f, 3.0f, 4.0f);
	oglplus::Vector<float, 4> v2(2.0f, 4.0f, 6.0f, 8.0f);

	BOOST_CHECK(v0/10.0f == v0);
	BOOST_CHECK(v1*0.5f == v1/2.0f);
	BOOST_CHECK(v1 == v2/2.0f);
}

BOOST_AUTO_TEST_CASE(Vector_dot_2)
{
	oglplus::Vector<float, 2> v0(0.0f, 0.0f);
	oglplus::Vector<float, 2> v1(1.0f, 1.0f);
	oglplus::Vector<float, 2> v2(1.0f, 2.0f);
	oglplus::Vector<float, 2> v3(1.0f, 0.0f);
	oglplus::Vector<float, 2> v4(0.0f, 1.0f);

	BOOST_CHECK_EQUAL(Dot(v0, v1), 0.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v1), 2.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v2), 3.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v3), 1.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v4), 1.0f);
	BOOST_CHECK_EQUAL(Dot(v2, v3), 1.0f);
	BOOST_CHECK_EQUAL(Dot(v2, v4), 2.0f);
	BOOST_CHECK_EQUAL(Dot(v3, v4), 0.0f);
}

BOOST_AUTO_TEST_CASE(Vector_dot_3)
{
	oglplus::Vector<float, 3> v0(0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 3> v1(1.0f, 1.0f, 1.0f);
	oglplus::Vector<float, 3> v2(1.0f, 2.0f, 3.0f);
	oglplus::Vector<float, 3> v3(1.0f, 0.0f, 1.0f);
	oglplus::Vector<float, 3> v4(0.0f, 1.0f, 0.0f);

	BOOST_CHECK_EQUAL(Dot(v0, v1), 0.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v1), 3.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v2), 6.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v3), 2.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v4), 1.0f);
	BOOST_CHECK_EQUAL(Dot(v2, v3), 4.0f);
	BOOST_CHECK_EQUAL(Dot(v2, v4), 2.0f);
	BOOST_CHECK_EQUAL(Dot(v3, v4), 0.0f);
}

BOOST_AUTO_TEST_CASE(Vector_dot_4)
{
	oglplus::Vector<float, 4> v0(0.0f, 0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 4> v1(1.0f, 1.0f, 1.0f, 1.0f);
	oglplus::Vector<float, 4> v2(1.0f, 2.0f, 3.0f, 4.0f);
	oglplus::Vector<float, 4> v3(1.0f, 0.0f, 1.0f, 0.0f);
	oglplus::Vector<float, 4> v4(0.0f, 1.0f, 0.0f, 1.0f);

	BOOST_CHECK_EQUAL(Dot(v0, v1), 0.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v1), 4.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v2), 10.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v3), 2.0f);
	BOOST_CHECK_EQUAL(Dot(v1, v4), 2.0f);
	BOOST_CHECK_EQUAL(Dot(v2, v3), 4.0f);
	BOOST_CHECK_EQUAL(Dot(v2, v4), 6.0f);
	BOOST_CHECK_EQUAL(Dot(v3, v4), 0.0f);
}

BOOST_AUTO_TEST_CASE(Vector_perpendicular)
{
	float eps = 1e-4;

	oglplus::Vector<float, 2> v2x(1.0f, 0.0f);
	oglplus::Vector<float, 2> v2y(0.0f, 1.0f);

	BOOST_CHECK(Perpendicular(v2x) == v2y);
	BOOST_CHECK(Perpendicular(v2y) ==-v2x);

	for(unsigned i=0; i!=1000; ++i)
	{
		oglplus::Vector<float, 2> v1(
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f)
		);
		oglplus::Vector<float, 2> v2 = Perpendicular(v1);
		BOOST_CHECK_CLOSE(Dot(v1, v2)+2.0, 2.0, eps);
	}
}

BOOST_AUTO_TEST_CASE(Vector_cross)
{
	float eps = 1e-4;

	oglplus::Vector<float, 3> v3x(1.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 3> v3y(0.0f, 1.0f, 0.0f);
	oglplus::Vector<float, 3> v3z(0.0f, 0.0f, 1.0f);

	BOOST_CHECK(Cross(v3x, v3y) == v3z);
	BOOST_CHECK(Cross(v3z, v3x) == v3y);
	BOOST_CHECK(Cross(v3y, v3z) == v3x);

	for(unsigned i=0; i!=1000; ++i)
	{
		oglplus::Vector<float, 2> v21(
			(float(std::rand())/RAND_MAX-0.5f),
			(float(std::rand())/RAND_MAX-0.5f)
		);
		oglplus::Vector<float, 2> v22 = Perpendicular(v21);

		oglplus::Vector<float, 3> v1(v21, 0.0);
		oglplus::Vector<float, 3> v2(v22, 0.0);
		oglplus::Vector<float, 3> v3 = Cross(v1, v2);

		BOOST_CHECK_CLOSE(Dot(v1, v2)+2.0, 2.0, eps);
		BOOST_CHECK_CLOSE(Dot(v1, v3)+2.0, 2.0, eps);
		BOOST_CHECK_CLOSE(Dot(v2, v3)+2.0, 2.0, eps);
	}
}

BOOST_AUTO_TEST_CASE(Vector_normalized)
{
	float eps = 1e-4;

	for(unsigned i=0; i!=1000; ++i)
	{
		oglplus::Vector<float, 2> v2(
			(float(std::rand())/RAND_MAX-0.5f)*std::rand(),
			(float(std::rand())/RAND_MAX-0.5f)*std::rand()
		);
		oglplus::Vector<float, 2> v2n = Normalized(v2);
		BOOST_CHECK_CLOSE(Dot(v2n, v2n), 1.0, eps);

		oglplus::Vector<float, 3> v3(
			(float(std::rand())/RAND_MAX-0.5f)*std::rand(),
			(float(std::rand())/RAND_MAX-0.5f)*std::rand(),
			(float(std::rand())/RAND_MAX-0.5f)*std::rand()
		);
		oglplus::Vector<float, 3> v3n = Normalized(v3);
		BOOST_CHECK_CLOSE(Dot(v3n, v3n), 1.0, eps);

		oglplus::Vector<float, 4> v4(
			(float(std::rand())/RAND_MAX-0.5f)*std::rand(),
			(float(std::rand())/RAND_MAX-0.5f)*std::rand(),
			(float(std::rand())/RAND_MAX-0.5f)*std::rand(),
			(float(std::rand())/RAND_MAX-0.5f)*std::rand()
		);
		oglplus::Vector<float, 4> v4n = Normalized(v4);
		BOOST_CHECK_CLOSE(Dot(v4n, v4n), 1.0, eps);
	}
}

BOOST_AUTO_TEST_CASE(Vector_length_4)
{
	oglplus::Vector<float, 4> v0(0.0f, 0.0f, 0.0f, 0.0f);
	oglplus::Vector<float, 4> v1(1.0f, 1.0f, 1.0f, 1.0f);

	BOOST_CHECK_EQUAL(Length(v0), 0.0f);
	BOOST_CHECK_EQUAL(Length(v1), 2.0f);
}

BOOST_AUTO_TEST_CASE(Vector_swizzle_1)
{
	oglplus::Vector<float, 2> v2(0.0f, 1.0f);
	oglplus::Vector<float, 3> v3(0.0f, 1.0f, 2.0f);
	oglplus::Vector<float, 4> v4(0.0f, 1.0f, 2.0f, 3.0f);

	BOOST_CHECK_EQUAL(Swizzle(v2).x(), 0.0f);
	BOOST_CHECK_EQUAL(Swizzle(v2).y(), 1.0f);

	BOOST_CHECK_EQUAL(Swizzle(v3).x(), 0.0f);
	BOOST_CHECK_EQUAL(Swizzle(v3).y(), 1.0f);
	BOOST_CHECK_EQUAL(Swizzle(v3).z(), 2.0f);

	BOOST_CHECK_EQUAL(Swizzle(v4).x(), 0.0f);
	BOOST_CHECK_EQUAL(Swizzle(v4).y(), 1.0f);
	BOOST_CHECK_EQUAL(Swizzle(v4).z(), 2.0f);
	BOOST_CHECK_EQUAL(Swizzle(v4).w(), 3.0f);
}

template <typename T, std::size_t N>
void do_test_swizzle(
	const oglplus::Vector<T, N>& v,
	T c0 = T(0),
	T c1 = T(0),
	T c2 = T(0),
	T c3 = T(0)
)
{
	const T c[4] = {c0, c1, c2, c3};
	for(std::size_t i=0; i!=N; ++i)
	{
		BOOST_CHECK_EQUAL(v.At(i), c[i]);
	}
}

BOOST_AUTO_TEST_CASE(Vector_swizzle_2)
{
	float x = 1.2f, y = 3.4f;
	oglplus::Vector<float, 2> v(x, y);

	do_test_swizzle(Swizzle(v).xx(), x,x);
	do_test_swizzle(Swizzle(v).xy(), x,y);
	do_test_swizzle(Swizzle(v).yx(), y,x);
	do_test_swizzle(Swizzle(v).yy(), y,y);
}

BOOST_AUTO_TEST_CASE(Vector_swizzle_3)
{
	float x = 1.2f, y = 3.4f, z = 5.6f;
	oglplus::Vector<float, 3> v(x, y, z);

	do_test_swizzle(Swizzle(v).xxx(), x,x,x);
	do_test_swizzle(Swizzle(v).xxy(), x,x,y);
	do_test_swizzle(Swizzle(v).xxz(), x,x,z);
	do_test_swizzle(Swizzle(v).xyx(), x,y,x);
	do_test_swizzle(Swizzle(v).xyy(), x,y,y);
	do_test_swizzle(Swizzle(v).xyz(), x,y,z);
	do_test_swizzle(Swizzle(v).xzx(), x,z,x);
	do_test_swizzle(Swizzle(v).xzy(), x,z,y);
	do_test_swizzle(Swizzle(v).xzz(), x,z,z);

	do_test_swizzle(Swizzle(v).yxx(), y,x,x);
	do_test_swizzle(Swizzle(v).yxy(), y,x,y);
	do_test_swizzle(Swizzle(v).yxz(), y,x,z);
	do_test_swizzle(Swizzle(v).yyx(), y,y,x);
	do_test_swizzle(Swizzle(v).yyy(), y,y,y);
	do_test_swizzle(Swizzle(v).yyz(), y,y,z);
	do_test_swizzle(Swizzle(v).yzx(), y,z,x);
	do_test_swizzle(Swizzle(v).yzy(), y,z,y);
	do_test_swizzle(Swizzle(v).yzz(), y,z,z);

	do_test_swizzle(Swizzle(v).zxx(), z,x,x);
	do_test_swizzle(Swizzle(v).zxy(), z,x,y);
	do_test_swizzle(Swizzle(v).zxz(), z,x,z);
	do_test_swizzle(Swizzle(v).zyx(), z,y,x);
	do_test_swizzle(Swizzle(v).zyy(), z,y,y);
	do_test_swizzle(Swizzle(v).zyz(), z,y,z);
	do_test_swizzle(Swizzle(v).zzx(), z,z,x);
	do_test_swizzle(Swizzle(v).zzy(), z,z,y);
	do_test_swizzle(Swizzle(v).zzz(), z,z,z);
}

BOOST_AUTO_TEST_CASE(Vector_swizzle_4)
{
	float x = 1.2f, y = 3.4f, z = 5.6f, w = 7.8;
	oglplus::Vector<float, 4> v(x, y, z, w);


	do_test_swizzle(Swizzle(v).xxxx(), x,x,x,x);
	do_test_swizzle(Swizzle(v).xxxy(), x,x,x,y);
	do_test_swizzle(Swizzle(v).xxxz(), x,x,x,z);
	do_test_swizzle(Swizzle(v).xxxw(), x,x,x,w);
	do_test_swizzle(Swizzle(v).xxyx(), x,x,y,x);
	do_test_swizzle(Swizzle(v).xxyy(), x,x,y,y);
	do_test_swizzle(Swizzle(v).xxyz(), x,x,y,z);
	do_test_swizzle(Swizzle(v).xxyw(), x,x,y,w);
	do_test_swizzle(Swizzle(v).xxzx(), x,x,z,x);
	do_test_swizzle(Swizzle(v).xxzy(), x,x,z,y);
	do_test_swizzle(Swizzle(v).xxzz(), x,x,z,z);
	do_test_swizzle(Swizzle(v).xxzw(), x,x,z,w);
	do_test_swizzle(Swizzle(v).xxwx(), x,x,w,x);
	do_test_swizzle(Swizzle(v).xxwy(), x,x,w,y);
	do_test_swizzle(Swizzle(v).xxwz(), x,x,w,z);
	do_test_swizzle(Swizzle(v).xxww(), x,x,w,w);

	do_test_swizzle(Swizzle(v).xyxx(), x,y,x,x);
	do_test_swizzle(Swizzle(v).xyxy(), x,y,x,y);
	do_test_swizzle(Swizzle(v).xyxz(), x,y,x,z);
	do_test_swizzle(Swizzle(v).xyxw(), x,y,x,w);
	do_test_swizzle(Swizzle(v).xyyx(), x,y,y,x);
	do_test_swizzle(Swizzle(v).xyyy(), x,y,y,y);
	do_test_swizzle(Swizzle(v).xyyz(), x,y,y,z);
	do_test_swizzle(Swizzle(v).xyyw(), x,y,y,w);
	do_test_swizzle(Swizzle(v).xyzx(), x,y,z,x);
	do_test_swizzle(Swizzle(v).xyzy(), x,y,z,y);
	do_test_swizzle(Swizzle(v).xyzz(), x,y,z,z);
	do_test_swizzle(Swizzle(v).xyzw(), x,y,z,w);
	do_test_swizzle(Swizzle(v).xywx(), x,y,w,x);
	do_test_swizzle(Swizzle(v).xywy(), x,y,w,y);
	do_test_swizzle(Swizzle(v).xywz(), x,y,w,z);
	do_test_swizzle(Swizzle(v).xyww(), x,y,w,w);

	do_test_swizzle(Swizzle(v).xzxx(), x,z,x,x);
	do_test_swizzle(Swizzle(v).xzxy(), x,z,x,y);
	do_test_swizzle(Swizzle(v).xzxz(), x,z,x,z);
	do_test_swizzle(Swizzle(v).xzxw(), x,z,x,w);
	do_test_swizzle(Swizzle(v).xzyx(), x,z,y,x);
	do_test_swizzle(Swizzle(v).xzyy(), x,z,y,y);
	do_test_swizzle(Swizzle(v).xzyz(), x,z,y,z);
	do_test_swizzle(Swizzle(v).xzyw(), x,z,y,w);
	do_test_swizzle(Swizzle(v).xzzx(), x,z,z,x);
	do_test_swizzle(Swizzle(v).xzzy(), x,z,z,y);
	do_test_swizzle(Swizzle(v).xzzz(), x,z,z,z);
	do_test_swizzle(Swizzle(v).xzzw(), x,z,z,w);
	do_test_swizzle(Swizzle(v).xzwx(), x,z,w,x);
	do_test_swizzle(Swizzle(v).xzwy(), x,z,w,y);
	do_test_swizzle(Swizzle(v).xzwz(), x,z,w,z);
	do_test_swizzle(Swizzle(v).xzww(), x,z,w,w);

	do_test_swizzle(Swizzle(v).xwxx(), x,w,x,x);
	do_test_swizzle(Swizzle(v).xwxy(), x,w,x,y);
	do_test_swizzle(Swizzle(v).xwxz(), x,w,x,z);
	do_test_swizzle(Swizzle(v).xwxw(), x,w,x,w);
	do_test_swizzle(Swizzle(v).xwyx(), x,w,y,x);
	do_test_swizzle(Swizzle(v).xwyy(), x,w,y,y);
	do_test_swizzle(Swizzle(v).xwyz(), x,w,y,z);
	do_test_swizzle(Swizzle(v).xwyw(), x,w,y,w);
	do_test_swizzle(Swizzle(v).xwzx(), x,w,z,x);
	do_test_swizzle(Swizzle(v).xwzy(), x,w,z,y);
	do_test_swizzle(Swizzle(v).xwzz(), x,w,z,z);
	do_test_swizzle(Swizzle(v).xwzw(), x,w,z,w);
	do_test_swizzle(Swizzle(v).xwwx(), x,w,w,x);
	do_test_swizzle(Swizzle(v).xwwy(), x,w,w,y);
	do_test_swizzle(Swizzle(v).xwwz(), x,w,w,z);
	do_test_swizzle(Swizzle(v).xwww(), x,w,w,w);


	do_test_swizzle(Swizzle(v).yxxx(), y,x,x,x);
	do_test_swizzle(Swizzle(v).yxxy(), y,x,x,y);
	do_test_swizzle(Swizzle(v).yxxz(), y,x,x,z);
	do_test_swizzle(Swizzle(v).yxxw(), y,x,x,w);
	do_test_swizzle(Swizzle(v).yxyx(), y,x,y,x);
	do_test_swizzle(Swizzle(v).yxyy(), y,x,y,y);
	do_test_swizzle(Swizzle(v).yxyz(), y,x,y,z);
	do_test_swizzle(Swizzle(v).yxyw(), y,x,y,w);
	do_test_swizzle(Swizzle(v).yxzx(), y,x,z,x);
	do_test_swizzle(Swizzle(v).yxzy(), y,x,z,y);
	do_test_swizzle(Swizzle(v).yxzz(), y,x,z,z);
	do_test_swizzle(Swizzle(v).yxzw(), y,x,z,w);
	do_test_swizzle(Swizzle(v).yxwx(), y,x,w,x);
	do_test_swizzle(Swizzle(v).yxwy(), y,x,w,y);
	do_test_swizzle(Swizzle(v).yxwz(), y,x,w,z);
	do_test_swizzle(Swizzle(v).yxww(), y,x,w,w);

	do_test_swizzle(Swizzle(v).yyxx(), y,y,x,x);
	do_test_swizzle(Swizzle(v).yyxy(), y,y,x,y);
	do_test_swizzle(Swizzle(v).yyxz(), y,y,x,z);
	do_test_swizzle(Swizzle(v).yyxw(), y,y,x,w);
	do_test_swizzle(Swizzle(v).yyyx(), y,y,y,x);
	do_test_swizzle(Swizzle(v).yyyy(), y,y,y,y);
	do_test_swizzle(Swizzle(v).yyyz(), y,y,y,z);
	do_test_swizzle(Swizzle(v).yyyw(), y,y,y,w);
	do_test_swizzle(Swizzle(v).yyzx(), y,y,z,x);
	do_test_swizzle(Swizzle(v).yyzy(), y,y,z,y);
	do_test_swizzle(Swizzle(v).yyzz(), y,y,z,z);
	do_test_swizzle(Swizzle(v).yyzw(), y,y,z,w);
	do_test_swizzle(Swizzle(v).yywx(), y,y,w,x);
	do_test_swizzle(Swizzle(v).yywy(), y,y,w,y);
	do_test_swizzle(Swizzle(v).yywz(), y,y,w,z);
	do_test_swizzle(Swizzle(v).yyww(), y,y,w,w);

	do_test_swizzle(Swizzle(v).yzxx(), y,z,x,x);
	do_test_swizzle(Swizzle(v).yzxy(), y,z,x,y);
	do_test_swizzle(Swizzle(v).yzxz(), y,z,x,z);
	do_test_swizzle(Swizzle(v).yzxw(), y,z,x,w);
	do_test_swizzle(Swizzle(v).yzyx(), y,z,y,x);
	do_test_swizzle(Swizzle(v).yzyy(), y,z,y,y);
	do_test_swizzle(Swizzle(v).yzyz(), y,z,y,z);
	do_test_swizzle(Swizzle(v).yzyw(), y,z,y,w);
	do_test_swizzle(Swizzle(v).yzzx(), y,z,z,x);
	do_test_swizzle(Swizzle(v).yzzy(), y,z,z,y);
	do_test_swizzle(Swizzle(v).yzzz(), y,z,z,z);
	do_test_swizzle(Swizzle(v).yzzw(), y,z,z,w);
	do_test_swizzle(Swizzle(v).yzwx(), y,z,w,x);
	do_test_swizzle(Swizzle(v).yzwy(), y,z,w,y);
	do_test_swizzle(Swizzle(v).yzwz(), y,z,w,z);
	do_test_swizzle(Swizzle(v).yzww(), y,z,w,w);

	do_test_swizzle(Swizzle(v).ywxx(), y,w,x,x);
	do_test_swizzle(Swizzle(v).ywxy(), y,w,x,y);
	do_test_swizzle(Swizzle(v).ywxz(), y,w,x,z);
	do_test_swizzle(Swizzle(v).ywxw(), y,w,x,w);
	do_test_swizzle(Swizzle(v).ywyx(), y,w,y,x);
	do_test_swizzle(Swizzle(v).ywyy(), y,w,y,y);
	do_test_swizzle(Swizzle(v).ywyz(), y,w,y,z);
	do_test_swizzle(Swizzle(v).ywyw(), y,w,y,w);
	do_test_swizzle(Swizzle(v).ywzx(), y,w,z,x);
	do_test_swizzle(Swizzle(v).ywzy(), y,w,z,y);
	do_test_swizzle(Swizzle(v).ywzz(), y,w,z,z);
	do_test_swizzle(Swizzle(v).ywzw(), y,w,z,w);
	do_test_swizzle(Swizzle(v).ywwx(), y,w,w,x);
	do_test_swizzle(Swizzle(v).ywwy(), y,w,w,y);
	do_test_swizzle(Swizzle(v).ywwz(), y,w,w,z);
	do_test_swizzle(Swizzle(v).ywww(), y,w,w,w);


	do_test_swizzle(Swizzle(v).zxxx(), z,x,x,x);
	do_test_swizzle(Swizzle(v).zxxy(), z,x,x,y);
	do_test_swizzle(Swizzle(v).zxxz(), z,x,x,z);
	do_test_swizzle(Swizzle(v).zxxw(), z,x,x,w);
	do_test_swizzle(Swizzle(v).zxyx(), z,x,y,x);
	do_test_swizzle(Swizzle(v).zxyy(), z,x,y,y);
	do_test_swizzle(Swizzle(v).zxyz(), z,x,y,z);
	do_test_swizzle(Swizzle(v).zxyw(), z,x,y,w);
	do_test_swizzle(Swizzle(v).zxzx(), z,x,z,x);
	do_test_swizzle(Swizzle(v).zxzy(), z,x,z,y);
	do_test_swizzle(Swizzle(v).zxzz(), z,x,z,z);
	do_test_swizzle(Swizzle(v).zxzw(), z,x,z,w);
	do_test_swizzle(Swizzle(v).zxwx(), z,x,w,x);
	do_test_swizzle(Swizzle(v).zxwy(), z,x,w,y);
	do_test_swizzle(Swizzle(v).zxwz(), z,x,w,z);
	do_test_swizzle(Swizzle(v).zxww(), z,x,w,w);

	do_test_swizzle(Swizzle(v).zyxx(), z,y,x,x);
	do_test_swizzle(Swizzle(v).zyxy(), z,y,x,y);
	do_test_swizzle(Swizzle(v).zyxz(), z,y,x,z);
	do_test_swizzle(Swizzle(v).zyxw(), z,y,x,w);
	do_test_swizzle(Swizzle(v).zyyx(), z,y,y,x);
	do_test_swizzle(Swizzle(v).zyyy(), z,y,y,y);
	do_test_swizzle(Swizzle(v).zyyz(), z,y,y,z);
	do_test_swizzle(Swizzle(v).zyyw(), z,y,y,w);
	do_test_swizzle(Swizzle(v).zyzx(), z,y,z,x);
	do_test_swizzle(Swizzle(v).zyzy(), z,y,z,y);
	do_test_swizzle(Swizzle(v).zyzz(), z,y,z,z);
	do_test_swizzle(Swizzle(v).zyzw(), z,y,z,w);
	do_test_swizzle(Swizzle(v).zywx(), z,y,w,x);
	do_test_swizzle(Swizzle(v).zywy(), z,y,w,y);
	do_test_swizzle(Swizzle(v).zywz(), z,y,w,z);
	do_test_swizzle(Swizzle(v).zyww(), z,y,w,w);

	do_test_swizzle(Swizzle(v).zzxx(), z,z,x,x);
	do_test_swizzle(Swizzle(v).zzxy(), z,z,x,y);
	do_test_swizzle(Swizzle(v).zzxz(), z,z,x,z);
	do_test_swizzle(Swizzle(v).zzxw(), z,z,x,w);
	do_test_swizzle(Swizzle(v).zzyx(), z,z,y,x);
	do_test_swizzle(Swizzle(v).zzyy(), z,z,y,y);
	do_test_swizzle(Swizzle(v).zzyz(), z,z,y,z);
	do_test_swizzle(Swizzle(v).zzyw(), z,z,y,w);
	do_test_swizzle(Swizzle(v).zzzx(), z,z,z,x);
	do_test_swizzle(Swizzle(v).zzzy(), z,z,z,y);
	do_test_swizzle(Swizzle(v).zzzz(), z,z,z,z);
	do_test_swizzle(Swizzle(v).zzzw(), z,z,z,w);
	do_test_swizzle(Swizzle(v).zzwx(), z,z,w,x);
	do_test_swizzle(Swizzle(v).zzwy(), z,z,w,y);
	do_test_swizzle(Swizzle(v).zzwz(), z,z,w,z);
	do_test_swizzle(Swizzle(v).zzww(), z,z,w,w);

	do_test_swizzle(Swizzle(v).zwxx(), z,w,x,x);
	do_test_swizzle(Swizzle(v).zwxy(), z,w,x,y);
	do_test_swizzle(Swizzle(v).zwxz(), z,w,x,z);
	do_test_swizzle(Swizzle(v).zwxw(), z,w,x,w);
	do_test_swizzle(Swizzle(v).zwyx(), z,w,y,x);
	do_test_swizzle(Swizzle(v).zwyy(), z,w,y,y);
	do_test_swizzle(Swizzle(v).zwyz(), z,w,y,z);
	do_test_swizzle(Swizzle(v).zwyw(), z,w,y,w);
	do_test_swizzle(Swizzle(v).zwzx(), z,w,z,x);
	do_test_swizzle(Swizzle(v).zwzy(), z,w,z,y);
	do_test_swizzle(Swizzle(v).zwzz(), z,w,z,z);
	do_test_swizzle(Swizzle(v).zwzw(), z,w,z,w);
	do_test_swizzle(Swizzle(v).zwwx(), z,w,w,x);
	do_test_swizzle(Swizzle(v).zwwy(), z,w,w,y);
	do_test_swizzle(Swizzle(v).zwwz(), z,w,w,z);
	do_test_swizzle(Swizzle(v).zwww(), z,w,w,w);


	do_test_swizzle(Swizzle(v).wxxx(), w,x,x,x);
	do_test_swizzle(Swizzle(v).wxxy(), w,x,x,y);
	do_test_swizzle(Swizzle(v).wxxz(), w,x,x,z);
	do_test_swizzle(Swizzle(v).wxxw(), w,x,x,w);
	do_test_swizzle(Swizzle(v).wxyx(), w,x,y,x);
	do_test_swizzle(Swizzle(v).wxyy(), w,x,y,y);
	do_test_swizzle(Swizzle(v).wxyz(), w,x,y,z);
	do_test_swizzle(Swizzle(v).wxyw(), w,x,y,w);
	do_test_swizzle(Swizzle(v).wxzx(), w,x,z,x);
	do_test_swizzle(Swizzle(v).wxzy(), w,x,z,y);
	do_test_swizzle(Swizzle(v).wxzz(), w,x,z,z);
	do_test_swizzle(Swizzle(v).wxzw(), w,x,z,w);
	do_test_swizzle(Swizzle(v).wxwx(), w,x,w,x);
	do_test_swizzle(Swizzle(v).wxwy(), w,x,w,y);
	do_test_swizzle(Swizzle(v).wxwz(), w,x,w,z);
	do_test_swizzle(Swizzle(v).wxww(), w,x,w,w);

	do_test_swizzle(Swizzle(v).wyxx(), w,y,x,x);
	do_test_swizzle(Swizzle(v).wyxy(), w,y,x,y);
	do_test_swizzle(Swizzle(v).wyxz(), w,y,x,z);
	do_test_swizzle(Swizzle(v).wyxw(), w,y,x,w);
	do_test_swizzle(Swizzle(v).wyyx(), w,y,y,x);
	do_test_swizzle(Swizzle(v).wyyy(), w,y,y,y);
	do_test_swizzle(Swizzle(v).wyyz(), w,y,y,z);
	do_test_swizzle(Swizzle(v).wyyw(), w,y,y,w);
	do_test_swizzle(Swizzle(v).wyzx(), w,y,z,x);
	do_test_swizzle(Swizzle(v).wyzy(), w,y,z,y);
	do_test_swizzle(Swizzle(v).wyzz(), w,y,z,z);
	do_test_swizzle(Swizzle(v).wyzw(), w,y,z,w);
	do_test_swizzle(Swizzle(v).wywx(), w,y,w,x);
	do_test_swizzle(Swizzle(v).wywy(), w,y,w,y);
	do_test_swizzle(Swizzle(v).wywz(), w,y,w,z);
	do_test_swizzle(Swizzle(v).wyww(), w,y,w,w);

	do_test_swizzle(Swizzle(v).wzxx(), w,z,x,x);
	do_test_swizzle(Swizzle(v).wzxy(), w,z,x,y);
	do_test_swizzle(Swizzle(v).wzxz(), w,z,x,z);
	do_test_swizzle(Swizzle(v).wzxw(), w,z,x,w);
	do_test_swizzle(Swizzle(v).wzyx(), w,z,y,x);
	do_test_swizzle(Swizzle(v).wzyy(), w,z,y,y);
	do_test_swizzle(Swizzle(v).wzyz(), w,z,y,z);
	do_test_swizzle(Swizzle(v).wzyw(), w,z,y,w);
	do_test_swizzle(Swizzle(v).wzzx(), w,z,z,x);
	do_test_swizzle(Swizzle(v).wzzy(), w,z,z,y);
	do_test_swizzle(Swizzle(v).wzzz(), w,z,z,z);
	do_test_swizzle(Swizzle(v).wzzw(), w,z,z,w);
	do_test_swizzle(Swizzle(v).wzwx(), w,z,w,x);
	do_test_swizzle(Swizzle(v).wzwy(), w,z,w,y);
	do_test_swizzle(Swizzle(v).wzwz(), w,z,w,z);
	do_test_swizzle(Swizzle(v).wzww(), w,z,w,w);

	do_test_swizzle(Swizzle(v).wwxx(), w,w,x,x);
	do_test_swizzle(Swizzle(v).wwxy(), w,w,x,y);
	do_test_swizzle(Swizzle(v).wwxz(), w,w,x,z);
	do_test_swizzle(Swizzle(v).wwxw(), w,w,x,w);
	do_test_swizzle(Swizzle(v).wwyx(), w,w,y,x);
	do_test_swizzle(Swizzle(v).wwyy(), w,w,y,y);
	do_test_swizzle(Swizzle(v).wwyz(), w,w,y,z);
	do_test_swizzle(Swizzle(v).wwyw(), w,w,y,w);
	do_test_swizzle(Swizzle(v).wwzx(), w,w,z,x);
	do_test_swizzle(Swizzle(v).wwzy(), w,w,z,y);
	do_test_swizzle(Swizzle(v).wwzz(), w,w,z,z);
	do_test_swizzle(Swizzle(v).wwzw(), w,w,z,w);
	do_test_swizzle(Swizzle(v).wwwx(), w,w,w,x);
	do_test_swizzle(Swizzle(v).wwwy(), w,w,w,y);
	do_test_swizzle(Swizzle(v).wwwz(), w,w,w,z);
	do_test_swizzle(Swizzle(v).wwww(), w,w,w,w);
}

BOOST_AUTO_TEST_SUITE_END()
