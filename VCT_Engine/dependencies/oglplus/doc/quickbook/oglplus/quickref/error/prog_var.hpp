/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_ProgVarError

class ProgVarError
 : public __Error
{
public:
	ProgVarError(const char* message);

	const char* ObjectTypeName(void); /*<
	Returns the string [^"PROGRAM"].
	>*/
	GLint ObjectName(void) const; /*<
	Returns the ['raw] GL name of the program related to this error.
	>*/
	__ProgramName Program(void) const; /*<
	Returns the name of the program related to this error.
	>*/

	const char* Identifier(void) const; /*<
	Returns a c-string containing the identifier of the GPU program
	variable related to this error.
	>*/
};

//]

