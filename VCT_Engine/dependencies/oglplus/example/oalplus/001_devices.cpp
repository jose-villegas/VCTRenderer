/**
 *  @example oalplus/001_devices.cpp
 *  @brief Shows the basic usage of OALplus by printing names of audio devices
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
	std::cout << "Devices:" << std::endl;
	for(auto r=oalplus::Device::Specifiers(); !r.Empty(); r.Next())
	std::cout << '\t' << r.Front().c_str() << std::endl;
	//
	std::cout << "Capture devices:" << std::endl;
	for(auto r=oalplus::CaptureDevice::Specifiers(); !r.Empty(); r.Next())
	std::cout << '\t' << r.Front().c_str() << std::endl;
	//
	return 0;
}
