/**
 * Copyright 2013-2014 Matus Chochlik. Distributed under the Boost
 * Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

package org.oglplus.triangle;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

public class ExampleActivity extends Activity
{
	ExampleView view;

	@Override protected void onCreate(Bundle icicle)
	{
		super.onCreate(icicle);
		view = new ExampleView(getApplication());
		setContentView(view);
	}

	@Override protected void onPause()
	{
		super.onPause();
		view.onPause();
	}

	@Override protected void onResume()
	{
		super.onResume();
		view.onResume();
	}
}
