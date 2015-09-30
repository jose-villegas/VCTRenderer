/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_face_mode_fns

__Face Opposite(__Face facing); /*<
Returns the opposite facing direction (Front if Back and Back if Front).
>*/

__FaceOrientation Inverted(__FaceOrientation winding); /*<
Inverts the face orientation (CW to CCW and CCW to CW).
>*/

//]

