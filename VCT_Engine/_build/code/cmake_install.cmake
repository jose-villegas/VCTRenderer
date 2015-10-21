# Install script for directory: F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_install64")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/Debug/assimpd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/Release/assimp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/MinSizeRel/assimp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/RelWithDebInfo/assimp.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.1.1")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/Debug/assimpd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/Release/assimp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/MinSizeRel/assimp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/RelWithDebInfo/assimp.dll")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/anim.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/ai_assert.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/camera.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/color4.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/color4.inl"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/config.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/defs.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/cfileio.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/light.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/material.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/material.inl"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/matrix3x3.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/matrix3x3.inl"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/matrix4x4.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/matrix4x4.inl"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/mesh.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/postprocess.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/quaternion.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/quaternion.inl"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/scene.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/metadata.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/texture.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/types.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/vector2.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/vector2.inl"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/vector3.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/vector3.inl"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/version.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/cimport.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/importerdesc.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/Importer.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/DefaultLogger.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/ProgressHandler.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/IOStream.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/IOSystem.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/Logger.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/LogStream.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/NullLogger.hpp"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/cexport.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/Exporter.hpp"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/Compiler/poppack1.h"
    "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/dependencies/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/Debug/assimpd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/code/RelWithDebInfo/assimp.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

