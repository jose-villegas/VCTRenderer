/**
 *  .file test/oglplus/boost_utfw.cpp
 *  .brief Test case checking if Boost unit test framework works
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_boost_utf
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(boost_utf)
{
	BOOST_CHECK_EQUAL(1, 1);
}
