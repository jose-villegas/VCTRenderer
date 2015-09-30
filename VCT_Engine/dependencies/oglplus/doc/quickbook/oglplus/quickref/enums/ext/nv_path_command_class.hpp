//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_command_class.hpp
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
//[oglplus_enums_ext_nv_path_command_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PathNVCommand> class Transform>
class __EnumToClass<Base, __PathNVCommand, Transform> /*<
Specialization of __EnumToClass for the __PathNVCommand enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PathNVCommand::Close>
		Close;
	Transform<PathNVCommand::MoveTo>
		MoveTo;
	Transform<PathNVCommand::RelativeMoveTo>
		RelativeMoveTo;
	Transform<PathNVCommand::LineTo>
		LineTo;
	Transform<PathNVCommand::RelativeLineTo>
		RelativeLineTo;
	Transform<PathNVCommand::HorizontalLineTo>
		HorizontalLineTo;
	Transform<PathNVCommand::RelativeHorizontalLineTo>
		RelativeHorizontalLineTo;
	Transform<PathNVCommand::VerticalLineTo>
		VerticalLineTo;
	Transform<PathNVCommand::RelativeVerticalLineTo>
		RelativeVerticalLineTo;
	Transform<PathNVCommand::QuadraticCurveTo>
		QuadraticCurveTo;
	Transform<PathNVCommand::RelativeQuadraticCurveTo>
		RelativeQuadraticCurveTo;
	Transform<PathNVCommand::CubicCurveTo>
		CubicCurveTo;
	Transform<PathNVCommand::RelativeCubicCurveTo>
		RelativeCubicCurveTo;
	Transform<PathNVCommand::SmoothQuadraticCurveTo>
		SmoothQuadraticCurveTo;
	Transform<PathNVCommand::RelativeSmoothQuadraticCurveTo>
		RelativeSmoothQuadraticCurveTo;
	Transform<PathNVCommand::SmoothCubicCurveTo>
		SmoothCubicCurveTo;
	Transform<PathNVCommand::RelativeSmoothCubicCurveTo>
		RelativeSmoothCubicCurveTo;
	Transform<PathNVCommand::SmallCCWArcTo>
		SmallCCWArcTo;
	Transform<PathNVCommand::RelativeSmallCCWArcTo>
		RelativeSmallCCWArcTo;
	Transform<PathNVCommand::SmallCWArcTo>
		SmallCWArcTo;
	Transform<PathNVCommand::RelativeSmallCWArcTo>
		RelativeSmallCWArcTo;
	Transform<PathNVCommand::LargeCCWArcTo>
		LargeCCWArcTo;
	Transform<PathNVCommand::RelativeLargeCCWArcTo>
		RelativeLargeCCWArcTo;
	Transform<PathNVCommand::LargeCWArcTo>
		LargeCWArcTo;
	Transform<PathNVCommand::RelativeLargeCWArcTo>
		RelativeLargeCWArcTo;
	Transform<PathNVCommand::Restart>
		Restart;
	Transform<PathNVCommand::DupFirstCubicCurveTo>
		DupFirstCubicCurveTo;
	Transform<PathNVCommand::DupLastCubicCurveTo>
		DupLastCubicCurveTo;
	Transform<PathNVCommand::Rect>
		Rect;
	Transform<PathNVCommand::CircularCCWArcTo>
		CircularCCWArcTo;
	Transform<PathNVCommand::CircularCWArcTo>
		CircularCWArcTo;
	Transform<PathNVCommand::CircularTangentArcTo>
		CircularTangentArcTo;
	Transform<PathNVCommand::ArcTo>
		ArcTo;
	Transform<PathNVCommand::RelativeArcTo>
		RelativeArcTo;
};

} // namespace enums
#endif
//]

