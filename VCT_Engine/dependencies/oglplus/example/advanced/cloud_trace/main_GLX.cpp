/**
 *  @example advanced/cloud_trace/main_GLX.cpp
 *  @brief GLX-based implementation of the main function.
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "main_common.hpp"
#include "app_data.hpp"
#include "resources.hpp"
#include "renderer.hpp"
#include "raytracer.hpp"
#include "copier.hpp"
#include "saver.hpp"
#include "threads.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <oglplus/glx/context.hpp>
#include <oglplus/glx/fb_configs.hpp>
#include <oglplus/glx/version.hpp>
#include <oglplus/glx/pixmap.hpp>
#include <oglplus/glx/pbuffer.hpp>
#include <oglplus/x11/window.hpp>
#include <oglplus/x11/color_map.hpp>
#include <oglplus/x11/visual_info.hpp>
#include <oglplus/x11/display.hpp>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

namespace oglplus {
namespace cloud_trace {

class CommonData
{
public:
	x11::Display& display;
	glx::Context& context;

	RaytracerData& rt_data;
	RaytracerTarget& rt_target;
private:

	const unsigned max_tiles;
	unsigned tile, face;
	bool keep_running;

	void SkipFaces(const AppData& app_data)
	{
		while((face < 6) && (app_data.skip_face[face]))
		{
			if(app_data.verbosity > 2)
			{
				app_data.logstr()
					<< "Skipping cube face "
					<< face
					<< std::endl;
			}
			++face;
		}
	}

	std::vector<std::exception_ptr> thread_errors;

	std::mutex mutex;
public:
	Semaphore master_ready;
	Semaphore thread_ready;

	CommonData(
		const AppData& app_data,
		x11::Display& disp,
		glx::Context& ctx,
		RaytracerData& rtd,
		RaytracerTarget& rtt
	): display(disp)
	 , context(ctx)
	 , rt_data(rtd)
	 , rt_target(rtt)
	 , max_tiles(app_data.tiles())
	 , tile(0)
	 , face(0)
	 , keep_running(true)
	{
		SkipFaces(app_data);
	}

	std::unique_lock<std::mutex> Lock(void)
	{
		return std::unique_lock<std::mutex>(mutex);
	}

	void PushError(std::exception_ptr&& err)
	{
		std::lock_guard<std::mutex> lock(mutex);
		thread_errors.push_back(std::move(err));
		keep_running = false;
	}

	void RethrowErrors(void)
	{
		for(auto& ep: thread_errors)
		{
			if(ep != nullptr)
			{
				std::rethrow_exception(ep);
			}
		}
	}

	void Stop(void)
	{
		std::lock_guard<std::mutex> lock(mutex);
		keep_running = false;
	}

	bool Done(void)
	{
		std::lock_guard<std::mutex> lock(mutex);
		return !keep_running;
	}

	unsigned Face(void)
	{
		return face;
	}

	bool FaceDone(void)
	{
		std::lock_guard<std::mutex> lock(mutex);

		return (tile >= max_tiles);
	}

	void NextFace(const AppData& app_data)
	{
		std::lock_guard<std::mutex> lock(mutex);

		if(face < 5)
		{
			++face;
			SkipFaces(app_data);

			if(face < 6) tile = 0;
			else keep_running = false;
		}
		else keep_running = false;
	}

	bool NextFaceTile(unsigned& rt_tile)
	{
		std::lock_guard<std::mutex> lock(mutex);

		rt_tile = tile;

		if(keep_running)
		{
			if(tile < max_tiles)
			{
				++tile;
				return true;
			}
		}
		return false;
	}

	void LogProgress(const AppData& app_data)
	{
		unsigned prom = ((1000*tile)/app_data.tiles());
		app_data.logstr()
			<< "Rendering face tile "
			<< tile
			<< " out of "
			<< app_data.tiles()
			<< " ("
			<< (prom / 10)
			<< "."
			<< (prom % 10)
			<< " %)"
			<< std::endl;
	}
};

void thread_loop(AppData& app_data, CommonData& common, x11::Display& display, glx::Context& context)
{
	Context gl;
	ResourceAllocator alloc;
	RaytraceCopier::Params rtc_params(
		display,
		context,
		common.context
	);
	RaytraceCopier rt_copier(app_data, common.rt_target);
	RaytracerResources rt_res(app_data, common.rt_data, alloc);
	Raytracer raytracer(app_data, rt_res);
	raytracer.Use(app_data);

	std::vector<unsigned> backlog;
	backlog.reserve(app_data.cols());
	std::chrono::milliseconds bl_interval(100);

	while(!common.Done())
	{
		// all threads must wait until
		// the raytrace target is cleared
		common.master_ready.Wait();

		if(common.Done()) break;

		raytracer.InitFrame(app_data, common.Face());
		raytracer.BeginWork(app_data);

		auto bl_begin = std::chrono::steady_clock::now();
		unsigned tile = 0;
		while(common.NextFaceTile(tile))
		{
			raytracer.Raytrace(app_data, tile);
			backlog.push_back(tile);

			auto now = std::chrono::steady_clock::now();

			if(bl_begin + bl_interval < now)
			{
				gl.Finish();
				auto lock = common.Lock();
				for(unsigned bl_tile : backlog)
				{
					rt_copier.Copy(
						app_data,
						rtc_params,
						raytracer,
						bl_tile
					);
				}
				lock.unlock();
				backlog.clear();
				bl_begin = now;
			}
			else gl.Finish();
		}
		auto lock = common.Lock();
		for(unsigned bl_tile : backlog)
		{
			rt_copier.Copy(
				app_data,
				rtc_params,
				raytracer,
				bl_tile
			);
		}
		lock.unlock();
		backlog.clear();
		gl.Finish();

		// signal to the master that the raytracing
		// of the current face has finished
		common.thread_ready.Signal();

		if(common.Done()) break;
		raytracer.EndWork(app_data);

		// wait for the master to save the face image
		common.master_ready.Wait();
	}
}

void pbuffer_loop(
	const glx::Pbuffer& pbuffer,
	AppData& app_data,
	CommonData& common,
	RaytracerTarget& rt_target,
	std::size_t n_threads
)
{
	Context gl;
	Renderer renderer(app_data, rt_target.tex_unit);
	renderer.Use(app_data);
	Saver saver(app_data);

	while(!common.Done())
	{
		if(app_data.verbosity > 0)
		{
			app_data.logstr()
				<< "Rendering cube face "
				<< common.Face()
				<< std::endl;
		}

		// clear the raytrace target
		rt_target.Clear(app_data);

		renderer.InitFrame(app_data, common.Face());
		renderer.Render(app_data);
		gl.Finish();

		// signal all threads that they can start raytracing tiles
		common.master_ready.Signal(n_threads);

		if(common.Done()) break;

		if(app_data.verbosity > 1)
		{
			// setup logging period
			std::chrono::system_clock::duration log_period;
			if(app_data.verbosity > 4)
			{
				log_period = std::chrono::seconds(1);
			}
			else if(app_data.verbosity > 3)
			{
				log_period = std::chrono::seconds(5);
			}
			else if(app_data.verbosity > 2)
			{
				log_period = std::chrono::seconds(10);
			}
			else if(app_data.verbosity > 1)
			{
				log_period = std::chrono::seconds(30);
			}

			auto log_time = std::chrono::system_clock::now();

			while(!common.thread_ready.Signalled())
			{
				auto now = std::chrono::system_clock::now();
				if(log_time + log_period < now)
				{
					common.LogProgress(app_data);
					log_time = now;
				}
				std::chrono::milliseconds period(50);
				std::this_thread::sleep_for(period);
			}
		}

		// wait for all raytracer threads to finish
		common.thread_ready.Wait(n_threads);

		for(unsigned b=0; b!=2; ++b)
		{
			renderer.Render(app_data);
			gl.Finish();
			common.context.SwapBuffers(pbuffer);
		}

		// save the face image
		saver.SaveFrame(app_data, rt_target, common.Face());
		// switch the face
		common.NextFace(app_data);

		// signal that the master is ready to render
		// the next face
		common.master_ready.Signal(n_threads);
	}
}

void window_loop(
	const x11::Window& window,
	AppData& app_data,
	CommonData& common,
	RaytracerTarget& rt_target,
	std::size_t n_threads
)
{
	Context gl;
	Renderer renderer(app_data, rt_target.tex_unit);
	renderer.Use(app_data);
	Saver saver(app_data);

	window.SelectInput(StructureNotifyMask| PointerMotionMask| KeyPressMask);
	::XEvent event;

	while(!common.Done())
	{
		if(app_data.verbosity > 0)
		{
			app_data.logstr()
				<< "Rendering cube face "
				<< common.Face()
				<< std::endl;
		}

		// clear the raytrace target
		rt_target.Clear(app_data);
		renderer.InitFrame(app_data, common.Face());
		renderer.Render(app_data);
		common.context.SwapBuffers(window);

		// setup logging period
		std::chrono::system_clock::duration log_period;
		if(app_data.verbosity > 4)
		{
			log_period = std::chrono::seconds(1);
		}
		else if(app_data.verbosity > 3)
		{
			log_period = std::chrono::seconds(5);
		}
		else if(app_data.verbosity > 2)
		{
			log_period = std::chrono::seconds(15);
		}
		else if(app_data.verbosity > 1)
		{
			log_period = std::chrono::minutes(1);
		}

		auto log_time = std::chrono::system_clock::now();

		// signal all threads that they can start raytracing tiles
		common.master_ready.Signal(n_threads);

		while(!common.FaceDone())
		{
			unsigned slot = 0;
			while(common.display.NextEvent(event))
			{
				switch(event.type)
				{
					case ClientMessage:
					case DestroyNotify:
						common.Stop();
						break;
					case ConfigureNotify:
						app_data.render_width = event.xconfigure.width;
						app_data.render_height = event.xconfigure.height;
						break;
					case MotionNotify:
						break;
					case KeyPress:
						if(::XLookupKeysym(&event.xkey, 0) == XK_Escape)
							common.Stop();
						break;
					default:;
				}
			}

			if(common.Done()) break;

			if((slot++ % 5) == 0)
			{
				auto lock = common.Lock();
				renderer.Render(app_data);
				gl.Finish();
				lock.unlock();

				common.context.SwapBuffers(window);
			}

			if(app_data.verbosity > 1)
			{
				auto now = std::chrono::system_clock::now();
				if(log_time + log_period < now)
				{
					common.LogProgress(app_data);
					log_time = now;
				}
			}
			std::chrono::milliseconds period(5);
			std::this_thread::sleep_for(period);
		}

		if(common.Done()) break;

		// wait for all raytracer threads to finish
		common.thread_ready.Wait(n_threads);

		renderer.Render(app_data);
		common.context.SwapBuffers(window);

		if(app_data.verbosity > 1)
		{
			app_data.logstr()
				<< "Finished cube face "
				<< common.Face()
				<< std::endl;
		}

		// save the face image
		saver.SaveFrame(app_data, rt_target, common.Face());
		// switch the face
		common.NextFace(app_data);

		// signal that the master is ready to render
		// the next face
		common.master_ready.Signal(n_threads);
	}
}

void main_thread(
	AppData& app_data,
	CommonData& common,
	const std::string& screen_name
)
{
#ifdef CLOUD_TRACE_USE_NV_copy_image
	x11::Display display(screen_name.empty()?nullptr:screen_name.c_str());
#else
	x11::Display& display = common.display;
	(void)screen_name;
#endif

	static int visual_attribs[] =
	{
		GLX_DRAWABLE_TYPE   , GLX_PIXMAP_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		None
	};

	glx::FBConfig fbc = glx::FBConfigs(
		display,
		visual_attribs
	).FindBest(display);

	x11::VisualInfo vi(display, fbc);

	x11::Pixmap xpm(display, vi, 8, 8);
	glx::Pixmap gpm(display, vi, xpm);

#ifdef CLOUD_TRACE_USE_NV_copy_image
	glx::Context context(display, fbc, 3, 3);
#else
	glx::Context context(display, fbc, common.context, 3, 3);
#endif

	context.MakeCurrent(gpm);

	common.thread_ready.Signal();

	common.master_ready.Wait();

	if(!common.Done())
	{
		try { thread_loop(app_data, common, display, context); }
		catch(...)
		{
			common.PushError(std::current_exception());
		}
	}

	context.Release(display);
}

template <typename Drawable, typename DrawLoop>
void call_drawable_loop(
	AppData& app_data,
	x11::Display& display,
	glx::Context& context,
	Drawable& drawable,
	DrawLoop drawable_loop
)
{
	RaytracerData rt_data(app_data);

	ResourceAllocator res_alloc;
	RaytracerTarget rt_target(app_data, res_alloc);

	CommonData common(
		app_data,
		display,
		context,
		rt_data,
		rt_target
	);

	std::vector<std::thread> threads;

	try
	{
		if(app_data.raytracer_params.empty())
		{
			app_data.raytracer_params.push_back(std::string());
		}

		for(auto& param : app_data.raytracer_params)
		{
			if(app_data.verbosity > 2)
			{
				app_data.logstr()
					<< "Spawning raytracing thread on ";
				if(param.empty())
				{
					app_data.logstr()
						<< "the current X screen";
				}
				else
				{
					app_data.logstr()
						<< "X screen "
						<< param;
				}
				app_data.logstr() << std::endl;
			}
			threads.push_back(
				std::thread(
					main_thread,
					std::ref(app_data),
					std::ref(common),
					std::cref(param)
				)
			);
		}
	}
	catch (...)
	{
		for(auto& t: threads) t.join();
		throw;
	}

	try
	{
		common.thread_ready.Wait(threads.size());
		common.master_ready.Signal(threads.size());

		drawable_loop(
			drawable,
			app_data,
			common,
			rt_target,
			threads.size()
		);
		for(auto& t: threads) t.join();
	}
	catch (...)
	{
		common.Stop();
		common.master_ready.Signal(threads.size());
		for(auto& t: threads) t.join();
		throw;
	}

	common.RethrowErrors();
}

int main_GLX(AppData& app_data)
{
	::XInitThreads();
	x11::Display display;
	glx::Version version(display);
	version.AssertAtLeast(1, 3);

	static const int visual_attribs[] =
	{
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT|GLX_PBUFFER_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DOUBLEBUFFER    , True,
		None
	};

	glx::FBConfig fbc = glx::FBConfigs(
		display,
		visual_attribs
	).FindBest(display);

	x11::VisualInfo vi(display, fbc);
	glx::Context context(display, fbc, 3, 3);

	if(app_data.render_offscreen)
	{
		const int pbuffer_attribs [] = {
			GLX_PBUFFER_WIDTH  , int(app_data.render_width),
			GLX_PBUFFER_HEIGHT , int(app_data.render_height),
			None
		};

		glx::Pbuffer pbuffer(display, fbc, pbuffer_attribs);

		context.MakeCurrent(pbuffer);

		GLAPIInitializer api_init;

		call_drawable_loop(
			app_data,
			display,
			context,
			pbuffer,
			pbuffer_loop
		);
	}
	else
	{
		x11::Window window(
			display,
			vi,
			x11::Colormap(display, vi),
			"CloudTrace",
			app_data.render_width,
			app_data.render_height
		);

		context.MakeCurrent(window);

		GLAPIInitializer api_init;

		call_drawable_loop(
			app_data,
			display,
			context,
			window,
			window_loop
		);
	}

	context.Release(display);

	return 0;
}

} // namespace cloud_trace
} // namespace oglplus

int main (int argc, char ** argv)
{
	using oglplus::cloud_trace::main_GLX;
	using oglplus::cloud_trace::AppData;

	AppData app_data;
	app_data.allow_x_rt_screens = true;
	app_data.allow_offscreen = true;
	if(app_data.ParseArgs(argc, argv))
	{
		app_data.LogInfo();
		return do_run_main(main_GLX, app_data);
	}
	else return 0;
}

