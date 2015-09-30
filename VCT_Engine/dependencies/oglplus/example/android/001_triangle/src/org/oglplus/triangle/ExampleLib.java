/**
 * Copyright 2013-2014 Matus Chochlik. Distributed under the Boost
 * Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

package org.oglplus.triangle;

public class ExampleLib
{
     static { System.loadLibrary("triangle"); }

     public static native void init();
     public static native void resize(int width, int height);
     public static native void step();
}
