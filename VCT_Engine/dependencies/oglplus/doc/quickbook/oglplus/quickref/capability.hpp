/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_capability_syntax_sugar

void operator << (__Capability capability, bool enable); /*<
Enables or disables the specified [^capability].
>*/
void operator + (Capability capability); /*<
Enables the specified [^capability].
>*/
void operator - (Capability capability); /*<
Disables the specified [^capability].
>*/

struct FunctionalityAndNumber { }; /*<
An opaque helper type used by the [^Functionality] operators below.
>*/

FunctionalityAndNumber operator | (__Functionality func, GLuint number); /*<
Creates an instance of [^FunctionalityAndNumber].
>*/

void operator << (FunctionalityAndNumber func_and_num, bool enable); /*<
Enables or disables the specified functionality.
>*/
void operator + (FunctionalityAndNumber func_and_num); /*<
Enables the specified functionality.
>*/
void operator - (FunctionalityAndNumber func_and_num); /*<
Disables the specified functionality.
>*/

//]

