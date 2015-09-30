/**
 *  @file advanced/cloud_trace/app_data.hpp
 *  @brief Declares data structures and functions used application-wide
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_APP_DATA_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_APP_DATA_1119071146_HPP

#include <iosfwd>
#include <string>
#include <array>
#include <vector>

namespace oglplus {
namespace cloud_trace {

struct AppData
{
	// user interface functions
	void* puser_intf;
	void (*pset_status)(void*, const char*);

	void set_status(const char*);

	// error output stream
	std::ostream* perrstr;
	std::ostream& errstr(void) const;

	// logging output stream
	std::ostream* plogstr;
	std::ostream& logstr(void) const;

	// if the --offscreen parameter is supported
	bool allow_offscreen;
	// if the --X-rt-screen parameter is supported
	bool allow_x_rt_screens;

	// if the rendering should be done offscreen
	bool render_offscreen;
	// if the raytrace tiles should be clipped (using polygon clipping)
	bool clip_tiles;
	// if the raytrace  data should be saved instead of render output
	bool save_raytrace_data;

	// the verbosity level
	unsigned verbosity;

	// the random engine seed
	unsigned rand_seed;

	// output file prefix
	std::string output_prefix;
	// the face id strings
	std::array<std::string, 6> output_face_id;
	// the file suffix
	std::string output_suffix;

	// the face mask indicating which faces should be skipped
	std::array<bool, 6> skip_face;

	// parameters for raytracer threads (if any)
	std::vector<std::string> raytracer_params;

	// the finish shader that should be used
	std::string finish_shader;

	// width, height tile size, cells
	unsigned raytrace_width, raytrace_height;
	unsigned render_width, render_height;
	unsigned tile;
	unsigned rows(void) const;
	unsigned cols(void) const;
	unsigned tiles(void) const;

	// cloud parameters
	float unit_opacity, unit_attenuation;
	unsigned cloud_count, cloud_res;
	bool dump_cloud_image;
	std::string cloud_image_path;
	std::string cloud_data_path;

	// generator parameters
	float planet_radius, atm_thickness, covered_angle;
	float cloud_mean_alt, cloud_alt_disp;
	float cloud_mean_size, cloud_size_disp;

	// camera near/far values
	float cam_near, cam_far;

	// lighting parameters
	float light_x, light_y, light_z;
	float high_light, ambi_light;

	// crep. ray samples
	float crep_ray_far;
	unsigned crep_ray_sam;

	AppData(void);
	bool ParseArgs(int argc, char** argv);
	void LogInfo(void) const;
};

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
