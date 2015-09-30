/*
 *  .file doc/tutorial/oalplus/001_hello.cpp
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#if OALPLUS_DOCUMENTATION_ONLY

/** @page oalplus_tut_001_hello Hello OpenAL
 *
 *  This tutorial explains the code of an @OALplus application that shows
 *  how to setup a listener and a sound source and play a sound using OpenAL.
 *  For a full working code see the @oalplus_example{oalplus,001_hello}
 *  file in the example directory.
 *
 *  @dontinclude oalplus/001_hello.cpp
 *
 *  First we include the @OALplus' header (that includes a header) defining
 *  the OpenAL API (types, constants, functions, etc.) This header (or some
 *  other header that defines the OpenAL API must be included before including
 *  any other @OALplus' headers.
 *  @until #include <oalplus/al.hpp>
 *
 *  Next we'll include the main header that pulls in everything from @OALplus
 *  except for the wrapper aroung the AL Utility Toolkit (ALUT),
 *  @until #include <oalplus/all.hpp>
 *
 *  followed by the header that includes ALUT (ALUT is a library that is usually
 *  distributed separately from OpenAL and because of this, the wrapper is also
 *  included separately from the rest of @OALplus):
 *  @until #include <oalplus/alut.hpp>
 *
 *  We will also need to put the main thread to sleep for a while and we use the
 *  classes and functions implemented in the standard library for this purpose,
 *  so we include the @c chrono and @c thread headers:
 *  @until #include <thread>
 *
 *  The main function of this example is pretty straightforward:
 *  @skipline int main
 *  @until {
 *
 *  First we create an instance of the @c Device class. We use here the default
 *  constructor that opens the default OpenAL device:
 *  @skipline oalplus::Device
 *
 *  Next we need to create an OpenAL context and make it current. This is done
 *  by constructing an instance of the @c CurrentContext class:
 *  @skipline oalplus::CurrentContext
 *
 *  We use the ALUT library to load the 'Hello world' sound that we are going
 *  to play, so we will need an instance of ALUtilityToolkit:
 *  @skipline oalplus::ALUtilityToolkit
 *
 *  Now we can set the listeners attributes; its position, orientation and
 *  velocity. In this example the listener is stationary, located at the origin
 *  and oriented to be looking in the direction of the negative-Z axis, while
 *  'up' is specified as the positive-Y axis.
 *  @skipline oalplus::Listener
 *  @until listener.Orientation
 *
 *  In order to be able to do audio playback we need the sound samples
 *  and store them in a Buffer object. In this example the ALUT's
 *  @c CreateBufferHelloWorld function is used to load the sound samples
 *  and store them in a Buffer object:
 *  @skipline oalplus::Buffer
 *
 *  Next we need to create a Source object,
 *  @skipline oalplus::Source
 *
 *  enqueue the buffer containing the sound sample data to the source's internal
 *  queue,
 *  @skipline source.Buffer
 *
 *  set its position (in this example the source is also stationary and located
 *  at (0, 0,-1):
 *  @skipline source.Position
 *
 *  and now we can tell the source to start playing the enqueued sounds:
 *  @skipline source.Play
 *
 *  After starting the playback we wait for 2 seconds for it to finish,
 *  @skipline std::chrono::seconds
 *  @until std::this_thread::sleep
 *
 *  and the application can finish its execution.
 *  @skipline return 0
 *  @until }
 */

#endif // OALPLUS_DOCUMENTATION_ONLY
