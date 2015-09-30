//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_command.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_command.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_command

enum class PathNVCommand : GLubyte
{
	Close                          = GL_CLOSE_PATH_NV,
	MoveTo                         = GL_MOVE_TO_NV,
	RelativeMoveTo                 = GL_RELATIVE_MOVE_TO_NV,
	LineTo                         = GL_LINE_TO_NV,
	RelativeLineTo                 = GL_RELATIVE_LINE_TO_NV,
	HorizontalLineTo               = GL_HORIZONTAL_LINE_TO_NV,
	RelativeHorizontalLineTo       = GL_RELATIVE_HORIZONTAL_LINE_TO_NV,
	VerticalLineTo                 = GL_VERTICAL_LINE_TO_NV,
	RelativeVerticalLineTo         = GL_RELATIVE_VERTICAL_LINE_TO_NV,
	QuadraticCurveTo               = GL_QUADRATIC_CURVE_TO_NV,
	RelativeQuadraticCurveTo       = GL_RELATIVE_QUADRATIC_CURVE_TO_NV,
	CubicCurveTo                   = GL_CUBIC_CURVE_TO_NV,
	RelativeCubicCurveTo           = GL_RELATIVE_CUBIC_CURVE_TO_NV,
	SmoothQuadraticCurveTo         = GL_SMOOTH_QUADRATIC_CURVE_TO_NV,
	RelativeSmoothQuadraticCurveTo = GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV,
	SmoothCubicCurveTo             = GL_SMOOTH_CUBIC_CURVE_TO_NV,
	RelativeSmoothCubicCurveTo     = GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV,
	SmallCCWArcTo                  = GL_SMALL_CCW_ARC_TO_NV,
	RelativeSmallCCWArcTo          = GL_RELATIVE_SMALL_CCW_ARC_TO_NV,
	SmallCWArcTo                   = GL_SMALL_CW_ARC_TO_NV,
	RelativeSmallCWArcTo           = GL_RELATIVE_SMALL_CW_ARC_TO_NV,
	LargeCCWArcTo                  = GL_LARGE_CCW_ARC_TO_NV,
	RelativeLargeCCWArcTo          = GL_RELATIVE_LARGE_CCW_ARC_TO_NV,
	LargeCWArcTo                   = GL_LARGE_CW_ARC_TO_NV,
	RelativeLargeCWArcTo           = GL_RELATIVE_LARGE_CW_ARC_TO_NV,
	Restart                        = GL_RESTART_PATH_NV,
	DupFirstCubicCurveTo           = GL_DUP_FIRST_CUBIC_CURVE_TO_NV,
	DupLastCubicCurveTo            = GL_DUP_LAST_CUBIC_CURVE_TO_NV,
	Rect                           = GL_RECT_NV,
	CircularCCWArcTo               = GL_CIRCULAR_CCW_ARC_TO_NV,
	CircularCWArcTo                = GL_CIRCULAR_CW_ARC_TO_NV,
	CircularTangentArcTo           = GL_CIRCULAR_TANGENT_ARC_TO_NV,
	ArcTo                          = GL_ARC_TO_NV,
	RelativeArcTo                  = GL_RELATIVE_ARC_TO_NV
};

template <>
__Range<PathNVCommand> __EnumValueRange<PathNVCommand>(void);

__StrCRef __EnumValueName(PathNVCommand);

//]
