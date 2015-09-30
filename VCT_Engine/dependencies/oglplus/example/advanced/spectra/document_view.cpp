/*
 *  .file advanced/spectra/document_view.cpp
 *  .brief Implements the document view class.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "document_view.hpp"

#include <cmath>

const GLfloat min_target_x = -1.5f;
const GLfloat min_target_y =  0.0f;
const GLfloat max_target_y =  1.0f;
const GLfloat min_target_z = -0.5f;
const GLfloat max_target_z =  1.0f;

const GLfloat min_camera_distance = 2.0f;
const GLfloat max_camera_distance = 50.0f;

const GLfloat min_stretch_x = 0.5f;
const GLfloat max_stretch_x = 100.0f;
const GLfloat min_stretch_y = 0.5f;
const GLfloat max_stretch_y = 20.0f;
const GLfloat min_stretch_z = 0.5f;
const GLfloat max_stretch_z = 20.0f;

void SpectraDocumentView::Clamp(GLfloat& value, GLfloat min, GLfloat max, GLfloat s)
{
	if(value < min*s) value = min*s;
	if(value > max*s) value = max*s;
}

SpectraDocumentView::SpectraDocumentView(void)
 : max_time(1.0f)
 , width(64)
 , height(64)
 , camera_distance(4.0f)
 , target_x(0.5f)
 , target_y(0.5f)
 , target_z(0.5f)
 , stretch_x(10.f)
 , stretch_y(1.0f)
 , stretch_z(4.0f)
 , camera_azimuth(oglplus::Degrees(215))
 , camera_elevation(oglplus::Degrees(45))
 , camera_xfov(oglplus::Degrees(70))
{
	RecalcProjection();
	RecalcCamera();
	RecalcStretch();
}

GLfloat SpectraDocumentView::MaxTargetX(void)
{
	return max_time+1.5f;
}

GLuint SpectraDocumentView::SegmentCount(void)
{
	assert(max_time > 0.0f);
	GLuint result = GLuint(max_time);
	if(max_time - result > 0.0f)
		++result;
	return result;
}

void SpectraDocumentView::SetMaxTime(GLfloat max_t)
{
	assert(max_t > 0.0);
	if(max_t < 1.0f) max_time = 1.0f;
	else max_time = max_t;
}

void SpectraDocumentView::Reshape(GLuint w, GLuint h)
{
	width = w;
	height = h;
	RecalcProjection();
}

oglplus::Vec3f SpectraDocumentView::ScreenToWorld(GLint x, GLint y)
{
	GLfloat u = -1.0f+GLfloat(2*x)/GLfloat(width);
	GLfloat v = +1.0f-GLfloat(2*y)/GLfloat(height);
	GLfloat w = 1.0f;

	oglplus::Vec4f ndc(u, v, w, 1.0f);

	oglplus::Vec4f wsc = inv_view_matrix * ndc;

	return wsc.xyz() / wsc.w();
}

oglplus::Vec3f SpectraDocumentView::PickOnPlane(unsigned vn, GLint x, GLint y)
{
	oglplus::Vec3f cam = camera_matrix.Position();
	oglplus::Vec3f ray = Normalized(ScreenToWorld(x, y) - cam);

	if(ray[vn] != 0)
	{
		GLfloat t = -cam[vn]/ray[vn];
		return cam + ray * t;
	}
	return ray * camera_distance;
}

oglplus::Vec3f SpectraDocumentView::PickOnGround(GLint x, GLint y)
{
	return PickOnPlane(1, x, y);
}

oglplus::Vec3f SpectraDocumentView::PickOnWall(GLint x, GLint y)
{
	return PickOnPlane(2, x, y);
}

oglplus::Vec3f SpectraDocumentView::PickOnSphere(GLint x, GLint y)
{
	oglplus::Vec3f ori(target_x, target_y, target_z);
	oglplus::Vec3f cam = camera_matrix.Position();
	oglplus::Vec3f ray = Normalized(ScreenToWorld(x, y) - cam);
	oglplus::Vec3f ofs = (cam-ori);
	GLfloat rad = camera_distance*Tan(camera_xfov*0.5f);

	GLfloat a = 1.0f; //Dot(ray, ray);
	GLfloat b = 2*Dot(ofs, ray);
	GLfloat c = Dot(ofs, ofs)-rad*rad;
	GLfloat d = b*b-4*a*c;

	if(d >= 0.0f)
	{
		GLfloat q = 0.5f/a;

		GLfloat t0 = (-b-std::sqrt(d))*q;
		GLfloat t1 = (-b+std::sqrt(d))*q;
		GLfloat t = t0<t1?t0:t1;

		return cam + ray*t;
	}
	return oglplus::Vec3f();
}

void SpectraDocumentView::Slide(GLint new_x, GLint new_y, GLint old_x, GLint old_y)
{
	oglplus::Vec3f dir = PickOnGround(new_x, new_y)-PickOnGround(old_x, old_y);
	target_x -= dir.x();
	Clamp(target_x, min_target_x, MaxTargetX(), stretch_x);
	target_z -= dir.z();
	Clamp(target_z, min_target_z, max_target_z, stretch_z);
	RecalcCamera();
}

void SpectraDocumentView::Elevate(GLint new_x, GLint new_y, GLint old_x, GLint old_y)
{
	oglplus::Vec3f dir = PickOnWall(new_x, new_y)-PickOnGround(old_x, old_y);
	target_x -= dir.x();
	Clamp(target_x, min_target_x, MaxTargetX(), stretch_x);
	target_y -= dir.y();
	Clamp(target_y, min_target_y, max_target_y, stretch_y);
	RecalcCamera();
}

void SpectraDocumentView::Orbit(GLint new_x, GLint new_y, GLint old_x, GLint old_y)
{
	oglplus::Anglef fc = oglplus::FullCircle();
	oglplus::Anglef ra = oglplus::RightAngle()-oglplus::Degrees(1);

	oglplus::Vec3f ns = Normalized(ScreenToWorld(new_x, new_y));
	oglplus::Vec3f os = Normalized(ScreenToWorld(old_x, old_y));

	if(Length(ns) > 0.0f && Length(os) > 0.0f)
	{
		camera_azimuth += oglplus::Radians(
			std::atan2(-ns.z(), ns.x())-
			std::atan2(-os.z(), os.x())
		);
		while(camera_azimuth > fc)
			camera_azimuth -= fc;
		while(camera_azimuth < -fc)
			camera_azimuth += fc;

		camera_elevation -= (ns.y()-os.y())*ra;

		if(camera_elevation > ra)
			camera_elevation = ra;
		if(camera_elevation <-ra)
			camera_elevation =-ra;
	}

	RecalcCamera();
}

void SpectraDocumentView::Scale(GLint new_x, GLint new_y, GLint old_x, GLint old_y)
{
	oglplus::Vec3f t(target_x, target_y, target_z);
	oglplus::Vec3f ng = PickOnGround(new_x, new_y);
	oglplus::Vec3f og = PickOnGround(old_x, old_y);

	GLfloat lng = Distance(ng, t);
	GLfloat log = Distance(og, t);

	if(lng > 0)
	{
		camera_distance *= log / lng;
		Clamp(camera_distance, min_camera_distance, max_camera_distance);
		RecalcCamera();
	}
}

void SpectraDocumentView::Zoom(GLfloat delta)
{
	camera_distance *= std::exp(-delta*0.02f);
	Clamp(camera_distance, min_camera_distance, max_camera_distance);
	RecalcCamera();
}

void SpectraDocumentView::StretchDomain(GLint new_x, GLint new_y, GLint old_x, GLint old_y)
{
	oglplus::Vec3f ng = PickOnGround(new_x, new_y);
	oglplus::Vec3f og = PickOnGround(old_x, old_y);

	if(ng.x() != 0)
	{
		stretch_x *= 1.0f+(ng.x()-og.x()) / ng.x();
		Clamp(stretch_x, min_stretch_x, max_stretch_x);
	}
	if(ng.z() != 0)
	{
		stretch_z *= 1.0f+(ng.z()-og.z()) / ng.z();
		Clamp(stretch_z, min_stretch_z, max_stretch_z);
	}

	RecalcStretch();
}

void SpectraDocumentView::StretchRange(GLint new_x, GLint new_y, GLint old_x, GLint old_y)
{
	oglplus::Vec3f ng = PickOnWall(new_x, new_y);
	oglplus::Vec3f og = PickOnWall(old_x, old_y);

	if(ng.x() != 0)
	{
		stretch_x *= 1.0f+(ng.x()-og.x()) / ng.x();
		Clamp(stretch_x, min_stretch_x, max_stretch_x);
	}
	if(ng.y() != 0)
	{
		stretch_y *= 1.0f+(ng.y()-og.y()) / ng.y();
		Clamp(stretch_y, min_stretch_y, max_stretch_y);
	}

	RecalcStretch();
}

void SpectraDocumentView::RecalcProjection(void)
{
	projection_matrix = oglplus::CamMatrixf::PerspectiveX(
		camera_xfov,
		float(width)/height,
		0.1f,
		10000.0f
	);
	RecalcTransf();
}

void SpectraDocumentView::RecalcCamera(void)
{
	camera_matrix = oglplus::CamMatrixf::Orbiting(
		oglplus::Vec3f(target_x, target_y, target_z),
		camera_distance,
		camera_azimuth,
		camera_elevation
	);
	RecalcTransf();
}

void SpectraDocumentView::RecalcStretch(void)
{
	stretch_matrix = oglplus::ModelMatrixf::Scale(
		stretch_x,
		stretch_y,
		stretch_z
	);
	RecalcTransf();
}

void SpectraDocumentView::RecalcTransf(void)
{
	transf_matrix = projection_matrix * camera_matrix * stretch_matrix;
	inv_view_matrix = Inverse(projection_matrix * camera_matrix);
}

float SpectraDocumentView::TimeStretch(void) const
{
	return stretch_x;
}

const oglplus::Mat4f& SpectraDocumentView::ProjectionMatrix(void) const
{
	return projection_matrix;
}

const oglplus::Mat4f& SpectraDocumentView::CameraMatrix(void) const
{
	return camera_matrix;
}

const oglplus::Mat4f& SpectraDocumentView::StretchMatrix(void) const
{
	return stretch_matrix;
}

const oglplus::Mat4f& SpectraDocumentView::TransfMatrix(void) const
{
	return transf_matrix;
}

