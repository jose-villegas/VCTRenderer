/**
 *  @file oglplus/example.hpp
 *  @brief Declares a common base class for examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef __OGLPLUS_EXAMPLE_EXAMPLE_1119071146_HPP__
#define __OGLPLUS_EXAMPLE_EXAMPLE_1119071146_HPP__

#include <set>
#include <memory>
#include <oglplus/assert.hpp>

#ifndef _NDEBUG
#include <iostream>
#endif

#ifdef _MSC_VER
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4305 )
#endif //_MSC_VER

namespace oglplus {

/// Run-time parameters for example instances
struct ExampleParams
{
	/// the number of arguments passed on command-line
	int argc;

	/// the arguments strings
	char ** argv;

	/// The quality of rendered image (0.0 = low, 0.5 = default, 1.0 = high)
	float quality;

	/// The number of available GPUs
	unsigned num_gpus;

	/// The number of offscreen rendering threads (not counting the main thread)
	unsigned num_threads;

	/// The maximum number of threads
	unsigned max_threads;

	/// The set of threads for which a compatibility context should be created
	std::set<unsigned> compat_context_threads;

	ExampleParams(int argn, char ** args)
	 : argc(argn)
	 , argv(args)
	 , quality(0.5f)
	 , num_gpus(1)
	 , num_threads(0)
	 , max_threads(0)
	{ }

	void Check(void)
	{
		assert(num_threads <= max_threads);
	}

	bool HighQuality(void) const
	{
		return quality > 0.9f;
	}
};

void setupExample(ExampleParams& params);

/// A class measuring a time period in the examples
class ExampleTimePeriod
{
private:
	double _time;
public:
	ExampleTimePeriod(double time)
	 : _time(time)
	{
		assert(_time >= 0.0);
	}

	/// The length of the periods in seconds
	double Seconds(void) const
	{
		return _time;
	}

	/// The current second of the period <0-60)
	int Second(void) const
	{
		return int(Seconds()) % 60;
	}

	/// The length of the periods in minutes
	double Minutes(void) const
	{
		return _time / 60.0;
	}

	/// The current minute of the period <0-60)
	int Minute(void) const
	{
		return int(Minutes()) % 60;
	}

	/// The length of the periods in hours
	double Hours(void) const
	{
		return _time / 3600.0;
	}

	/// The current minute of the period <0-24)
	int Hour(void) const
	{
		return int(Hours()) % 24;
	}

	/// The length of the period in days
	double Days(void) const
	{
		return _time / (24*3600.0);
	}

	/// The current day of the period
	int Day(void) const
	{
		return int(Days());
	}
};

/// Class measuring the simulation time of an Example
class ExampleClock
{
private:
	double _start, _past, _real_time, _curr_time, _prev_time, _pace; //[s]
public:
	ExampleClock(double start = 0.0)
	 : _start(start)
	 , _past(start)
	 , _real_time(start)
	 , _curr_time(start)
	 , _prev_time(start)
	 , _pace(1.0)
	{ }

	/// Update the clock by providing real time
	void Update(double real_time)
	{
		_prev_time = _curr_time;
		_real_time = real_time;
		_curr_time = _past + (_real_time - _start) * _pace;
	}

	/// Advances the clock
	void Advance(double seconds)
	{
		_prev_time = _curr_time;
		_real_time += seconds;
		_curr_time = _past + (_real_time - _start) * _pace;
	}

	/// Set the pace by which the sim. time advances compared to real-time
	void Pace(double pace)
	{
		if(_pace != pace)
		{
			_start = _real_time;
			_past = _curr_time;
			_pace = pace;
		}
	}

	/// Equivalent to Now().Seconds()
	double Time(void) const
	{
		return this->Now().Seconds();
	}

	/// Returns the real time elapsed since the start of the example
	ExampleTimePeriod RealTime(void) const
	{
		return ExampleTimePeriod(_real_time);
	}

	/// Returns the simulation time elapsed since the start of the example
	ExampleTimePeriod Now(void) const
	{
		return ExampleTimePeriod(_curr_time);
	}

	/// Returns the time elapsed between the last and the previous Update
	ExampleTimePeriod Interval(void) const
	{
		return ExampleTimePeriod(_curr_time-_prev_time);
	}
};

class Example;

/// Base class for OGLplus example offscreen rendering threads
class ExampleThread
{
public:
	virtual ~ExampleThread(void)
	{ }

	/// Notifies the thread that the example is closing
	/** Implementations of ExampleThread should cancel
	 *  all rendering operations and quit the Render
	 *  function ASAP.
	 */
	virtual void Cancel(void){ }

	/// Rendering procedure with simple timing
	virtual void Render(double /*time*/)
	{
		OGLPLUS_ABORT("Render must be overloaded by examples!");
	}

	/// Rendering procedure with advanced timing
	virtual void Render(const ExampleClock& clock)
	{
		this->Render(clock.Now().Seconds());
	}

	/// Rendering procedure split into several parts
	virtual double RenderPart(
		unsigned /*part_no*/,
		const ExampleClock& clock
	)
	{
		this->Render(clock);
		return 1.0;
	}
};

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& example,
	unsigned thread_id,
	const ExampleParams& params
);

