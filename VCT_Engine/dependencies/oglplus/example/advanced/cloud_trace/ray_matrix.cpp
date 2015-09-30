/**
 *  @file advanced/cloud_trace/ray_matrix.cpp
 *  @brief Implements ray projection matrix constructor
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "ray_matrix.hpp"

#include <oglplus/assert.hpp>

namespace oglplus {
namespace cloud_trace {

Mat4f RayMatrix(const AppData& app_data, unsigned face)
{
	Vec3f cx, cy, cz;

	switch(face)
	{
		case 0: // +X
		{
			cx = +Vec3f::Unit(2);
			cy = +Vec3f::Unit(1);
			cz = -Vec3f::Unit(0);
			break;
		}
		case 1: // -X
		{
			cx = -Vec3f::Unit(2);
			cy = +Vec3f::Unit(1);
			cz = +Vec3f::Unit(0);
			break;
		}
		case 2: // +Y
		{
			cx = -Vec3f::Unit(0);
			cy = +Vec3f::Unit(2);
			cz = +Vec3f::Unit(1);
			break;
		}
		case 3: // -Y
		{
			cx = -Vec3f::Unit(0);
			cy = -Vec3f::Unit(2);
			cz = -Vec3f::Unit(1);
			break;
		}
		case 4: // +Z
		{
			cx = -Vec3f::Unit(0);
			cy = +Vec3f::Unit(1);
			cz = -Vec3f::Unit(2);
			break;
		}
		case 5: // -Z
		{
			cx = +Vec3f::Unit(0);
			cy = +Vec3f::Unit(1);
			cz = +Vec3f::Unit(2);
			break;
		}
		default: OGLPLUS_ABORT("Invalid cube face");
	}

	return
		CamMatrixf::PerspectiveX(
			RightAngle(), 1,
			app_data.cam_near,
			app_data.cam_far
		)* Mat4f(
			Vec4f(cx, 0),
			Vec4f(cy, 0),
			Vec4f(cz, 0),
			Vec4f::Unit(3)
		);
}

} // namespace cloud_trace
} // namespace oglplus

