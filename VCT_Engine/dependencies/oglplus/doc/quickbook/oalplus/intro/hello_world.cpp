/*
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//[oalplus_example_hello_world
#include <oalplus/al.hpp>
#include <oalplus/all.hpp>
#include <oalplus/alut.hpp>

#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
	oalplus::__Device device; /*<
	Creates a wrapper for and opens the default audio device.
	>*/

	oalplus::__ContextMadeCurrent context(device); /*<
	Creates a context using the device wrapper and makes it current.
	>*/

	oalplus::__ALUtilityToolkit alut(false, argc, argv); /*<
	Creates an instance of a wrapper of the ALUT library.
	>*/

	oalplus::__Listener listener; /*<
	Creates a listener and sets its position, velocity and orientation.
	>*/
	listener.Position(0.0f, 0.0f, 0.0f);
	listener.Velocity(0.0f, 0.0f, 0.0f);
	listener.Orientation(0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f);

	oalplus::__Buffer buffer = alut.CreateBufferHelloWorld(); /*<
	Creates a ['Hello World] sound and stores it into a buffer.
	>*/

	oalplus::__Source source; /*<
	Create an audio source from the data in buffer and sets its position.
	>*/
	source.Buffer(buffer);
	source.Position(0.0f, 0.0f,-1.0f);

	source.Play(); /*<
	Makes the source play the queued sound.
	>*/
	
	std::chrono::seconds duration(2); 
	std::this_thread::sleep_for(duration); /*<
	Waits for a couple of seconds before exitting.
	>*/

	return 0;
}
//]
