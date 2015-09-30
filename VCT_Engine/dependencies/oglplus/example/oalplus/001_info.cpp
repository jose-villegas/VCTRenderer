/**
 *  @example oalplus/001_info.cpp
 *  @brief Shows the basic usage of OALplus by printing some info
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <oalplus/al.hpp>
#include <oalplus/all.hpp>

#include <iostream>

int main(void)
{
	// open the default device
	oalplus::Device device;
	// create a context using the device and make it current
	oalplus::ContextMadeCurrent context(device);
	//
	std::cout << "Vendor: " << context.Vendor() << std::endl;
	std::cout << "Version: " << context.Version() << std::endl;
	std::cout << "Renderer: " << context.Renderer() << std::endl;

	std::cout << "Device extensions:" << std::endl;
	for(auto r=device.Extensions(); !r.Empty(); r.Next())
	std::cout << '\t' << r.Front() << std::endl;

	std::cout << "Context extensions:" << std::endl;
	for(auto r=context.Extensions(); !r.Empty(); r.Next())
	std::cout << '\t' << r.Front() << std::endl;
	//
	return 0;
}
