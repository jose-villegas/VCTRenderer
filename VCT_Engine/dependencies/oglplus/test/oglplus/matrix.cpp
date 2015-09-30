/**
 *  .file test/oglplus/matrix.cpp
 *  .brief Test case for Matrix class and related functionality.
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_Matrix
#include <boost/test/unit_test.hpp>

#include <oglplus/gl.hpp>
#include <oglplus/math/matrix.hpp>

BOOST_AUTO_TEST_SUITE(Matrix)

template <typename T>
void do_test_matrix_default_construction(void)
{
	oglplus::Matrix<T, 2, 2> mat2;
	oglplus::Matrix<T, 2, 3> mat2x3;
	oglplus::Matrix<T, 2, 4> mat2x4;
	oglplus::Matrix<T, 3, 2> mat3x2;
	oglplus::Matrix<T, 3, 3> mat3;
	oglplus::Matrix<T, 3, 4> mat3x4;
	oglplus::Matrix<T, 4, 2> mat4x2;
	oglplus::Matrix<T, 4, 3> mat4x3;
	oglplus::Matrix<T, 4, 4> mat4;
}

BOOST_AUTO_TEST_CASE(Matrix_default_construction)
{
	do_test_matrix_default_construction<float>();
	do_test_matrix_default_construction<double>();
}

BOOST_AUTO_TEST_CASE(Matrix_construction_1)
{
	float mat_init[16] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f
	};
	oglplus::Matrix<float, 2, 2> mat2(mat_init, 4);
	oglplus::Matrix<float, 2, 3> mat2x3(mat_init, 6);
	oglplus::Matrix<float, 3, 2> mat3x2(mat_init, 6);
	oglplus::Matrix<float, 2, 4> mat2x4(mat_init, 8);
	oglplus::Matrix<float, 4, 2> mat4x2(mat_init, 8);
	oglplus::Matrix<float, 3, 3> mat3(mat_init, 9);
	oglplus::Matrix<float, 3, 4> mat3x4(mat_init, 12);
	oglplus::Matrix<float, 4, 3> mat4x3(mat_init, 12);
	oglplus::Matrix<float, 4, 4> mat4(mat_init, 16);
}

BOOST_AUTO_TEST_CASE(Matrix_construction_2)
{
	float mat_init_4[4] = {
		0.0f, 1.0f, 2.0f, 3.0f
	};
	oglplus::Matrix<float, 2, 2> mat2(mat_init_4);

	float mat_init_6[6] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f
	};
	oglplus::Matrix<float, 2, 3> mat2x3(mat_init_6);
	oglplus::Matrix<float, 3, 2> mat3x2(mat_init_6);

	float mat_init_8[8] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f
	};
	oglplus::Matrix<float, 2, 4> mat2x4(mat_init_8);
	oglplus::Matrix<float, 4, 2> mat4x2(mat_init_8);

	float mat_init_9[9] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f
	};
	oglplus::Matrix<float, 3, 3> mat3(mat_init_9);

	float mat_init_12[12] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f
	};
	oglplus::Matrix<float, 3, 4> mat3x4(mat_init_12);
	oglplus::Matrix<float, 4, 3> mat4x3(mat_init_12);

	float mat_init_16[16] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15
	};
	oglplus::Matrix<float, 4, 4> mat4(mat_init_16);
}

BOOST_AUTO_TEST_CASE(Matrix_construction_3)
{
	oglplus::Matrix<float, 2, 2> mat2(
		0.0f, 1.0f,
		2.0f, 3.0f
	);
	oglplus::Matrix<float, 2, 3> mat2x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f
	);
	oglplus::Matrix<float, 2, 4> mat2x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f
	);
	oglplus::Matrix<float, 3, 2> mat3x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f
	);
	oglplus::Matrix<float, 3, 3> mat3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f
	);
	oglplus::Matrix<float, 3, 4> mat3x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 2> mat4x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f,
		6.0f, 7.0f
	);
	oglplus::Matrix<float, 4, 3> mat4x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f,
		9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 4> mat4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);
}

BOOST_AUTO_TEST_CASE(Matrix_construction_4)
{
	oglplus::Matrix<float, 2, 2> mat2(
		oglplus::Vector<float, 2>(0.0f, 1.0f),
		oglplus::Vector<float, 2>(2.0f, 3.0f)
	);
	oglplus::Matrix<float, 2, 3> mat2x3(
		oglplus::Vector<float, 3>(0.0f, 1.0f, 2.0f),
		oglplus::Vector<float, 3>(3.0f, 4.0f, 5.0f)
	);
	oglplus::Matrix<float, 2, 4> mat2x4(
		oglplus::Vector<float, 4>(0.0f, 1.0f, 2.0f, 3.0f),
		oglplus::Vector<float, 4>(4.0f, 5.0f, 6.0f, 7.0f)
	);
	oglplus::Matrix<float, 3, 2> mat3x2(
		oglplus::Vector<float, 2>(0.0f, 1.0f),
		oglplus::Vector<float, 2>(2.0f, 3.0f),
		oglplus::Vector<float, 2>(4.0f, 5.0f)
	);
	oglplus::Matrix<float, 3, 3> mat3(
		oglplus::Vector<float, 3>(0.0f, 1.0f, 2.0f),
		oglplus::Vector<float, 3>(3.0f, 4.0f, 5.0f),
		oglplus::Vector<float, 3>(6.0f, 7.0f, 8.0f)
	);
	oglplus::Matrix<float, 3, 4> mat3x4(
		oglplus::Vector<float, 4>(0.0f, 1.0f, 2.0f, 3.0f),
		oglplus::Vector<float, 4>(4.0f, 5.0f, 6.0f, 7.0f),
		oglplus::Vector<float, 4>(8.0f, 9.0f, 10.f, 11.f)
	);
	oglplus::Matrix<float, 4, 2> mat4x2(
		oglplus::Vector<float, 2>(0.0f, 1.0f),
		oglplus::Vector<float, 2>(2.0f, 3.0f),
		oglplus::Vector<float, 2>(4.0f, 5.0f),
		oglplus::Vector<float, 2>(6.0f, 7.0f)
	);
	oglplus::Matrix<float, 4, 3> mat4x3(
		oglplus::Vector<float, 3>(0.0f, 1.0f, 2.0f),
		oglplus::Vector<float, 3>(3.0f, 4.0f, 5.0f),
		oglplus::Vector<float, 3>(6.0f, 7.0f, 8.0f),
		oglplus::Vector<float, 3>(9.0f, 10.f, 11.f)
	);
	oglplus::Matrix<float, 4, 4> mat4(
		oglplus::Vector<float, 4>(0.0f, 1.0f, 2.0f, 3.0f),
		oglplus::Vector<float, 4>(4.0f, 5.0f, 6.0f, 7.0f),
		oglplus::Vector<float, 4>(8.0f, 9.0f, 10.f, 11.f),
		oglplus::Vector<float, 4>(12.f, 13.f, 14.f, 15.f)
	);
}

BOOST_AUTO_TEST_CASE(Matrix_copy_construction_1)
{
	oglplus::Matrix<float, 2, 2> mat2(
		0.0f, 1.0f,
		2.0f, 3.0f
	);
	oglplus::Matrix<float, 2, 3> mat2x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f
	);
	oglplus::Matrix<float, 2, 4> mat2x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f
	);
	oglplus::Matrix<float, 3, 2> mat3x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f
	);
	oglplus::Matrix<float, 3, 3> mat3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f
	);
	oglplus::Matrix<float, 3, 4> mat3x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 2> mat4x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f,
		6.0f, 7.0f
	);
	oglplus::Matrix<float, 4, 3> mat4x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f,
		9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 4> mat4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);

	oglplus::Matrix<float, 2, 2> mat2c(mat2);
	oglplus::Matrix<float, 2, 3> mat2x3c(mat2x3);
	oglplus::Matrix<float, 2, 4> mat2x4c(mat2x4);
	oglplus::Matrix<float, 3, 2> mat3x2c(mat3x2);
	oglplus::Matrix<float, 3, 3> mat3c(mat3);
	oglplus::Matrix<float, 3, 4> mat3x4c(mat3x4);
	oglplus::Matrix<float, 4, 2> mat4x2c(mat4x2);
	oglplus::Matrix<float, 4, 3> mat4x3c(mat4x3);
	oglplus::Matrix<float, 4, 4> mat4c(mat4);
}

BOOST_AUTO_TEST_CASE(Matrix_copy_construction_2)
{
	oglplus::Matrix<double, 4, 4> mat4(
		0.0, 1.0, 2.0, 3.0,
		4.0, 5.0, 6.0, 7.0,
		8.0, 9.0, 10., 11.,
		12., 13., 14., 15.
	);

	oglplus::Matrix<float, 2, 2> mat2c(mat4);
	oglplus::Matrix<float, 2, 3> mat2x3c(mat4);
	oglplus::Matrix<float, 2, 4> mat2x4c(mat4);
	oglplus::Matrix<float, 3, 2> mat3x2c(mat4);
	oglplus::Matrix<float, 3, 3> mat3c(mat4);
	oglplus::Matrix<float, 3, 4> mat3x4c(mat4);
	oglplus::Matrix<float, 4, 2> mat4x2c(mat4);
	oglplus::Matrix<float, 4, 3> mat4x3c(mat4);
	oglplus::Matrix<float, 4, 4> mat4c(mat4);
}

BOOST_AUTO_TEST_CASE(Matrix_dimensions)
{
	oglplus::Matrix<float, 2, 2> mat2;
	oglplus::Matrix<float, 2, 3> mat2x3;
	oglplus::Matrix<float, 2, 4> mat2x4;
	oglplus::Matrix<float, 3, 2> mat3x2;
	oglplus::Matrix<float, 3, 3> mat3;
	oglplus::Matrix<float, 3, 4> mat3x4;
	oglplus::Matrix<float, 4, 2> mat4x2;
	oglplus::Matrix<float, 4, 3> mat4x3;
	oglplus::Matrix<float, 4, 4> mat4;

	BOOST_CHECK_EQUAL(Rows(mat2), 2);
	BOOST_CHECK_EQUAL(Cols(mat2), 2);
	BOOST_CHECK_EQUAL(Size(mat2), 4);
	BOOST_CHECK_EQUAL(mat2.Size(), 4);

	BOOST_CHECK_EQUAL(Rows(mat2x3), 2);
	BOOST_CHECK_EQUAL(Cols(mat2x3), 3);
	BOOST_CHECK_EQUAL(Size(mat2x3), 6);
	BOOST_CHECK_EQUAL(mat2x3.Size(), 6);

	BOOST_CHECK_EQUAL(Rows(mat2x4), 2);
	BOOST_CHECK_EQUAL(Cols(mat2x4), 4);
	BOOST_CHECK_EQUAL(Size(mat2x4), 8);
	BOOST_CHECK_EQUAL(mat2x4.Size(), 8);

	BOOST_CHECK_EQUAL(Rows(mat3x2), 3);
	BOOST_CHECK_EQUAL(Cols(mat3x2), 2);
	BOOST_CHECK_EQUAL(Size(mat3x2), 6);
	BOOST_CHECK_EQUAL(mat3x2.Size(), 6);

	BOOST_CHECK_EQUAL(Rows(mat3), 3);
	BOOST_CHECK_EQUAL(Cols(mat3), 3);
	BOOST_CHECK_EQUAL(Size(mat3), 9);
	BOOST_CHECK_EQUAL(mat3.Size(), 9);

	BOOST_CHECK_EQUAL(Rows(mat3x4), 3);
	BOOST_CHECK_EQUAL(Cols(mat3x4), 4);
	BOOST_CHECK_EQUAL(Size(mat3x4), 12);
	BOOST_CHECK_EQUAL(mat3x4.Size(), 12);

	BOOST_CHECK_EQUAL(Rows(mat4x2), 4);
	BOOST_CHECK_EQUAL(Cols(mat4x2), 2);
	BOOST_CHECK_EQUAL(Size(mat4x2), 8);
	BOOST_CHECK_EQUAL(mat4x2.Size(), 8);

	BOOST_CHECK_EQUAL(Rows(mat4x3), 4);
	BOOST_CHECK_EQUAL(Cols(mat4x3), 3);
	BOOST_CHECK_EQUAL(Size(mat4x3), 12);
	BOOST_CHECK_EQUAL(mat4x3.Size(), 12);

	BOOST_CHECK_EQUAL(Rows(mat4), 4);
	BOOST_CHECK_EQUAL(Cols(mat4), 4);
	BOOST_CHECK_EQUAL(Size(mat4), 16);
	BOOST_CHECK_EQUAL(mat4.Size(), 16);
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_data_1(const oglplus::Matrix<T, R, C>& m)
{
	BOOST_CHECK_EQUAL(Rows(m), R);
	BOOST_CHECK_EQUAL(Cols(m), C);

	for(std::size_t i=0; i!=R; ++i)
	{
		for(std::size_t j=0; j!=C; ++j)
		{
			if(i == j)
			{
				BOOST_CHECK_EQUAL(Data(m)[i*C+j], T(1));
			}
			else
			{
				BOOST_CHECK_EQUAL(Data(m)[i*C+j], T(0));
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(Matrix_data_1)
{
	oglplus::Matrix<float, 2, 2> mat2;
	oglplus::Matrix<float, 2, 3> mat2x3;
	oglplus::Matrix<float, 2, 4> mat2x4;
	oglplus::Matrix<float, 3, 2> mat3x2;
	oglplus::Matrix<float, 3, 3> mat3;
	oglplus::Matrix<float, 3, 4> mat3x4;
	oglplus::Matrix<float, 4, 2> mat4x2;
	oglplus::Matrix<float, 4, 3> mat4x3;
	oglplus::Matrix<float, 4, 4> mat4;

	do_test_matrix_data_1(mat2);
	do_test_matrix_data_1(mat2x3);
	do_test_matrix_data_1(mat2x4);
	do_test_matrix_data_1(mat3x2);
	do_test_matrix_data_1(mat3);
	do_test_matrix_data_1(mat3x4);
	do_test_matrix_data_1(mat4x2);
	do_test_matrix_data_1(mat4x3);
	do_test_matrix_data_1(mat4);
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_data_i(const oglplus::Matrix<T, R, C>& m)
{
	BOOST_CHECK_EQUAL(Rows(m), R);
	BOOST_CHECK_EQUAL(Cols(m), C);

	std::size_t k = 0;

	for(std::size_t i=0; i!=R; ++i)
	{
		for(std::size_t j=0; j!=C; ++j)
		{
			BOOST_CHECK_EQUAL(Data(m)[i*C+j], T(k++));
		}
	}
}

BOOST_AUTO_TEST_CASE(Matrix_data_2)
{
	float mat_init_4[4] = {
		0.0f, 1.0f, 2.0f, 3.0f
	};
	oglplus::Matrix<float, 2, 2> mat2(mat_init_4, 4);

	float mat_init_6[6] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f
	};
	oglplus::Matrix<float, 2, 3> mat2x3(mat_init_6, 6);
	oglplus::Matrix<float, 3, 2> mat3x2(mat_init_6, 6);

	float mat_init_8[8] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f
	};
	oglplus::Matrix<float, 2, 4> mat2x4(mat_init_8, 8);
	oglplus::Matrix<float, 4, 2> mat4x2(mat_init_8, 8);

	float mat_init_9[9] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f
	};
	oglplus::Matrix<float, 3, 3> mat3(mat_init_9, 9);

	float mat_init_12[12] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f
	};
	oglplus::Matrix<float, 3, 4> mat3x4(mat_init_12, 12);
	oglplus::Matrix<float, 4, 3> mat4x3(mat_init_12, 12);

	float mat_init_16[16] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15
	};
	oglplus::Matrix<float, 4, 4> mat4(mat_init_16, 16);

	do_test_matrix_data_i(mat2);
	do_test_matrix_data_i(mat2x3);
	do_test_matrix_data_i(mat2x4);
	do_test_matrix_data_i(mat3x2);
	do_test_matrix_data_i(mat3);
	do_test_matrix_data_i(mat3x4);
	do_test_matrix_data_i(mat4x2);
	do_test_matrix_data_i(mat4x3);
	do_test_matrix_data_i(mat4);
}


BOOST_AUTO_TEST_CASE(Matrix_data_3)
{
	oglplus::Matrix<float, 2, 2> mat2(
		0.0f, 1.0f,
		2.0f, 3.0f
	);
	oglplus::Matrix<float, 2, 3> mat2x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f
	);
	oglplus::Matrix<float, 2, 4> mat2x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f
	);
	oglplus::Matrix<float, 3, 2> mat3x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f
	);
	oglplus::Matrix<float, 3, 3> mat3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f
	);
	oglplus::Matrix<float, 3, 4> mat3x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 2> mat4x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f,
		6.0f, 7.0f
	);
	oglplus::Matrix<float, 4, 3> mat4x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f,
		9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 4> mat4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);

	do_test_matrix_data_i(mat2);
	do_test_matrix_data_i(mat2x3);
	do_test_matrix_data_i(mat2x4);
	do_test_matrix_data_i(mat3x2);
	do_test_matrix_data_i(mat3);
	do_test_matrix_data_i(mat3x4);
	do_test_matrix_data_i(mat4x2);
	do_test_matrix_data_i(mat4x3);
	do_test_matrix_data_i(mat4);
}


BOOST_AUTO_TEST_CASE(Matrix_data_4)
{
	oglplus::Matrix<float, 2, 2> mat2(
		oglplus::Vector<float, 2>(0.0f, 1.0f),
		oglplus::Vector<float, 2>(2.0f, 3.0f)
	);
	oglplus::Matrix<float, 2, 3> mat2x3(
		oglplus::Vector<float, 3>(0.0f, 1.0f, 2.0f),
		oglplus::Vector<float, 3>(3.0f, 4.0f, 5.0f)
	);
	oglplus::Matrix<float, 2, 4> mat2x4(
		oglplus::Vector<float, 4>(0.0f, 1.0f, 2.0f, 3.0f),
		oglplus::Vector<float, 4>(4.0f, 5.0f, 6.0f, 7.0f)
	);
	oglplus::Matrix<float, 3, 2> mat3x2(
		oglplus::Vector<float, 2>(0.0f, 1.0f),
		oglplus::Vector<float, 2>(2.0f, 3.0f),
		oglplus::Vector<float, 2>(4.0f, 5.0f)
	);
	oglplus::Matrix<float, 3, 3> mat3(
		oglplus::Vector<float, 3>(0.0f, 1.0f, 2.0f),
		oglplus::Vector<float, 3>(3.0f, 4.0f, 5.0f),
		oglplus::Vector<float, 3>(6.0f, 7.0f, 8.0f)
	);
	oglplus::Matrix<float, 3, 4> mat3x4(
		oglplus::Vector<float, 4>(0.0f, 1.0f, 2.0f, 3.0f),
		oglplus::Vector<float, 4>(4.0f, 5.0f, 6.0f, 7.0f),
		oglplus::Vector<float, 4>(8.0f, 9.0f, 10.f, 11.f)
	);
	oglplus::Matrix<float, 4, 2> mat4x2(
		oglplus::Vector<float, 2>(0.0f, 1.0f),
		oglplus::Vector<float, 2>(2.0f, 3.0f),
		oglplus::Vector<float, 2>(4.0f, 5.0f),
		oglplus::Vector<float, 2>(6.0f, 7.0f)
	);
	oglplus::Matrix<float, 4, 3> mat4x3(
		oglplus::Vector<float, 3>(0.0f, 1.0f, 2.0f),
		oglplus::Vector<float, 3>(3.0f, 4.0f, 5.0f),
		oglplus::Vector<float, 3>(6.0f, 7.0f, 8.0f),
		oglplus::Vector<float, 3>(9.0f, 10.f, 11.f)
	);
	oglplus::Matrix<float, 4, 4> mat4(
		oglplus::Vector<float, 4>(0.0f, 1.0f, 2.0f, 3.0f),
		oglplus::Vector<float, 4>(4.0f, 5.0f, 6.0f, 7.0f),
		oglplus::Vector<float, 4>(8.0f, 9.0f, 10.f, 11.f),
		oglplus::Vector<float, 4>(12.f, 13.f, 14.f, 15.f)
	);

	do_test_matrix_data_i(mat2);
	do_test_matrix_data_i(mat2x3);
	do_test_matrix_data_i(mat2x4);
	do_test_matrix_data_i(mat3x2);
	do_test_matrix_data_i(mat3);
	do_test_matrix_data_i(mat3x4);
	do_test_matrix_data_i(mat4x2);
	do_test_matrix_data_i(mat4x3);
	do_test_matrix_data_i(mat4);
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_at(const oglplus::Matrix<T, R, C>& m, std::size_t k = 0)
{
	BOOST_CHECK_EQUAL(Rows(m), R);
	BOOST_CHECK_EQUAL(Cols(m), C);

	for(std::size_t i=0; i!=R; ++i)
	{
		for(std::size_t j=0; j!=C; ++j)
		{
			BOOST_CHECK_EQUAL(m.At(i, j), T(k++));
		}
	}
}

BOOST_AUTO_TEST_CASE(Matrix_at)
{
	oglplus::Matrix<float, 2, 2> mat2(
		0.0f, 1.0f,
		2.0f, 3.0f
	);
	oglplus::Matrix<float, 2, 3> mat2x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f
	);
	oglplus::Matrix<float, 2, 4> mat2x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f
	);
	oglplus::Matrix<float, 3, 2> mat3x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f
	);
	oglplus::Matrix<float, 3, 3> mat3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f
	);
	oglplus::Matrix<float, 3, 4> mat3x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 2> mat4x2(
		0.0f, 1.0f,
		2.0f, 3.0f,
		4.0f, 5.0f,
		6.0f, 7.0f
	);
	oglplus::Matrix<float, 4, 3> mat4x3(
		0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f,
		6.0f, 7.0f, 8.0f,
		9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 4> mat4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);

	do_test_matrix_at(mat2);
	do_test_matrix_at(mat2x3);
	do_test_matrix_at(mat2x4);
	do_test_matrix_at(mat3x2);
	do_test_matrix_at(mat3);
	do_test_matrix_at(mat3x4);
	do_test_matrix_at(mat4x2);
	do_test_matrix_at(mat4x3);
	do_test_matrix_at(mat4);
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_set(oglplus::Matrix<T, R, C>& m, std::size_t n = 10)
{
	BOOST_CHECK_EQUAL(Rows(m), R);
	BOOST_CHECK_EQUAL(Cols(m), C);

	std::size_t k = n;

	for(std::size_t i=0; i!=R; ++i)
	{
		for(std::size_t j=0; j!=C; ++j)
		{
			m.Set(i, j, T(k++));
		}
	}
	do_test_matrix_at(m, n);
}

BOOST_AUTO_TEST_CASE(Matrix_set)
{
	oglplus::Matrix<float, 2, 2> mat2;
	oglplus::Matrix<float, 2, 3> mat2x3;
	oglplus::Matrix<float, 2, 4> mat2x4;
	oglplus::Matrix<float, 3, 2> mat3x2;
	oglplus::Matrix<float, 3, 3> mat3;
	oglplus::Matrix<float, 3, 4> mat3x4;
	oglplus::Matrix<float, 4, 2> mat4x2;
	oglplus::Matrix<float, 4, 3> mat4x3;
	oglplus::Matrix<float, 4, 4> mat4;

	do_test_matrix_set(mat2);
	do_test_matrix_set(mat2x3);
	do_test_matrix_set(mat2x4);
	do_test_matrix_set(mat3x2);
	do_test_matrix_set(mat3);
	do_test_matrix_set(mat3x4);
	do_test_matrix_set(mat4x2);
	do_test_matrix_set(mat4x3);
	do_test_matrix_set(mat4);
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_row(oglplus::Matrix<T, R, C>& m)
{
	std::size_t k = 20;
	do_test_matrix_set(m, k);

	BOOST_CHECK_EQUAL(Rows(m), R);
	BOOST_CHECK_EQUAL(Cols(m), C);

	for(std::size_t i=0; i!=R; ++i)
	{
		oglplus::Vector<T, C> row = m.Row(i);
		for(std::size_t j=0; j!=C; ++j)
		{
			BOOST_CHECK_EQUAL(row[j], T(k++));
		}
	}
}

BOOST_AUTO_TEST_CASE(Matrix_row)
{
	oglplus::Matrix<float, 2, 2> mat2;
	oglplus::Matrix<float, 2, 3> mat2x3;
	oglplus::Matrix<float, 2, 4> mat2x4;
	oglplus::Matrix<float, 3, 2> mat3x2;
	oglplus::Matrix<float, 3, 3> mat3;
	oglplus::Matrix<float, 3, 4> mat3x4;
	oglplus::Matrix<float, 4, 2> mat4x2;
	oglplus::Matrix<float, 4, 3> mat4x3;
	oglplus::Matrix<float, 4, 4> mat4;

	do_test_matrix_row(mat2);
	do_test_matrix_row(mat2x3);
	do_test_matrix_row(mat2x4);
	do_test_matrix_row(mat3x2);
	do_test_matrix_row(mat3);
	do_test_matrix_row(mat3x4);
	do_test_matrix_row(mat4x2);
	do_test_matrix_row(mat4x3);
	do_test_matrix_row(mat4);
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_col(oglplus::Matrix<T, R, C>& m)
{
	std::size_t k = 20;
	do_test_matrix_set(m, k);

	BOOST_CHECK_EQUAL(Rows(m), R);
	BOOST_CHECK_EQUAL(Cols(m), C);

	for(std::size_t i=0; i!=R; ++i)
	{
		for(std::size_t j=0; j!=C; ++j)
		{
			oglplus::Vector<T, R> col = m.Col(j);
			BOOST_CHECK_EQUAL(col[i], T(k++));
		}
	}
}

BOOST_AUTO_TEST_CASE(Matrix_col)
{
	oglplus::Matrix<float, 2, 2> mat2;
	oglplus::Matrix<float, 2, 3> mat2x3;
	oglplus::Matrix<float, 2, 4> mat2x4;
	oglplus::Matrix<float, 3, 2> mat3x2;
	oglplus::Matrix<float, 3, 3> mat3;
	oglplus::Matrix<float, 3, 4> mat3x4;
	oglplus::Matrix<float, 4, 2> mat4x2;
	oglplus::Matrix<float, 4, 3> mat4x3;
	oglplus::Matrix<float, 4, 4> mat4;

	do_test_matrix_col(mat2);
	do_test_matrix_col(mat2x3);
	do_test_matrix_col(mat2x4);
	do_test_matrix_col(mat3x2);
	do_test_matrix_col(mat3);
	do_test_matrix_col(mat3x4);
	do_test_matrix_col(mat4x2);
	do_test_matrix_col(mat4x3);
	do_test_matrix_col(mat4);
}

BOOST_AUTO_TEST_CASE(Matrix_comparison_1)
{
	oglplus::Matrix<float, 2, 2> mat2(
		0.0f, 1.0f,
		4.0f, 5.0f
	);
	oglplus::Matrix<float, 2, 3> mat2x3(
		0.0f, 1.0f, 2.0f,
		4.0f, 5.0f, 6.0f
	);
	oglplus::Matrix<float, 2, 4> mat2x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f
	);
	oglplus::Matrix<float, 3, 2> mat3x2(
		0.0f, 1.0f,
		4.0f, 5.0f,
		8.0f, 9.0f
	);
	oglplus::Matrix<float, 3, 3> mat3(
		0.0f, 1.0f, 2.0f,
		4.0f, 5.0f, 6.0f,
		8.0f, 9.0f, 10.f
	);
	oglplus::Matrix<float, 3, 4> mat3x4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f
	);
	oglplus::Matrix<float, 4, 2> mat4x2(
		0.0f, 1.0f,
		4.0f, 5.0f,
		8.0f, 9.0f,
		12.f, 13.f
	);
	oglplus::Matrix<float, 4, 3> mat4x3(
		0.0f, 1.0f, 2.0f,
		4.0f, 5.0f, 6.0f,
		8.0f, 9.0f, 10.f,
		12.f, 13.f, 14.f
	);
	oglplus::Matrix<float, 4, 4> mat4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);

	oglplus::Matrix<float, 2, 2> mat2c(mat4);
	oglplus::Matrix<float, 2, 3> mat2x3c(mat4);
	oglplus::Matrix<float, 2, 4> mat2x4c(mat4);
	oglplus::Matrix<float, 3, 2> mat3x2c(mat4);
	oglplus::Matrix<float, 3, 3> mat3c(mat4);
	oglplus::Matrix<float, 3, 4> mat3x4c(mat4);
	oglplus::Matrix<float, 4, 2> mat4x2c(mat4);
	oglplus::Matrix<float, 4, 3> mat4x3c(mat4);
	oglplus::Matrix<float, 4, 4> mat4c(mat4);

	BOOST_CHECK(mat2 == mat2c);
	BOOST_CHECK(mat2x3 == mat2x3c);
	BOOST_CHECK(mat2x4 == mat2x4c);
	BOOST_CHECK(mat3x2 == mat3x2c);
	BOOST_CHECK(mat3 == mat3c);
	BOOST_CHECK(mat3x4 == mat3x4c);
	BOOST_CHECK(mat4x2 == mat4x2c);
	BOOST_CHECK(mat4x3 == mat4x3c);
	BOOST_CHECK(mat4 == mat4c);
}

// TODO

BOOST_AUTO_TEST_CASE(Matrix_submatrix)
{
	oglplus::Matrix<float, 4, 4> mat4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);

	oglplus::Matrix<float, 2, 2> mat2c(mat4);
	oglplus::Matrix<float, 2, 3> mat2x3c(mat4);
	oglplus::Matrix<float, 2, 4> mat2x4c(mat4);
	oglplus::Matrix<float, 3, 2> mat3x2c(mat4);
	oglplus::Matrix<float, 3, 3> mat3c(mat4);
	oglplus::Matrix<float, 3, 4> mat3x4c(mat4);
	oglplus::Matrix<float, 4, 2> mat4x2c(mat4);
	oglplus::Matrix<float, 4, 3> mat4x3c(mat4);
	oglplus::Matrix<float, 4, 4> mat4c(mat4);

	BOOST_CHECK((oglplus::Submatrix<0, 0, 2, 2>(mat4) == mat2c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 2, 3>(mat4) == mat2x3c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 2, 4>(mat4) == mat2x4c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 3, 2>(mat4) == mat3x2c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 3, 3>(mat4) == mat3c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 3, 4>(mat4) == mat3x4c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 4, 2>(mat4) == mat4x2c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 4, 3>(mat4) == mat4x3c));
	BOOST_CHECK((oglplus::Submatrix<0, 0, 4, 4>(mat4) == mat4c));

	BOOST_CHECK((
		oglplus::Submatrix<0, 1, 2, 2>(mat4) ==
		oglplus::Matrix<float, 2, 2>(
			1.0f, 2.0f,
			5.0f, 6.0f
		)
	));

	BOOST_CHECK((
		oglplus::Submatrix<1, 0, 2, 2>(mat4) ==
		oglplus::Matrix<float, 2, 2>(
			4.0f, 5.0f,
			8.0f, 9.0f
		)
	));

	BOOST_CHECK((
		oglplus::Submatrix<1, 1, 2, 2>(mat4) ==
		oglplus::Matrix<float, 2, 2>(
			5.0f, 6.0f,
			9.0f, 10.f
		)
	));

	BOOST_CHECK((
		oglplus::Submatrix<2, 2, 2, 2>(mat4) ==
		oglplus::Matrix<float, 2, 2>(
			10.f, 11.f,
			14.f, 15.f
		)
	));

	BOOST_CHECK((
		oglplus::Submatrix<1, 1, 3, 3>(mat4) ==
		oglplus::Matrix<float, 3, 3>(
			5.0f, 6.0f, 7.0f,
			9.0f, 10.f, 11.f,
			13.f, 14.f, 15.f
		)
	));
}

BOOST_AUTO_TEST_CASE(Matrix_negation)
{
	oglplus::Matrix<float, 4, 4> mat4(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);
	oglplus::Matrix<float, 4, 4> mat4n(
		-0.0f,-1.0f,-2.0f,-3.0f,
		-4.0f,-5.0f,-6.0f,-7.0f,
		-8.0f,-9.0f,-10.f,-11.f,
		-12.f,-13.f,-14.f,-15.f
	);

	BOOST_CHECK(mat4 != mat4n);
	BOOST_CHECK(mat4 == -mat4n);
	BOOST_CHECK(-mat4 == mat4n);
	BOOST_CHECK(mat4 == -(-mat4));
	BOOST_CHECK(mat4n == -(-mat4n));
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_addition(void)
{
	const T eps = 1e-9;
	for(unsigned y=0; y!=1000; ++y)
	{
		T data_1[R*C];
		T data_2[R*C];
		for(std::size_t x=0; x!=R*C; ++x)
		{
			data_1[x] = T(std::rand())/RAND_MAX;
			data_2[x] = T(std::rand())/RAND_MAX;
		}

		oglplus::Matrix<T, R, C> mat_1(data_1, R*C);
		oglplus::Matrix<T, R, C> mat_2(data_2, R*C);
		oglplus::Matrix<T, R, C> mat_3 = mat_1+mat_2;

		for(std::size_t i=0; i!=R; ++i)
		{
			for(std::size_t j=0; j!=C; ++j)
			{
				BOOST_CHECK_CLOSE(
					mat_3.At(i, j),
					mat_2.At(i, j)+
					mat_1.At(i, j),
					eps
				);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(Matrix_addition)
{
	oglplus::Matrix<float, 4, 4> mat4a(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);
	oglplus::Matrix<float, 4, 4> mat4b(
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	oglplus::Matrix<float, 4, 4> mat4c(
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 10.f, 11.f, 12.f,
		13.f, 14.f, 15.f, 16.f
	);

	BOOST_CHECK(mat4a+mat4b == mat4c);
	BOOST_CHECK(mat4b+mat4a == mat4c);

	do_test_matrix_addition<float, 2, 2>();
	do_test_matrix_addition<float, 2, 3>();
	do_test_matrix_addition<float, 2, 4>();
	do_test_matrix_addition<float, 3, 2>();
	do_test_matrix_addition<float, 3, 3>();
	do_test_matrix_addition<float, 3, 4>();
	do_test_matrix_addition<float, 4, 2>();
	do_test_matrix_addition<float, 4, 3>();
	do_test_matrix_addition<float, 4, 4>();
}

template <typename T, std::size_t R, std::size_t C>
void do_test_matrix_subtraction(void)
{
	const T eps = 1e-9;
	for(unsigned y=0; y!=1000; ++y)
	{
		T data_1[R*C];
		T data_2[R*C];
		T data_3[R*C];
		for(std::size_t x=0; x!=R*C; ++x)
		{
			data_1[x] = T(std::rand())/RAND_MAX;
			data_2[x] = T(std::rand())/RAND_MAX;
			data_3[x] = data_1[x] + data_2[x];
		}

		oglplus::Matrix<T, R, C> mat_1(data_1, R*C);
		oglplus::Matrix<T, R, C> mat_2(data_2, R*C);
		oglplus::Matrix<T, R, C> mat_3(data_3, R*C);
		oglplus::Matrix<T, R, C> mat_4 = mat_3-mat_1;
		oglplus::Matrix<T, R, C> mat_5 = mat_3-mat_2;

		for(std::size_t i=0; i!=R; ++i)
		{
			for(std::size_t j=0; j!=C; ++j)
			{
				BOOST_CHECK_CLOSE(
					mat_4.At(i, j),
					mat_2.At(i, j),
					eps
				);
				BOOST_CHECK_CLOSE(
					mat_5.At(i, j),
					mat_1.At(i, j),
					eps
				);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(Matrix_subtraction)
{
	oglplus::Matrix<float, 4, 4> mat4a(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.f, 11.f,
		12.f, 13.f, 14.f, 15.f
	);
	oglplus::Matrix<float, 4, 4> mat4b(
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	oglplus::Matrix<float, 4, 4> mat4c(
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 10.f, 11.f, 12.f,
		13.f, 14.f, 15.f, 16.f
	);

	BOOST_CHECK(mat4c-mat4a == mat4b);
	BOOST_CHECK(mat4c-mat4b == mat4a);

	do_test_matrix_subtraction<double, 2, 2>();
	do_test_matrix_subtraction<double, 2, 3>();
	do_test_matrix_subtraction<double, 2, 4>();
	do_test_matrix_subtraction<double, 3, 2>();
	do_test_matrix_subtraction<double, 3, 3>();
	do_test_matrix_subtraction<double, 3, 4>();
	do_test_matrix_subtraction<double, 4, 2>();
	do_test_matrix_subtraction<double, 4, 3>();
	do_test_matrix_subtraction<double, 4, 4>();
}

template <typename T, std::size_t M, std::size_t N, std::size_t K, std::size_t L>
void do_test_matrix_multiplication(void)
{
	const T eps = 1e-9;
	for(unsigned y=0; y!=1000; ++y)
	{
		T data_1[M*N];
		for(std::size_t x=0; x!=M*N; ++x)
			data_1[x] = T(std::rand())/RAND_MAX-0.5;
		oglplus::Matrix<T, M, N> mat_1(data_1, M*N);

		T data_2[N*K];
		for(std::size_t x=0; x!=N*K; ++x)
			data_2[x] = T(std::rand())/RAND_MAX-0.5;
		oglplus::Matrix<T, N, K> mat_2(data_2, N*K);

		T data_3[K*L];
		for(std::size_t x=0; x!=K*L; ++x)
			data_3[x] = T(std::rand())/RAND_MAX-0.5;
		oglplus::Matrix<T, K, L> mat_3(data_3, K*L);

		T data_4[L*M];
		for(std::size_t x=0; x!=L*M; ++x)
			data_4[x] = T(std::rand())/RAND_MAX-0.5;
		oglplus::Matrix<T, L, M> mat_4(data_4, L*M);

		BOOST_CHECK(Close(
			 mat_1 * mat_2 * mat_3 * mat_4,
			 mat_1 *(mat_2 * mat_3)* mat_4,
			 eps
		));

		BOOST_CHECK(Close(
			 mat_1 * mat_2 * mat_3 * mat_4,
			(mat_1 * mat_2)*(mat_3 * mat_4),
			 eps
		));
	}
}

BOOST_AUTO_TEST_CASE(Matrix_multiplication)
{
	do_test_matrix_multiplication<double, 2, 2, 2, 2>();
	do_test_matrix_multiplication<double, 2, 3, 4, 2>();
	do_test_matrix_multiplication<double, 2, 4, 4, 2>();
	do_test_matrix_multiplication<double, 3, 3, 3, 3>();
	do_test_matrix_multiplication<double, 3, 2, 4, 2>();
	do_test_matrix_multiplication<double, 3, 4, 3, 4>();
	do_test_matrix_multiplication<double, 4, 2, 2, 4>();
	do_test_matrix_multiplication<double, 4, 2, 3, 4>();
	do_test_matrix_multiplication<double, 4, 3, 2, 4>();
	do_test_matrix_multiplication<double, 4, 3, 3, 4>();
	do_test_matrix_multiplication<double, 4, 4, 4, 4>();
}

BOOST_AUTO_TEST_CASE(Matrix_inverse)
{
	double eps = 1.0;
	for(unsigned i=0; i!=1000; ++i)
	{
		typedef oglplus::Vector<double, 2> vec2;
		typedef oglplus::Vector<double, 3> vec3;
		typedef oglplus::Vector<double, 4> vec4;
		typedef oglplus::Matrix<double, 4, 4> mat4;

		vec2 v0(
			+1.0f+float(std::rand())/RAND_MAX,
			-1.0f-float(std::rand())/RAND_MAX
		);
		vec2 v1(Perpendicular(v0));

		vec3 v2(Cross(vec3(v0, 0.0), vec3(v1, 0.0)));

		mat4 e;
		mat4 m(
			vec4(v0, 0.0, float(std::rand())/RAND_MAX),
			vec4(v1, 0.0, float(std::rand())/RAND_MAX),
			vec4(v2,      float(std::rand())/RAND_MAX),
			vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
		mat4 inv(Inverse(m));

		BOOST_CHECK(oglplus::Close(m*inv, e, eps));
		BOOST_CHECK(oglplus::Close(Inverse(inv), m, eps));
	}
}

// TODO


BOOST_AUTO_TEST_CASE(Matrix_rotation)
{
	double eps = 1e-9;
	for(unsigned i=0; i!=1000; ++i)
	{
		oglplus::Vector<double, 3> axis(
			double(std::rand())/RAND_MAX-0.5,
			double(std::rand())/RAND_MAX-0.5,
			double(std::rand())/RAND_MAX-0.5
		);
		axis.Normalize();
		oglplus::Angle<double> angle =
			oglplus::Angle<double>::Degrees(std::rand()*0.001-1000.0);

		oglplus::Quaternion<double> quat(axis, angle);

		auto m1 = oglplus::ModelMatrix<double>::RotationA(axis, angle);
		auto m2 = oglplus::ModelMatrix<double>::RotationQ(quat);

		BOOST_CHECK(oglplus::Close(m1, m2, eps));
	}
}

// TODO

BOOST_AUTO_TEST_SUITE_END()

