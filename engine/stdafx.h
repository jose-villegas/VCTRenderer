#define NOMINMAX
// glm pre-processors
#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
// debug mode controller
#ifdef NDEBUG
    #define OGLPLUS_LOW_PROFILE 1
#endif
// opengl and context creation headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// ogl headers c++ wrapper on opengl
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/opt/smart_enums.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/buffer_usage.hpp>
#include <oglplus/glsl_source.hpp>
#include <oglplus/glsl_string.hpp>
#include <oglplus/object/array.hpp>
#include <oglplus/object/group.hpp>
#include <oglplus/interop/glm.hpp>
// standard and stl library headers
#include <algorithm>
#include <numeric>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
// glm math library headers
#include "glm/detail/type_mat.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/common.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/common.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/vec3.hpp>
// intel thread building blocks library for multi-threading
#include <tbb/tbb.h>
// immediate mode interface library
#include "3rdparty/imgui/imgui.h"

static oglplus::Context gl;