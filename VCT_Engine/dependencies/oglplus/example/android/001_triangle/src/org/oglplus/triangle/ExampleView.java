/**
 * Copyright 2013-2014 Matus Chochlik. Distributed under the Boost
 * Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

package org.oglplus.triangle;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class ExampleView extends GLSurfaceView
{
	private static final String TAG = "triangle";
	private static final boolean DEBUG = true;

	public ExampleView(Context context)
	{
		super(context);
		setEGLConfigChooser(8, 8, 8, 0, 16, 0);
		setEGLContextClientVersion(2);
		setRenderer(new ExampleRenderer());
	}

	private static class ExampleRenderer implements GLSurfaceView.Renderer
	{
		public void onDrawFrame(GL10 gl)
		{
			ExampleLib.step();
		}

		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			ExampleLib.resize(width, height);
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			ExampleLib.init();
		}
	}
}
