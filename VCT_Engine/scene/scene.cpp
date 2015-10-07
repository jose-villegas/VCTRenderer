#include "stdafx.h"
#include "scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
    // early owner-ship release for static scope (no gl context)
    OGLTexture2D::GetDefaultTexture().release();
    OGLTexture2D::GetDefaultNormalTexture().release();
    OGLTexture2D::GetErrorTexture().release();
}
