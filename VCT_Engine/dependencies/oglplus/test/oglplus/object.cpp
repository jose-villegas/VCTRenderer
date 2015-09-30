/**
 *  .file test/oglplus/object.cpp
 *  .brief Test case for ObjectWrappers
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2011-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_Object
#include <boost/test/unit_test.hpp>
#include "test.hpp"

#include "fixture.hpp"

#include <oglplus/buffer.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/program_pipeline.hpp>
#include <oglplus/program.hpp>
#include <oglplus/query.hpp>
#include <oglplus/renderbuffer.hpp>
#include <oglplus/sampler.hpp>
#include <oglplus/shader.hpp>
#include <oglplus/texture.hpp>
#include <oglplus/transform_feedback.hpp>
#include <oglplus/vertex_array.hpp>

BOOST_GLOBAL_FIXTURE(OGLplusTestFixture)

BOOST_AUTO_TEST_SUITE(Object)

template <template <class> class Tester>
void test_Object_func(void)
{
	BOOST_TEST_CHECKPOINT("BUFFER");
	Tester<oglplus::Buffer>()();

	BOOST_TEST_CHECKPOINT("FRAMEBUFFER");
	Tester<oglplus::Framebuffer>()();

#if GL_VERSION_4_1 || GL_ARB_separate_shader_objects
	BOOST_TEST_CHECKPOINT("PROGRAM_PIPELINE");
	Tester<oglplus::ProgramPipeline>()();
#endif

	BOOST_TEST_CHECKPOINT("PROGRAM");
	Tester<oglplus::Program>()();

	BOOST_TEST_CHECKPOINT("QUERY");
	Tester<oglplus::Query>()();

	BOOST_TEST_CHECKPOINT("RENDERBUFFER");
	Tester<oglplus::Renderbuffer>()();

	BOOST_TEST_CHECKPOINT("SAMPLER");
	Tester<oglplus::Sampler>()();

	BOOST_TEST_CHECKPOINT("VERTEX_SHADER");
	Tester<oglplus::VertexShader>()();
#if GL_VERSION_3_2
	BOOST_TEST_CHECKPOINT("GEOMETRY_SHADER");
	Tester<oglplus::GeometryShader>()();
#endif
#if GL_VERSION_4_0
	BOOST_TEST_CHECKPOINT("TESS_CONTROL_SHADER");
	Tester<oglplus::TessControlShader>()();
#endif
#if GL_VERSION_4_0
	BOOST_TEST_CHECKPOINT("TESS_EVALUATION_SHADER");
	Tester<oglplus::TessEvaluationShader>()();
#endif
	BOOST_TEST_CHECKPOINT("FRAGMENT_SHADER");
	Tester<oglplus::FragmentShader>()();
#if GL_VERSION_4_3
	BOOST_TEST_CHECKPOINT("COMPUTE_SHADER");
	Tester<oglplus::ComputeShader>()();
#endif

	BOOST_TEST_CHECKPOINT("TEXTURE");
	Tester<oglplus::Texture>()();

#if GL_VERSION_4_0 || GL_ARB_transform_feedback2
	BOOST_TEST_CHECKPOINT("TRANSFORM_FEEDBACK");
	Tester<oglplus::TransformFeedback>()();
#endif

	BOOST_TEST_CHECKPOINT("VERTEX_ARRAY");
	Tester<oglplus::VertexArray>()();
}

template <typename Object>
struct Object_default_construction_tester
{
	void operator () (void) const
	{
		Object object;
		BOOST_ASSERT(object.HasValidName());
	}
};

BOOST_AUTO_TEST_CASE(Object_default_construction)
{
	test_Object_func<Object_default_construction_tester>();
}

template <typename Object>
struct Object_move_construction_tester
{
	void operator () (void) const
	{
		Object object1;

		BOOST_ASSERT( object1.HasValidName());
		const GLint name = GetName(object1);

		Object object2 = std::move(object1);

		BOOST_ASSERT(name != GetName(object1));
		BOOST_ASSERT(name == GetName(object2));

		BOOST_ASSERT(!object1.HasValidName());
		BOOST_ASSERT( object2.HasValidName());
	}
};

BOOST_AUTO_TEST_CASE(Object_move_construction)
{
	test_Object_func<Object_move_construction_tester>();
}

template <typename Object>
struct Object_move_assignment_tester
{
	void operator () (void) const
	{
		Object object1, object2;

		BOOST_ASSERT( object1.HasValidName());
		BOOST_ASSERT( object2.HasValidName());

		const GLint name = GetName(object1);

		object2 = std::move(object1);

		BOOST_ASSERT(name != GetName(object1));
		BOOST_ASSERT(name == GetName(object2));

		BOOST_ASSERT(!object1.HasValidName());
		BOOST_ASSERT( object2.HasValidName());

		{ Object object3 = std::move(object2); }

		BOOST_ASSERT(name != GetName(object1));
		BOOST_ASSERT(name != GetName(object2));

		BOOST_ASSERT(!object1.HasValidName());
		BOOST_ASSERT(!object2.HasValidName());
	}
};

BOOST_AUTO_TEST_CASE(Object_move_assignment)
{
	test_Object_func<Object_move_assignment_tester>();
}

BOOST_AUTO_TEST_SUITE_END()
