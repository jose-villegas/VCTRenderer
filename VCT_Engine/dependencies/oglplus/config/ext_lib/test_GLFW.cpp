/**
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <GL/glfw.h>

int main(void)
{
	if(!glfwInit()) return 1;

	if(!glfwOpenWindow(64, 64, 8, 8, 8, 8, 32, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return 2;
	}
	glfwSetWindowTitle("GLFW test");

	glfwSwapBuffers();

	glfwTerminate();

	return 0;
}
