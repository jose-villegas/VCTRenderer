// glm pre-processors
#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
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
#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <array>
#include <math.h>
// glm math library headers
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/gtx/common.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/color_space.hpp>