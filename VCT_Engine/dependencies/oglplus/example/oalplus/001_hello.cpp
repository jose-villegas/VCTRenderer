/**
 *  @example oalplus/001_hello.cpp
 *  @brief Shows the basic usage of OALplus by playing a sound
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oalplus_example_uses_cpp_feat{THREADS}
 *  @oalplus_example_uses_cpp_feat{CHRONO}
 */

#include <oalplus/al.hpp>
#include <oalplus/all.hpp>
#include <oalplus/alut.hpp>

#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
	// open the default device
	oalplus::Device device;
	// create a context using the device and make it current
	oalplus::ContextMadeCurrent context(device);
	// create an instance of ALUT
	oalplus::ALUtilityToolkit alut(false, argc, argv);
	// create a listener and set its position, velocity and orientation
	oalplus::Listener listener;
	listener.Position(0.0f, 0.0f, 0.0f);
	listener.Velocity(0.0f, 0.0f, 0.0f);
	listener.Orientation(0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f);
	// create a Hello World sound and store it into a buffer
	oalplus::Buffer buffer = alut.CreateBufferHelloWorld();
	// create a source from the data in buffer and set its position
	oalplus::Source source;
	source.Buffer(buffer);
	source.Position(0.0f, 0.0f,-1.0f);
	// let the source play the sound
	source.Play();
	// wait for a while
	std::chrono::seconds duration(2);
	std::this_thread::sleep_for(duration);
	//
	return 0;
}
