//  File doc/quickbook/oglplus/quickref/enums/blend_equation_advanced_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/blend_equation_advanced.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_blend_equation_advanced_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__BlendEquationAdvanced> class Transform>
class __EnumToClass<Base, __BlendEquationAdvanced, Transform> /*<
Specialization of __EnumToClass for the __BlendEquationAdvanced enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<BlendEquationAdvanced::Multiply>
		Multiply;
	Transform<BlendEquationAdvanced::Screen>
		Screen;
	Transform<BlendEquationAdvanced::Overlay>
		Overlay;
	Transform<BlendEquationAdvanced::Darken>
		Darken;
	Transform<BlendEquationAdvanced::Lighten>
		Lighten;
	Transform<BlendEquationAdvanced::Colordodge>
		Colordodge;
	Transform<BlendEquationAdvanced::Colorburn>
		Colorburn;
	Transform<BlendEquationAdvanced::Hardlight>
		Hardlight;
	Transform<BlendEquationAdvanced::Softlight>
		Softlight;
	Transform<BlendEquationAdvanced::Difference>
		Difference;
	Transform<BlendEquationAdvanced::Exclusion>
		Exclusion;
	Transform<BlendEquationAdvanced::HSLHue>
		HSLHue;
	Transform<BlendEquationAdvanced::HSLSaturation>
		HSLSaturation;
	Transform<BlendEquationAdvanced::HSLColor>
		HSLColor;
	Transform<BlendEquationAdvanced::HSLLuminosity>
		HSLLuminosity;
};

} // namespace enums
#endif
//]

