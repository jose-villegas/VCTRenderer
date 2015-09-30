/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_ObjectError

class ObjectError
 : public __Error
{
public:
	ObjectError(const char* message);

	GLenum ObjectType(void) const; /*<
	Returns the OpenGL constant value determining the object type
	(like [^GL_PROGRAM], [^GL_TEXTURE], [^GL_SHADER], [^GL_BUFFER], etc.),
	which caused the error.
	If the object type is not available the value zero is returned.
	>*/
	const char* ObjectTypeName(void) const; /*<
	Returns a c-string (without the [^GL_] prefix) corresponding
	to the GL constant determining the object type (like [^"PROGRAM"],
	[^"TEXTURE"], [^"SHADER"], [^"BUFFER"], [^"SAMPLER"], etc.).
	If the object type name is not available then nullptr is returned.
	>*/

	GLenum BindTarget(void) const; /*<
	Returns the OpenGL constant value determining the bind target,
	to which the object related to the error was bound.
	If the target is not available the value zero is returned.
	>*/
	const char* TargetName(void) const; /*<
	Returns a c-string (without the [^GL_] prefix) corresponding
	to the GL constant determining the bind target.
	If the target name is not available then nullptr is returned.
	>*/

	GLint ObjectName(void) const; /*<
	Returns the raw OpenGL (integer) name of the object, which caused
	the error. If the object name is not available then a negative
	integer value is returned.
	>*/
	const __String& ObjectDesc(void) const; /*<
	Returns a textual description of the object, which caused the error.
	If no description is available, then an empty __String is returned.
	>*/
};
//]

//[oglplus_error_ObjectPairError

class ObjectPairError
 : public __ObjectError
{
public:
	ObjectPairError(const char* message);

	GLenum SubjectType(void) const; /*<
	Returns the OpenGL constant value determining the type of the subject,
	in the object pair which caused the error.
	If the subject type is not available the value zero is returned.
	>*/
	const char* SubjectTypeName(void) const; /*<
	Returns a c-string (without the [^GL_] prefix) corresponding
	to the GL constant determining the subject type.
	If the subject type name is not available then nullptr is returned.
	>*/

	GLint SubjectName(void) const; /*<
	Returns the OpenGL (integer) name of the subject in the object
	pair, which caused the error.
	If the object name is not available then a negative integer value
	is returned.
	>*/
	const __String& SubjectDesc(void) const; /*<
	Returns a textual description of the subject, which caused the error.
	If no description is available, then an empty __String is returned.
	>*/
};

//]

