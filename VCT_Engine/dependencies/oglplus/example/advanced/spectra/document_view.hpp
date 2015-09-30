/*
 *  .file advanced/spectra/document_view.hpp
 *  .brief Declares the document view class.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_VIEW_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_VIEW_HPP

#include <oglplus/gl.hpp>
#include <oglplus/math/matrix.hpp>
#include <oglplus/math/vector.hpp>
#include <oglplus/math/angle.hpp>

struct SpectraDocumentView
{
private:
	GLfloat max_time;
	GLuint width;
	GLuint height;
	GLfloat camera_distance;
	GLfloat target_x, target_y, target_z;
	GLfloat stretch_x, stretch_y, stretch_z;
	oglplus::Anglef camera_azimuth;
	oglplus::Anglef camera_elevation;
	oglplus::Anglef camera_xfov;

	oglplus::Mat4f stretch_matrix;
	oglplus::CamMatrixf camera_matrix;
	oglplus::Mat4f projection_matrix;
	oglplus::Mat4f transf_matrix;
	oglplus::Mat4f inv_view_matrix;

	void Clamp(GLfloat& value, GLfloat min, GLfloat max, GLfloat s = 1.0f);

	void RecalcProjection(void);
	void RecalcCamera(void);
	void RecalcStretch(void);
	void RecalcTransf(void);
public:
	SpectraDocumentView(void);

	void SetMaxTime(GLfloat max_t);

	GLfloat MaxTargetX(void);

	GLuint SegmentCount(void);

	GLuint Width(void) const { return width; }
	GLuint Height(void) const { return height; }

	void Reshape(GLuint w, GLuint h);

	oglplus::Vec3f ScreenToWorld(GLint x, GLint y);
	oglplus::Vec3f PickOnPlane(unsigned vn, GLint x, GLint y);
	oglplus::Vec3f PickOnGround(GLint x, GLint y);
	oglplus::Vec3f PickOnWall(GLint x, GLint y);
	oglplus::Vec3f PickOnSphere(GLint x, GLint y);

	void Slide(GLint new_x, GLint new_y, GLint old_x, GLint old_y);
	void Elevate(GLint new_x, GLint new_y, GLint old_x, GLint old_y);
	void Orbit(GLint new_x, GLint new_y, GLint old_x, GLint old_y);
	void Scale(GLint new_x, GLint new_y, GLint old_x, GLint old_y);
	void Zoom(GLfloat delta);
	void StretchDomain(GLint new_x, GLint new_y, GLint old_x, GLint old_y);
	void StretchRange(GLint new_x, GLint new_y, GLint old_x, GLint old_y);

	float TimeStretch(void) const;

	const oglplus::Mat4f& ProjectionMatrix(void) const;

	const oglplus::Mat4f& CameraMatrix(void) const;

	const oglplus::Mat4f& StretchMatrix(void) const;

	const oglplus::Mat4f& TransfMatrix(void) const;
};

#endif // include guard
