/**
 *  .file test/oglplus/test.hpp
 *  .brief Testing-related setup
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef __OGLPLUS_TEST_TEST_1303270827_HPP__
#define __OGLPLUS_TEST_TEST_1303270827_HPP__

#define OGLPLUS_GLFUNC(FUNCNAME) (\
	BOOST_TEST_CHECKPOINT("gl" #FUNCNAME), \
	::gl##FUNCNAME)

#endif // include guard
