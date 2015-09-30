/*
 *  .file example/oglplus/glx_main.cpp
 *  Implements GLX-based program main function for running examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <oglplus/glx/context.hpp>
#include <oglplus/glx/fb_configs.hpp>
#include <oglplus/glx/version.hpp>
#include <oglplus/glx/pixmap.hpp>
#include <oglplus/x11/window.hpp>
#include <oglplus/x11/color_map.hpp>
#include <oglplus/x11/visual_info.hpp>
#include <oglplus/x11/display.hpp>

#include <oglplus/os/steady_clock.hpp>

#include <oglplus/math/vector.hpp>
#include <oglplus/math/curve.hpp>

#include <oglplus/ext/ARB_debug_output.hpp>

#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include <thread>
#include <mutex>
#include <condition_variable>

#include "example.hpp"
#include "example_main.hpp"

namespace oglplus {

class ThreadSemaphore
{
private:
	unsigned _value;
	std::mutex _mutex;
	std::condition_variable _cv;

	void _decr(void)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		while(_value == 0) _cv.wait(lock);
		--_value;
	}

	void _incr(unsigned n)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_value += n;
		_cv.notify_all();
	}
public:
	ThreadSemaphore(unsigned initial = 0)
	 : _value(initial)
	{ }

	void Wait(unsigned n = 1)
	{
		while(n--) _decr();
	}

	void Signal(unsigned n = 1)
	{
		_incr(n);
	}
};

struct ExampleThreadData
{
	unsigned thread_index;
	ExampleThread* example_thread;
	std::string error_message;

	struct Common
	{
		Example* example;
		const ExampleParams& example_params;
		const ExampleClock& clock;
		const x11::ScreenNames& screen_names;
		const x11::Display& display;
		const glx::Context& ctx;
		ThreadSemaphore& thread_ready;
		ThreadSemaphore& master_ready;
		bool failure;
		bool done;
	}* _pcommon;
	Common& common(void)
	{
		assert(_pcommon);
		return *_pcommon;
	}
};

struct ExampleOptions
{
	const char* screenshot_path;
	const char* framedump_prefix;
	GLuint width;
	GLuint height;
	GLint samples;
};

void example_thread_main(ExampleThreadData& data)
{
	const ExampleThreadData::Common& common = data.common();
	const x11::ScreenNames& sn = common.screen_names;

	// pick one of the available display screens
	// for this thread
	std::size_t disp_idx = data.thread_index;
	if(sn.size() > 1) ++disp_idx;
	disp_idx %= sn.size();
	// open the picked display
	x11::Display display(sn[disp_idx].c_str());

	// initialize the pixelmaps and the sharing context
	static int visual_attribs[] =
	{
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_PIXMAP_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		None
	};
	glx::FBConfig fbc = glx::FBConfigs(
		display,
		visual_attribs
	).FindBest(display);

	x11::VisualInfo vi(display, fbc);

	x11::Pixmap xpm(display, vi, 8, 8);
	glx::Pixmap gpm(display, vi, xpm);

	bool debugging = true;
	bool compatibility = common
		.example_params
		.compat_context_threads
		.count(data.thread_index) != 0;

	glx::Context ctx(
		display,
		fbc,
		common.ctx,
		3, 3,
		debugging,
		compatibility
	);

	ctx.MakeCurrent(gpm);

	// signal that the context is created
	common.thread_ready.Signal();

	// wait for the example to be created
	// in the main thread
	common.master_ready.Wait();
	// if something failed - quit
	if(common.failure)
	{
		common.thread_ready.Signal();
		return;
	}
	else
	{
		try
		{
			assert(common.example);
			// call makeExampleThread
			std::unique_ptr<ExampleThread> example_thread(
				makeExampleThread(
					*common.example,
					data.thread_index,
					common.example_params
				)
			);
			data.example_thread = example_thread.get();
			// signal that it is created
			common.thread_ready.Signal();
			// wait for the main thread
			common.master_ready.Wait();
			// if something failed - quit
			if(common.failure) return;

			// start rendering
			while(!common.done && !common.failure)
			{
				unsigned part_no = 0;
				double comp = 0.0;
				do
				{
					comp = example_thread->RenderPart(
						part_no++,
						common.clock
					);
					glFlush();
				}
				while(comp < 1.0);
			}
			data.example_thread = nullptr;
		}
		catch(...)
		{
			data.example_thread = nullptr;
			throw;
		}
	}
	ctx.Release(display);
}

void call_example_thread_main(ExampleThreadData& data)
{
	ExampleThreadData::Common& common = data.common();
	struct main_wrapper
	{
		void (*main_func)(ExampleThreadData&);
		ExampleThreadData& data;

		int operator()(void) const
		{
			main_func(data);
			return 0;
		}
	} wrapped_main = {example_thread_main, data};

	std::stringstream errstr;
	if(example_guarded_exec(wrapped_main, errstr) != 0)
	{
		common.failure = true;
		data.error_message = errstr.str();
		common.thread_ready.Signal();
	}
}

void do_run_example_loop(
	const x11::Display& display,
	const x11::Window& win,
	const glx::Context& ctx,
	std::unique_ptr<Example>& example,
	ExampleThreadData::Common& common,
	ExampleClock& clock,
	ExampleOptions& opts
)
{
	win.SelectInput(
		StructureNotifyMask|
		PointerMotionMask|
		KeyPressMask
	);

	XEvent event;
	os::steady_clock os_clock;
	bool done = false;
	while(!done && !common.failure)
	{
		clock.Update(os_clock.seconds());
		if(!example->Continue(clock)) break;

		unsigned part_no = 0;
		double comp = 0.0;
		do
		{
			while(display.NextEvent(event))
			{
				switch(event.type)
				{
					case ClientMessage:
					case DestroyNotify:
						done = true;
						break;
					case ConfigureNotify:
						opts.width = GLuint(event.xconfigure.width);
						opts.height = GLuint(event.xconfigure.height);
						example->Reshape(
							opts.width,
							opts.height
						);
						break;
					case MotionNotify:
						example->MouseMove(
							GLuint(event.xmotion.x),
							opts.height-
							GLuint(event.xmotion.y),
							opts.width,
							opts.height
						);
						break;
					case KeyPress:
						if(::XLookupKeysym(
							&event.xkey,
							0
						) == XK_Escape) done = true;
						break;
					default:;
				}
			}
			comp = example->RenderPart(part_no++, clock);
			ctx.SwapBuffers(win);
		}
		while(!done && !common.failure && (comp < 1.0));
	}
}

void run_example_loop(
	const x11::Display& display,
	const x11::Window& win,
	const glx::Context& ctx,
	std::unique_ptr<Example>& example,
	ExampleThreadData::Common& common,
	ExampleClock& clock,
	ExampleOptions& opts
)
{
#if GL_ARB_debug_output
	ARB_debug_output dbg(false); // don't throw
	if(dbg.Available())
	{
		ARB_debug_output_ToXML dbgprn(std::cout);
		ARB_debug_output_Unique dbgcb(dbgprn);
		ARB_debug_output::LogSink sink(dbgcb);

		dbg.Control(
			DebugOutputARBSource::DontCare,
			DebugOutputARBType::DontCare,
			DebugOutputARBSeverity::Low,
			true
		);

		dbg.InsertMessage(
			DebugOutputARBSource::Application,
			DebugOutputARBType::Other,
			0,
			DebugOutputARBSeverity::Low,
			"Starting main loop"
		);
		do_run_example_loop(
			display,
			win,
			ctx,
			example,
			common,
			clock,
			opts
		);
	}
	else
#endif // GL_ARB_debug_output
	{
		do_run_example_loop(
			display,
			win,
			ctx,
			example,
			common,
			clock,
			opts
		);
	}
}

void run_framedump_loop(
	const x11::Display& display,
	const x11::Window& win,
	const glx::Context& ctx,
	std::unique_ptr<Example>& example,
	ExampleClock& clock,
	const ExampleOptions& opts
)
{
	std::vector<char> txtbuf(1024);
	std::cin.getline(txtbuf.data(), std::streamsize(txtbuf.size()));
	if(std::strcmp(opts.framedump_prefix, txtbuf.data()) != 0) return;

	const std::size_t mouse_path_pts = 7;
	std::vector<Vec2f> mouse_path_pos(mouse_path_pts);
	std::vector<Vec2f> mouse_path_dir(mouse_path_pts);

	for(std::size_t p=0; p!= mouse_path_pts; ++p)
	{
		mouse_path_pos[p] = Vec2f(
			unsigned(std::rand()) % opts.width,
			unsigned(std::rand()) % opts.height
		);
		mouse_path_dir[p] = Vec2f(
			(std::rand()%2?1:-1)*10.0f*
			(0.2f+float(std::rand())/float(RAND_MAX)*0.8f),
			(std::rand()%2?1:-1)*10.0f*
			(0.2f+float(std::rand())/float(RAND_MAX)*0.8f)
		);
	}

	typedef CubicBezierLoop<Vec2f, double> Loop;

	double t = 0.0;
	double period = 1.0 / 25.0;
	GLuint frame_no = 0;
	std::vector<char> pixels(opts.width * opts.height * 4);

	GLuint border = 32;
	bool done = false;

	XEvent event;

	while(!done)
	{
		while(display.NextEvent(event))
		{
			switch(event.type)
			{
				case ClientMessage:
				case DestroyNotify:
					done = true;
					break;
			}
		}

		Vec2f mouse_pos = Loop(mouse_path_pos).Position(t*0.2);

		for(std::size_t p=0; p!= mouse_path_pts; ++p)
		{
			Vec2f dir = mouse_path_dir[p];
			Vec2f pos = mouse_path_pos[p];

			if((pos.x() < border) && (dir.x() < 0.0))
				dir = Vec2f(-dir.x(), dir.y());
			if((pos.y() < border) && (dir.y() < 0.0))
				dir = Vec2f( dir.x(),-dir.y());
			if((pos.x() > opts.width-border) && (dir.x() > 0.0))
				dir = Vec2f(-dir.x(), dir.y());
			if((pos.y() > opts.height-border) && (dir.y() > 0.0))
				dir = Vec2f( dir.x(),-dir.y());

			mouse_path_dir[p] = dir;
			mouse_path_pos[p] = pos + dir;
		}

		float mouse_x = mouse_pos.x();
		float mouse_y = mouse_pos.y();

		if(mouse_x < 0.0f) mouse_x = 0.0f;
		if(mouse_y < 0.0f) mouse_y = 0.0f;
		if(mouse_x > opts.width) mouse_x = opts.width;
		if(mouse_y > opts.height) mouse_y = opts.height;

		example->MouseMove(
			GLuint(mouse_x),
			GLuint(mouse_y),
			opts.width,
			opts.height
		);

		t += period;
		clock.Update(t);
		if(!example->Continue(clock)) break;

		unsigned part_no = 0;
		double comp = 0.0;
		do
		{
			comp = example->RenderPart(part_no++, clock);
		}
		while(comp < 1.0);

		glFinish();
		glReadPixels(
			0, 0,
			GLsizei(opts.width),
			GLsizei(opts.height),
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixels.data()
		);
		glFinish();
		ctx.SwapBuffers(win);
		std::stringstream filename;
		filename <<
			opts.framedump_prefix <<
			std::setfill('0') << std::setw(6) <<
			frame_no << ".rgba";
		{
			std::ofstream file(filename.str());
			file.write(pixels.data(), std::streamsize(pixels.size()));
			file.flush();
		}
		std::cout << filename.str() << std::endl;
		++frame_no;

		txtbuf.resize(filename.str().size()+1);
		std::cin.getline(txtbuf.data(), std::streamsize(txtbuf.size()));

		if(std::strncmp(
			filename.str().c_str(),
			txtbuf.data(),
			txtbuf.size()
		) != 0) break;
	}
	while(display.NextEvent(event));
}

void make_screenshot(
	const x11::Display& display,
	const x11::Window& win,
	const glx::Context& ctx,
	std::unique_ptr<Example>& example,
	ExampleClock& clock,
	const ExampleOptions& opts
)
{
	XEvent event;

	double s = example->HeatUpTime();
	double t = example->ScreenshotTime();
	double dt = example->FrameTime();

	clock.Update(s);

	// heat-up
	while(s < t)
	{
		while(display.NextEvent(event));
		s += dt;
		clock.Update(s);

		unsigned part_no = 0;
		double comp = 0.0;
		do
		{
			comp = example->RenderPart(part_no++, clock);
			if(s < t) ctx.SwapBuffers(win);
		}
		while(comp < 1.0);
	}
	while(display.NextEvent(event));
	glFinish();
	//save it to a file
	std::vector<char> pixels(opts.width * opts.height * 3);
	glReadPixels(
		0, 0,
		GLsizei(opts.width),
		GLsizei(opts.height),
		GL_RGB,
		GL_UNSIGNED_BYTE,
		pixels.data()
	);
	std::ofstream output(opts.screenshot_path);
	output.write(pixels.data(), std::streamsize(pixels.size()));
	ctx.SwapBuffers(win);
}

void run_example(
	const x11::Display& display,
	ExampleOptions& opts,
	int argc,
	char ** argv
)
{
	glx::Version version(display);
	version.AssertAtLeast(1, 3);

	static int visual_attribs[] =
	{
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		GLX_SAMPLE_BUFFERS  , (opts.samples>0)?1:0,
		GLX_SAMPLES         , (opts.samples>0)?opts.samples:0,
		None
	};
	glx::FBConfig fbc = glx::FBConfigs(
		display,
		visual_attribs
	).FindBest(display);

	x11::VisualInfo vi(display, fbc);


	x11::Window win(
		display,
		vi,
		x11::Colormap(display, vi),
		"OGLplus example",
		opts.width, opts.height
	);

	x11::ScreenNames screen_names;

	ExampleParams params(argc, argv);
	if(opts.framedump_prefix)
	{
		params.quality = 1.0;
	}
	params.max_threads = 128;
	params.num_gpus = unsigned(screen_names.size()); // TODO: something more reliable
	setupExample(params);
	params.Check();

	glx::Context ctx(display, fbc, 3, 3);

	ctx.MakeCurrent(win);

	// The clock for animation timing
	ExampleClock clock;

	std::vector<std::thread> threads;
	ThreadSemaphore thread_ready, master_ready;
	ExampleThreadData::Common example_thread_common_data = {
		nullptr,
		params,
		clock,
		screen_names,
		display,
		ctx,
		thread_ready,
		master_ready,
		false /*failure*/,
		false /*done*/
	};
	try
	{
		// Initialize the GL API library (GLEW/GL3W/...)
		oglplus::GLAPIInitializer api_init;

		// things required for multi-threaded examples
		std::vector<ExampleThreadData> thread_data;

		// prepare the example data
		for(unsigned t=0; t!=params.num_threads; ++t)
		{
			ExampleThreadData example_thread_data = {
				t, nullptr, std::string(),
				&example_thread_common_data
			};
			thread_data.push_back(example_thread_data);
		}

		// start the examples and let them
		// create their own contexts shared with
		// the main context
		for(unsigned t=0; t!=params.num_threads; ++t)
		{
			threads.emplace_back(
				call_example_thread_main,
				std::ref(thread_data[t])
			);
			// wait for the thread to create
			// an off-screen context
			thread_ready.Wait();
			// check for errors
			if(!thread_data[t].error_message.empty())
			{
				example_thread_common_data.failure = true;
				example_thread_common_data.master_ready.Signal(t);
				throw std::runtime_error(
					thread_data[t].error_message
				);
			}
		}

		// make the example
		std::unique_ptr<Example> example(makeExample(params));

		// tell the threads about the example
		// and let them call makeExampleThread
		example_thread_common_data.example = example.get();
		for(unsigned t=0; t!=params.num_threads; ++t)
		{
			// signal that the example is ready
			master_ready.Signal();
			// wait for the threads to call makeExampleThread
			thread_ready.Wait();
		}
		// check for potential errors and let
		// the example do additional thread-related preparations
		for(unsigned t=0; t!=params.num_threads; ++t)
		{
			if(!thread_data[t].error_message.empty())
			{
				example_thread_common_data.failure = true;
				example_thread_common_data.master_ready.Signal(
					params.num_threads
				);
				throw std::runtime_error(
					thread_data[t].error_message
				);
			}
			assert(thread_data[t].example_thread);
			example->PrepareThread(t,*thread_data[t].example_thread);
		}
		// signal that the example threads may start
		// rendering
		master_ready.Signal(params.num_threads);

		if(opts.samples>0)
		{
			// enable multisampling
			glEnable(GL_MULTISAMPLE);
		}

		example->Reshape(opts.width, opts.height);
		example->MouseMove(opts.width/2, opts.height/2, opts.width, opts.height);

		if(opts.screenshot_path)
		{
			make_screenshot(
				display,
				win,
				ctx,
				example,
				clock,
				opts
			);
		}
		else if(opts.framedump_prefix)
		{
			run_framedump_loop(
				display,
				win,
				ctx,
				example,
				clock,
				opts
			);
		}
		else
		{
			run_example_loop(
				display,
				win,
				ctx,
				example,
				example_thread_common_data,
				clock,
				opts
			);
		}
		example_thread_common_data.done = true;
		// cancel the example threads
		for(unsigned t=0; t!=params.num_threads; ++t)
		{
			if(thread_data[t].example_thread)
				thread_data[t].example_thread->Cancel();
		}
		// join the example threads
		for(unsigned t=0; t!=params.num_threads; ++t)
		{
			threads[t].join();
		}

		for(unsigned t=0; t!=params.num_threads; ++t)
		{
			if(!thread_data[t].error_message.empty())
				throw std::runtime_error(
					thread_data[t].error_message
				);
		}
	}
	catch(...)
	{
		example_thread_common_data.failure = true;
		master_ready.Signal(params.num_threads);
		try {for(auto& thread : threads) thread.join(); }
		catch(...){ }
		throw;
	}
	ctx.Release(display);
}

} // namespace oglplus