/// Base class for OGLplus examples
class Example
{
public:
	virtual ~Example(void)
	{ }

	/// Place to do additional thread-related initialization of example
	/** This function is called in the context of the main thread
	 *  after all example threads are created, before the rendering
	 *  loop starts.
	 */
	virtual void PrepareThread(unsigned, ExampleThread&)
	{
	}

	/// Hint for the main function whether to continue rendering
	/** Implementations of the main function may choose to ignore
	 *  the result of this function or not call it at all.
	 *  This is the overload for simple timing.
	 */
	virtual bool Continue(double duration)
	{
		return duration < 3.0; // [seconds]
	}

	/// Hint for the main function whether to continue rendering
	/** Implementations of the main function may choose to ignore
	 *  the result of this function or not call it at all.
	 *  This is the overload for advanced timing.
	 */
	virtual bool Continue(const ExampleClock& clock)
	{
		return this->Continue(clock.Now().Seconds());
	}

	/// Reshape event handler
	virtual void Reshape(GLuint width, GLuint height) = 0;

	/// Mouse move event handler
	virtual void MouseMoveNormalized(float x, float y, float aspect)
	{
		(void)(x+y+aspect);
	}

	/// Mouse move event handler
	virtual void MouseMove(GLuint x, GLuint y, GLuint width, GLuint height)
	{
		return MouseMoveNormalized(
			(float(x) - width * 0.5f) / (width * 0.5f),
			(float(y) - height* 0.5f) / (height* 0.5f),
			float(width)/height
		);
	}

	/// Rendering procedure with simple timing
	virtual void Render(double /*time*/)
	{
		OGLPLUS_ABORT("Render must be overloaded by examples!");
	}

	/// Rendering procedure with advanced timing
	virtual void Render(ExampleClock& clock)
	{
		this->Render(clock.Now().Seconds());
	}

	/// Rendering procedure split into several parts
	virtual double RenderPart(unsigned /*part_no*/, ExampleClock& clock)
	{
		this->Render(clock);
		return 1.0;
	}

	/// The time of the default screenshot
	virtual double ScreenshotTime(void) const
	{
		return 1.0; // [s]
	}

	/// Single frame time in the screenshot capture sequence
	virtual double FrameTime(void) const
	{
		return 1.0/25.0;
	}

	/// The number of heat-up sequence frames
	virtual GLuint HeatUpFrames(void) const
	{
		return 5;
	}

	/// The screenshot capture heat-up sequence start time
	virtual double HeatUpTime(void) const
	{
		return ScreenshotTime()-HeatUpFrames()*FrameTime();
	}
};

std::unique_ptr<Example> makeExample(const ExampleParams& params);

} // namespace oglplus

#endif // include guard
