# Copyright 2013-2014 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

LOCAL_PATH:= $(call my-dir)
OGLPLUS_ROOT := $(LOCAL_PATH)/../../../..

include $(CLEAR_VARS)

LOCAL_MODULE := libtriangle

LOCAL_CXXFLAGS := \
	-std=c++0x \
	-frtti \
	-fexceptions \
	-I$(OGLPLUS_ROOT)/third_party/include \
	-I$(OGLPLUS_ROOT)/include \
	-I$(OGLPLUS_ROOT)/implement

LOCAL_SRC_FILES := triangle.cpp

LOCAL_LDLIBS := -lGLESv3 -lEGL

include $(BUILD_SHARED_LIBRARY)
