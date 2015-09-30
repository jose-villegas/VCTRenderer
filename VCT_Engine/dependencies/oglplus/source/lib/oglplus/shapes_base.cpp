/**
 *  .file lib/oglplus/shapes_base.cpp
 *  .brief Shape generator functions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "prologue.ipp"
#include <oglplus/string/def.hpp>
#include <oglplus/string/empty.hpp>
#include <oglplus/string/utf8.hpp>
#include <oglplus/error/limit.hpp>
#include <oglplus/error/glfunc.hpp>
#include <oglplus/face_mode.hpp>
#include <oglplus/data_type.hpp>
#include <oglplus/primitive_type.hpp>
#include <oglplus/shapes/draw.hpp>

#include "implement.ipp"

#include <oglplus/shapes/cage.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/grid.hpp>
#include <oglplus/shapes/icosahedron.hpp>
#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/torus.hpp>
#include <oglplus/shapes/sphere.hpp>
#include <oglplus/shapes/subdiv_sphere.hpp>
#include <oglplus/shapes/spiral_sphere.hpp>
#include <oglplus/shapes/tetrahedrons.hpp>
#include <oglplus/shapes/twisted_torus.hpp>
#include <oglplus/shapes/wicker_torus.hpp>
#include "epilogue.ipp"
