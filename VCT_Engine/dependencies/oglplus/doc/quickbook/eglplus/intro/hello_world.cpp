/*
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//[eglplus_example_hello_world_1
#include <eglplus/egl.hpp>
#include <eglplus/all.hpp>

#include <iostream>
//]
//[eglplus_example_hello_world_2
int main(void)
{
	if(eglplus::__LibEGL::HasClientExtensions())
	{
		std::cout << "Client extensions:" << std::endl;
		for(auto r=eglplus::__LibEGL::ClientExtensions(); !r.Empty(); r.Next())
			std::cout << '\t' << r.Front() << std::endl;
	}
	else std::cout << "No client extensions." << std::endl;

	eglplus::__Display display;

	eglplus::__LibEGL egl(display);

	std::cout << "Vendor: " << egl.Vendor() << std::endl;
	std::cout << "Version: " << egl.Version() << std::endl;

	std::cout << "Client APIs:" << std::endl;
	for(auto r=egl.ClientAPIs(); !r.Empty(); r.Next())
		std::cout << '\t' << r.Front() << std::endl;

	std::cout << "Extensions:" << std::endl;
	for(auto r=egl.Extensions(); !r.Empty(); r.Next())
		std::cout << '\t' << r.Front() << std::endl;

	return 0;
}//]
