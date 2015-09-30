/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_name

template <typename __ObjTag>
class ObjectName
{
public:
	ObjectName(void) noexcept; /*<
	Constructs a wrapper for object
	with name [^0] (zero).
	>*/

	explicit ObjectName(GLuint name) noexcept; /*<
	Constructs a wrapper for an object
	with the specified [^name].
	>*/

	ObjectName(const ObjectName&) noexcept;
	ObjectName(ObjectName&&) noexcept;

	ObjectName& operator = (const ObjectName&) noexcept;
	ObjectName& operator = (ObjectName&&) noexcept;

	friend bool operator == (ObjectName, ObjectName);
	friend bool operator != (ObjectName, ObjectName);
	friend bool operator <  (ObjectName, ObjectName);
};

template <typename ObjTag>
GLuint GetGLName(__ObjectName<__ObjTag> named); /*<
	Allows to access the 'raw' GL object name.
>*/

//]
//[oglplus_object_name_typedefs

typedef __ObjectName<tag::Renderbuffer> RenderbufferName;
typedef ObjectName<tag::Framebuffer> FramebufferName;
typedef ObjectName<tag::Texture> TextureName;
typedef ObjectName<tag::Buffer> BufferName;
typedef ObjectName<tag::Query> QueryName;
typedef ObjectName<tag::ProgramPipeline> ProgramPipelineName;
typedef ObjectName<tag::Program> ProgramName;
typedef ObjectName<tag::TransformFeedback> TransformFeedbackName;
typedef ObjectName<tag::Sampler> SamplerName;
typedef ObjectName<tag::VertexArray> VertexArrayName;
typedef ObjectName<tag::Shader> ShaderName;
typedef ObjectName<tag::PerfMonitorAMD> PerfMonitorAMDName;
typedef ObjectName<tag::PathNV> PathNVName;

//]

