/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_RasterizationOps
namespace client {

using oglplus::__context_RasterizationOps;

} // namespace client
//]
//[oglplus_client_RasterizationState
namespace client {

class RasterizationState
{
public:
	__SettingStack<__FaceOrientation, ...> FrontFace; /*<
	Stack managing the setting of the front face orientation.
	>*/

	__SettingStack<__Face, ...> CullFace; /*<
	Stack managing the setting of cull face.
	>*/

	__SettingStack<__PolygonMode, ...> PolygonMode; /*<
	Stack managing the setting of polygon rasterization mode.
	>*/

	__SettingStack<__context_PolygonOffsPara, ...> PolygonOffset; /*<
	Stack managing the setting of polygon offset parameters.
	>*/

#if GL_VERSION_3_2 || GL_ARB_provoking_vertex
	__SettingStack<__ProvokeMode, ...> ProvokingVertex; /*<
	Stack managing the setting of provoking vertex mode.
	>*/
#endif

	__SettingStack<GLfloat, ...> LineWidth; /*<
	Stack managing the setting of line width value.
	>*/
	__SettingStack<GLfloat, ...> PointSize; /*<
	Stack managing the setting of point size value.
	>*/
	__SettingStack<GLfloat, ...> PointFadeThresholdSize; /*<
	Stack managing the setting of point fade threshold size value.
	>*/

#if GL_VERSION_4_0
	__SettingStack<GLfloat, ...> MinSampleShading; /*<
	Stack managing the setting of the minimal sample shading value.
	>*/
#endif

#if GL_NV_fragment_coverage_to_color
	__SettingStack<__FragDataSlot, ...> FragmentCoverageColor; /*<
	Stack managing the setting of the draw buffer used for
	fragment coverage to color.
	>*/

#endif
};

} // namespace client
//]

