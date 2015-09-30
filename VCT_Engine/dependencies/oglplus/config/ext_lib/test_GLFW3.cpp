/**
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <GLFW/glfw3.h>

int main(void)
{
	if(!glfwInit()) return 1;

	GLFWwindow* win = glfwCreateWindow(64, 64, "GLFW test", NULL, NULL);
	if(!win)
	{
		glfwTerminate();
		return 2;
	}
	glfwMakeContextCurrent(win);

	glfwSwapBuffers(win);

	glfwTerminate();

	return 0;
}
