/**
 *  @example oalplus/002_hello_path.cpp
 *  @brief Shows the basic usage of OALplus by playing a sound
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oalplus_example_uses_cpp_feat{THREADS}
 *  @oalplus_example_uses_cpp_feat{CHRONO}
 *  @oalplus_example_uses_cpp_feat{INITIALIZER_LISTS}
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
	oalplus::ContextMadeCurrent context(
		device,
		oalplus::ContextAttribs()
			.Add(oalplus::ContextAttrib::MonoSources, 1)
			.Get()
	);
	// create an instance of ALUT
	oalplus::ALUtilityToolkit alut(false, argc, argv);
	// create a listener and set its position, velocity and orientation
	oalplus::Listener listener;
	listener.Position(0.0f, 0.0f, 0.0f);
	listener.Velocity(0.0f, 0.0f, 0.0f);
	listener.Orientation(0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f);
	// create a Hello World sound and store it into a buffer
	oalplus::Buffer buffer = alut.CreateBufferHelloWorld();
	// create a source from the data in buffer and set enable looping
	oalplus::Source source(oalplus::ObjectDesc("Source"));
	source.Buffer(buffer);
	source.Looping(true);
	source.ReferenceDistance(15);
	// let the source play the sound
	source.Play();
	// the path of the source
	oalplus::CubicBezierLoop<oalplus::Vec3f, double> path({
		oalplus::Vec3f( 0.0f, 0.0f, -9.0f),
		oalplus::Vec3f( 9.0f, 2.0f, -8.0f),
		oalplus::Vec3f( 4.0f,-3.0f,  9.0f),
		oalplus::Vec3f(-3.0f, 4.0f,  9.0f),
		oalplus::Vec3f(-9.0f,-1.0f, -7.0f)
	});
	//
	// play the sound for a while
	typedef std::chrono::system_clock clock;
	typedef std::chrono::time_point<clock> time_point;

	time_point start = clock::now();

	while(true)
	{
		double t = double((clock::now() - start).count())*
			double(clock::period::num)/
			double(clock::period::den);
		if(t > 10.0) break;
		source.Position(path.Position(t/5.0));
		// wait for a while
		std::chrono::milliseconds duration(10);
		std::this_thread::sleep_for(duration);
	}
	//
	return 0;
}