int glx_example_main(int argc, char ** argv)
{
	oglplus::ExampleOptions opts;


	opts.screenshot_path = nullptr;
	opts.framedump_prefix = nullptr;
	opts.width = 800;
	opts.height = 600;
	opts.samples = 0;

	int a=1;
	while(a<argc)
	{
		short parsed = 0;

		if((std::strcmp(argv[a], "--screenshot")) == 0 && (a+1<argc))
		{
			opts.screenshot_path = argv[a+1];
			parsed = 2;
		}
		else if((std::strcmp(argv[a], "--frame-dump")) == 0 && (a+1<argc))
		{
			opts.framedump_prefix = argv[a+1];
			parsed = 2;
		}
		else if((std::strcmp(argv[a], "--width")) == 0 && (a+1<argc))
		{
			opts.width = GLuint(std::atoi(argv[a+1]));
			parsed = 2;
		}
		else if((std::strcmp(argv[a], "--height")) == 0 && (a+1<argc))
		{
			opts.height = GLuint(std::atoi(argv[a+1]));
			parsed = 2;
		}
		else if((std::strcmp(argv[a], "--samples")) == 0 && (a+1<argc))
		{
			opts.samples = GLint(std::atoi(argv[a+1]));
			parsed = 2;
		}

		if(parsed == 2)
		{
			for(int b=a+1; b<argc; ++b)
			{
				argv[b-2] = argv[b];
			}
			argc -= 2;
		}
		else if(parsed == 0)
		{
			++a;
		}
	}

	// run the main loop
	oglplus::run_example(oglplus::x11::Display(), opts, argc, argv);
	return 0;
}

int main (int argc, char ** argv)
{
	return oglplus::example_main(glx_example_main, argc, argv);
}

