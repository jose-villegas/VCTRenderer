/**
 *  .file test/oglplus/fixture.hpp
 *  .brief Declaration of test fixture class.
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef __OGLPLUS_TEST_FIXTURE_1303241505_HPP__
#define __OGLPLUS_TEST_FIXTURE_1303241505_HPP__

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>

namespace oglplus {

struct TestFixture
{
	void* _pimpl;

	TestFixture(void);
	TestFixture(TestFixture&&);
	~TestFixture(void);
};

} // namespace oglplus

typedef oglplus::TestFixture OGLplusTestFixture;


#endif // include guard
