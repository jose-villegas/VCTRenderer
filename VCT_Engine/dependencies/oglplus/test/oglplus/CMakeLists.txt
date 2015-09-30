#  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)


include(./oglplus_testing.cmake)

oglplus_exec_test_no_fixture(boost_utfw)
oglplus_exec_test_no_fixture(math)
oglplus_exec_test_no_fixture(angle)
oglplus_exec_test_no_fixture(vector)
oglplus_exec_test_no_fixture(quaternion)
oglplus_exec_test_no_fixture(matrix)

oglplus_exec_test(object "${OGLPLUS_TEST_LIBS}")
oglplus_exec_test(buffer "${OGLPLUS_TEST_LIBS}")

add_test(
	build-oglplus-examples 
	"${CMAKE_COMMAND}"
	--build "${CMAKE_BINARY_DIR}"
	--target oglplus-examples
)

