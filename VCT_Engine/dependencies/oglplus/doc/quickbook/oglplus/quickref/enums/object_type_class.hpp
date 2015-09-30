//  File doc/quickbook/oglplus/quickref/enums/object_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/object_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_object_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ObjectType> class Transform>
class __EnumToClass<Base, __ObjectType, Transform> /*<
Specialization of __EnumToClass for the __ObjectType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ObjectType::Buffer>
		Buffer;
	Transform<ObjectType::Framebuffer>
		Framebuffer;
	Transform<ObjectType::ProgramPipeline>
		ProgramPipeline;
	Transform<ObjectType::Program>
		Program;
	Transform<ObjectType::Query>
		Query;
	Transform<ObjectType::Renderbuffer>
		Renderbuffer;
	Transform<ObjectType::Sampler>
		Sampler;
	Transform<ObjectType::Shader>
		Shader;
	Transform<ObjectType::Texture>
		Texture;
	Transform<ObjectType::TransformFeedback>
		TransformFeedback;
	Transform<ObjectType::VertexArray>
		VertexArray;
	Transform<ObjectType::None>
		None;
};

} // namespace enums
#endif
//]

