/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_query_common

template <>
struct __ObjectSubtype<__tag_Query>
{
	typedef __QueryTarget Type;
};
//]
//[oglplus_query_1
template <>
class __ObjectOps<__tag_DirectState, __tag_Query>
 : public __ObjZeroOps<__tag_DirectState, __tag_Query>
{
public:
	typedef __QueryTarget Target;

	void Begin(__QueryTarget target); /*<
	Begins a query on the specified [^target].
	See [glfunc BeginQuery].
	>*/
	void End(__QueryTarget target); /*<
	Ends the currently active query on the specified [^target].
	See [glfunc EndQuery].
	>*/

#if GL_VERSION_3_0
	void BeginConditionalRender(__ConditionalRenderMode mode); /*<
	Begins conditional render on the query in the specified [^mode].
	See [glfunc BeginConditionalRender].
	>*/
	static void EndConditionalRender(void); /*<
	Ends currently active conditional render.
	See [glfunc EndConditionalRender].
	>*/
#endif

#if GL_VERSION_3_3 || GL_ARB_timer_query
	void Counter(__QueryTarget target); /*<
	Does a counter query on the specified [^target].
	See [glfunc QueryCounter].
	>*/
	void Timestamp(void); /*<
	Does a timestamp query.
	See [glfunc QueryCounter], [glconst TIMESTAMP].
	>*/
#endif

	__Boolean ResultAvailable(void) const; /*<
	Returns true if the query result is available.
	See [glfunc GetQueryObject], [glconst QUERY_RESULT_AVAILABLE].
	>*/

	void Result(GLint& result) const; /*<
	Returns the query result.
	See [glfunc GetQueryObject], [glconst QUERY_RESULT].
	>*/
	void Result(GLuint& result) const;

#if GL_VERSION_3_3 || GL_ARB_timer_query
	void Result(GLint64& result) const;
	void Result(GLuint64& result) const;
#endif

	__QueryActivator Activate(__QueryTarget target); /*<
	This function creates an instance of the [^QueryActivator] class which
	begins a query on the specified [^target] when it is constructed
	and ends this query when it is destroyed.
	>*/

	template <typename ResultType>
	__QueryExecution<ResultType>
	Execute(__QueryTarget target, ResultType& result); /*<
	This function creates an instance of the [^QueryExecution] class which
	begins a query on the specified [^target] when it is constructed
	and ends this query and gets its [^result] when it is destroyed.
	>*/
};

typedef ObjectOps<__tag_DirectState, __tag_Query>
	QueryOps;
//]
//[oglplus_query_activator

class QueryActivator
{
public:
	QueryActivator(QueryActivator&&);

	QueryActivator(__QueryName query, __QueryTarget target); /*<
	Begins a [^query] on the specified [^target].
	See [glfunc BeginQuery].
	>*/

	~QueryActivator(void) /*<
	Ends the currently active query.
	See [glfunc EndQuery].
	>*/
	noexcept;

	bool Finish(void); /*<
	Explicitly ends the query without waiting for the destructor.
	See [glfunc EndQuery].
	>*/
};

//]
//[oglplus_conditional_render

#if GL_VERSION_3_0
class ConditionalRender
{
public:
	ConditionalRender(ConditionalRender&&);

	ConditionalRender(
		__QueryName query,
		__ConditionalRenderMode mode
	); /*<
	Begins conditional render on a [^query] in the specified [^mode].
	See [glfunc BeginConditionalRender].
	>*/

	~ConditionalRender(void) /*<
	Ends the currently active conditional render.
	See [glfunc EndConditionalRender].
	>*/
	noexcept;

	bool Finish(void); /*<
	Explicitly ends the currently active conditional render.
	See [glfunc EndConditionalRender].
	>*/
};
#endif

//]
//[oglplus_query_execution

template <typename ResultType>
class QueryExecution
 : public __QueryActivator
{
public:
	QueryExecution(QueryExecution&&);

	QueryExecution(
		__QueryName query,
		__QueryTarget target,
		ResultType& result
	); /*<
	Begins a [^query] on the specified [^target], taking
	a [^reference] where the result will be stored .
	See [glfunc BeginQuery].
	>*/

	~QueryExecution(void) /*<
	Ends the currently active query and waits for the result.
	See [glfunc EndQuery], [glfunc GetQueryObject],
	[glconst QUERY_RESULT_AVAILABLE].
	>*/
	noexcept;

	void WaitForResult(void);  /*<
	Explicitly ends the query and waits for the result.
	See [glfunc EndQuery].
	>*/
};

//]

