//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_metric_query_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_metric_query.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_metric_query_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PathNVMetricQuery> class Transform>
class __EnumToClass<Base, __PathNVMetricQuery, Transform> /*<
Specialization of __EnumToClass for the __PathNVMetricQuery enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PathNVMetricQuery::GlyphWidth>
		GlyphWidth;
	Transform<PathNVMetricQuery::GlyphHeight>
		GlyphHeight;
	Transform<PathNVMetricQuery::GlyphHorizontalBearingX>
		GlyphHorizontalBearingX;
	Transform<PathNVMetricQuery::GlyphHorizontalBearingY>
		GlyphHorizontalBearingY;
	Transform<PathNVMetricQuery::GlyphHorizontalBearingAdvance>
		GlyphHorizontalBearingAdvance;
	Transform<PathNVMetricQuery::GlyphVerticalBearingX>
		GlyphVerticalBearingX;
	Transform<PathNVMetricQuery::GlyphVerticalBearingY>
		GlyphVerticalBearingY;
	Transform<PathNVMetricQuery::GlyphVerticalBearingAdvance>
		GlyphVerticalBearingAdvance;
	Transform<PathNVMetricQuery::GlyphHasKerning>
		GlyphHasKerning;
	Transform<PathNVMetricQuery::FontXMinBounds>
		FontXMinBounds;
	Transform<PathNVMetricQuery::FontYMinBounds>
		FontYMinBounds;
	Transform<PathNVMetricQuery::FontXMaxBounds>
		FontXMaxBounds;
	Transform<PathNVMetricQuery::FontYMaxBounds>
		FontYMaxBounds;
	Transform<PathNVMetricQuery::FontUnitsPerEm>
		FontUnitsPerEm;
	Transform<PathNVMetricQuery::FontAscender>
		FontAscender;
	Transform<PathNVMetricQuery::FontDescender>
		FontDescender;
	Transform<PathNVMetricQuery::FontHeight>
		FontHeight;
	Transform<PathNVMetricQuery::FontMaxAdvanceWidth>
		FontMaxAdvanceWidth;
	Transform<PathNVMetricQuery::FontMaxAdvanceHeight>
		FontMaxAdvanceHeight;
	Transform<PathNVMetricQuery::FontUnderlinePosition>
		FontUnderlinePosition;
	Transform<PathNVMetricQuery::FontUnderlineThickness>
		FontUnderlineThickness;
	Transform<PathNVMetricQuery::FontHasKerning>
		FontHasKerning;
};

} // namespace enums
#endif
//]

