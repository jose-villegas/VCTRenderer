#include "stdafx.h"
#include "scene.h"


Scene::Scene()
{
	// scenes have by default at least one camera 
	// and a light, they can be deleted
	this->cameras.push_back(Camera());
	this->lights.push_back(Light());
}


Scene::~Scene()
{
    // early owner-ship release for static scope (no gl context)
    OGLTexture2D::GetDefaultTexture().release();
    OGLTexture2D::GetDefaultNormalTexture().release();
    OGLTexture2D::GetErrorTexture().release();
}
