/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_Error_1

class Error
 : public std::runtime_error
{
public:
	static const char* Message(GLenum error_code); /*<
	Returns a message for the specified [^error_code].
	>*/

	Error(const char* message); /*<
	Constructs a new instance of [^Error] with the specified
	error [^message].
	>*/
	~Error(void) throw() { }

	__ErrorCode Code(void) const; /*<
	Return the GL error code associated with this error.
	>*/

	const char* SourceFile(void) const; /*<
	Returns the name of the OGLplus source file where the error occured.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_FILE preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns a nullptr.
	>*/
	const char* SourceFunc(void) const; /*<
	Returns the name of the OGLplus function where the error occured.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_FUNC preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns a nullptr.
	>*/
	unsigned SourceLine(void) const; /*<
	Returns the line of the OGLplus source file where the error occured.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_LINE preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns zero.
	>*/

	const char* GLLib(void) const; /*<
	This function returns the name of the GL library ([^"gl"], [^"wgl"],
	[^"glX"]) where the error occured.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_GL_LIB preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns nullptr.
	>*/
	const char* GLFunc(void) const; /*<
	This function returns the name of the failed OpenGL function
	(without the [^gl] prefix) which is related to the error.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_GL_FUNC preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns nullptr.
	>*/

	GLenum EnumParam(void) const; /*<
	Returns the value of the enumeration parameter related to the error.
	If no enum parameter is available, this function returns zero.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_GL_SYMBOL preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns zero.
	>*/
	const char* EnumParamName(void) const; /*<
	Returns the name of the enumeration parameter related to the error.
	If the enum parameter name is not available, this function returns
	zero.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_GL_SYMBOL preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns nullptr.
	>*/

	GLint Index(void) const; /*<
	Returns the value of an index parameter passed to the failed OpenGL
	function if applicable.
	If no index value is available then this function return a negative
	integer.
	The result of this function is also influenced by the
	__OGLPLUS_ERROR_NO_GL_SYMBOL preprocessor configuration option.
	If set to zero, this function behaves as described above, otherwise it
	returns a negative integer.
	>*/
//]
//[oglplus_error_Error_2
	virtual GLfloat Value(void) const; /*<
	Returns a value parameter related to the error, if applicable,
	returns zero otherwise.
	>*/
	virtual GLfloat Limit(void) const; /*<
	Returns a limit value related to the error, if applicable,
	returns zero otherwise.
	>*/

	virtual GLenum BindTarget(void) const; /*<
	Returns the object bind target, if applicable, returns zero otherwise.
	>*/
	virtual const char* TargetName(void) const; /*<
	Returns the object bind target name, if applicable, return nullptr
	otherwise.
	>*/

	virtual GLenum ObjectType(void) const; /*<
	If the error is related to a GL object, then an object
	type enumeration value is returned. Otherwise the result is zero.
	>*/
	virtual const char* ObjectTypeName(void) const; /*<
	If the error is related to a GL object, then a C string
	storing object type name is returned. Otherwise the result
	is nullptr.
	>*/
	virtual GLint ObjectName(void) const; /*<
	If the error is related to a GL object, then the numeric
	GL name of the object is returned. Otherwise the result
	is a negative integer.
	>*/
	virtual const std::string& ObjectDesc(void) const; /*<
	If the error is related to a GL object, then a std::string
	storing object description is returned. Otherwise the result
	is an empty std::string.
	>*/

	virtual GLenum SubjectType(void) const; /*<
	If the error is related to a pair of GL objects, then
	an object type enumeration value is returned. Otherwise
	the result is zero.
	>*/
	virtual const char* SubjectTypeName(void) const; /*<
	If the error is related a pair of GL objects, then a C string
	storing secondary object type name is returned. Otherwise the result
	is nullptr.
	>*/
	virtual GLint SubjectName(void) const; /*<
	If the error is related to a pair of GL objects, then
	the numeric GL name of the secondary object is returned.
	Otherwise the result is a negative integer.
	>*/
	virtual const std::string& SubjectDesc(void) const; /*<
	If the error is related to a pair of GL objects, then a std::string
	storing the secondary object description is returned. Otherwise
	the result is an empty std::string.
	>*/

	virtual const char* Identifier(void) const; /*<
	If the error is related to a GPU program variable (vertex attrib,
	uniform, subroutine, etc.) then this function returns a C string
	storing the identifier of the variable. Otherwise the result
	is nullptr.
	>*/

	virtual const String& Log(void) const; /*<
	If the error was caused by a process (like shader compilation,
	program linking or validation, etc.) which creates a textual
	log and it is available then it is returned by this function.
	Otherwise the result is an empty String.
	>*/
};

//]

