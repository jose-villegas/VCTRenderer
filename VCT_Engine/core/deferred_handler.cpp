#include "stdafx.h"
#include "deferred_handler.h"

DeferredHandler::DeferredHandler(unsigned int windowWith,
                                 unsigned int windowHeight)
{
    LoadShaders();
    InitializeGBuffer(windowWith, windowHeight);
    CreateFullscreenQuad();
}


void DeferredHandler::CreateFullscreenQuad()
{
    using namespace oglplus;
    // bind vao for full screen quad
    fsQuadVertexArray.Bind();
    // data for fs quad
    static const std::array<float, 20> fsQuadVertexBufferData =
    {
        // X    Y    Z     U     V
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // vertex 0
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // vertex 1
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // vertex 2
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // vertex 3
    };
    // bind vertex buffer and fill
    fsQuadVertexBuffer.Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, fsQuadVertexBufferData);
    // set up attrib points
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, 5 * sizeof(float), (const GLvoid*)0);
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(2, DataType::Float, false, 5 * sizeof(float), (const GLvoid*)12);
    // data for element buffer array
    static const std::array<unsigned int, 6> indexData =
    {
        0, 1, 2, // first triangle
        2, 1, 3, // second triangle
    };
    // bind and fill element array
    fsQuadElementBuffer.Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, indexData);
    // unbind vao
    NoVertexArray().Bind();
}

void DeferredHandler::RenderFSQuad()
{
    fsQuadVertexArray.Bind();
    gl.DrawElements(
        oglplus::PrimitiveType::Triangles, 6,
        oglplus::DataType::UnsignedInt
    );
}

void DeferredHandler::LoadShaders()
{
    using namespace oglplus;
    // light pass shader source code and compile
    std::ifstream vsLightPassFile("resources\\shaders\\light_pass.vert");
    std::string vsLightPassSource(
        (std::istreambuf_iterator<char>(vsLightPassFile)),
        std::istreambuf_iterator<char>()
    );
    lightPass.sVertex.Source(vsLightPassSource.c_str());
    lightPass.sVertex.Compile();
    // fragment shader
    std::ifstream fsLightPassFile("resources\\shaders\\light_pass.frag");
    std::string fsLightPassSource(
        (std::istreambuf_iterator<char>(fsLightPassFile)),
        std::istreambuf_iterator<char>()
    );
    lightPass.sFragment.Source(fsLightPassSource.c_str());
    lightPass.sFragment.Compile();
    // create a new shader program and attach the shaders
    lightPass.program.AttachShader(lightPass.sVertex);
    lightPass.program.AttachShader(lightPass.sFragment);
    // link attached shaders
    lightPass.program.Link().Use();
    // extract relevant uniforms
    lightPass.ExtractActiveUniforms();
    lightPass.viewPosLightpass = oglplus::Uniform<glm::vec3>
                                 (lightPass.program, "viewPosition");
    vsLightPassFile.close(); fsLightPassFile.close();
    // geometry pass shader source code and compile
    std::ifstream vsGeomPassFile("resources\\shaders\\geometry_pass.vert");
    std::string vsGeomPassSource(
        (std::istreambuf_iterator<char>(vsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    geometryPass.sVertex.Source(vsGeomPassSource.c_str());
    geometryPass.sVertex.Compile();
    // fragment shader
    std::ifstream fsGeomPassFile("resources\\shaders\\geometry_pass.frag");
    std::string fsGeomPassSource(
        (std::istreambuf_iterator<char>(fsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    geometryPass.sFragment.Source(fsGeomPassSource.c_str());
    geometryPass.sFragment.Compile();
    // create a new shader program and attach the shaders
    geometryPass.program.AttachShader(geometryPass.sVertex);
    geometryPass.program.AttachShader(geometryPass.sFragment);
    // link attached shaders
    geometryPass.program.Link().Use();
    // extract relevant uniforms
    geometryPass.ExtractActiveUniforms();
    vsGeomPassFile.close(); fsGeomPassFile.close();
}

void DeferredHandler::InitializeGBuffer(unsigned int windowWith,
                                        unsigned int windowHeight)
{
    using namespace oglplus;
    openedColorBuffers.clear();
    geomBuffer.Bind(FramebufferTarget::Draw);
    // position color buffer
    gl.Bound(Texture::Target::_2D,
             bTextures[(unsigned int)GBufferTextureId::Position])
    .Image2D(
        0, PixelDataInternalFormat::RGB16F, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // high precision texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geomBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_0,
        bTextures[(unsigned int)GBufferTextureId::Position], 0
    );
    openedColorBuffers.push_back(FramebufferColorAttachment::_0);
    // normal color buffer
    gl.Bound(Texture::Target::_2D,
             bTextures[(unsigned int)GBufferTextureId::Normal])
    .Image2D(
        0, PixelDataInternalFormat::RGB16F, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // high precision texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geomBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_1,
        bTextures[(unsigned int)GBufferTextureId::Normal],
        0
    );
    openedColorBuffers.push_back(FramebufferColorAttachment::_1);
    // albedo color buffer
    gl.Bound(Texture::Target::_2D,
             bTextures[(unsigned int)GBufferTextureId::Albedo])
    .Image2D(
        0, PixelDataInternalFormat::RGB8, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // 8 bit per component texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geomBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_2,
        bTextures[(unsigned int)GBufferTextureId::Albedo],
        0
    );
    openedColorBuffers.push_back(FramebufferColorAttachment::_2);
    // specular intensity and shininess buffer
    gl.Bound(Texture::Target::_2D,
             bTextures[(unsigned int)GBufferTextureId::Specular])
    .Image2D(
        0, PixelDataInternalFormat::R8, windowWith, windowHeight,
        0, PixelDataFormat::Red, PixelDataType::Float, nullptr
    ) // 8 bit per component texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geomBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_3,
        bTextures[(unsigned int)GBufferTextureId::Specular], 0
    );
    openedColorBuffers.push_back(FramebufferColorAttachment::_3);
    // depth buffer
    gl.Bound(Texture::Target::_2D, bDepthTexture)
    .Image2D(
        0, PixelDataInternalFormat::DepthComponent32F, windowWith, windowHeight,
        0, PixelDataFormat::DepthComponent, PixelDataType::Float, nullptr
    ); // high precision float texture
    geomBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferAttachment::Depth, bDepthTexture, 0
    );
    gl.DrawBuffers(openedColorBuffers.size(), openedColorBuffers.data());

    if(!Framebuffer::IsComplete(Framebuffer::Target::Draw))
    {
        FramebufferStatus status = Framebuffer::Status(Framebuffer::Target::Draw);
        std::cout << "(DeferredHandler) Framebuffer Error:"
                  << (unsigned int)status;
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}

DeferredHandler::~DeferredHandler()
{
}

void DeferredHandler::BindGBuffer(const oglplus::FramebufferTarget
                                  &bindingMode)
{
    gl.Bind(bindingMode, geomBuffer);
}

void DeferredHandler::ReadGBuffer(const GBufferTextureId &gBufferTexType)
{
    gl.ReadBuffer(openedColorBuffers[(unsigned int)gBufferTexType]);
}

void DeferredHandler::ActivateGBufferTextures()
{
    // bind and active position gbuffer texture
    oglplus::Texture::Active((unsigned int)GBufferTextureId::Position);
    bTextures[(unsigned int)GBufferTextureId::Position]
    .Bind(oglplus::TextureTarget::_2D);
    // bind and active normal gbuffer texture
    oglplus::Texture::Active((unsigned int)GBufferTextureId::Normal);
    bTextures[(unsigned int)GBufferTextureId::Normal]
    .Bind(oglplus::TextureTarget::_2D);
    // bind and active albedo gbuffer texture
    oglplus::Texture::Active((unsigned int)GBufferTextureId::Albedo);
    bTextures[(unsigned int)GBufferTextureId::Albedo]
    .Bind(oglplus::TextureTarget::_2D);
    // bind and active specular gbuffer texture
    oglplus::Texture::Active((unsigned int)GBufferTextureId::Specular);
    bTextures[(unsigned int)GBufferTextureId::Specular]
    .Bind(oglplus::TextureTarget::_2D);
}

void DeferredHandler::SetLightPassUniforms(const glm::vec3 &viewPosition)
{
    lightPass.viewPosLightpass.Set(viewPosition);
    lightPass.SetUniform(
        GBufferTextureId::Position,
        (unsigned int)GBufferTextureId::Position
    );
    lightPass.SetUniform(
        GBufferTextureId::Normal,
        (unsigned int)GBufferTextureId::Normal
    );
    lightPass.SetUniform(
        GBufferTextureId::Albedo,
        (unsigned int)GBufferTextureId::Albedo
    );
    lightPass.SetUniform(
        GBufferTextureId::Specular,
        (unsigned int)GBufferTextureId::Specular
    );
}

const oglplus::Texture & DeferredHandler::GetGBufferTexture(
    GBufferTextureId tID) const
{
    return bTextures[(unsigned int)tID];
}

void DeferredProgram::ExtractActiveUniforms()
{
    using namespace oglplus;
    Program::ActiveUniformRange uRange = program.ActiveUniforms();

    for(unsigned int i = 0; i < uRange.Size(); i++)
    {
        std::string uName = uRange.At(i).Name();
        SLDataType uType = uRange.At(i).Type();

        if(uType == SLDataType::Sampler2D)
        {
            int addSamplerType = -1;
            // texture samplers
            addSamplerType =
                uName == "noneMap" ? RawTexture::None :
                uName == "diffuseMap" ? RawTexture::Diffuse :
                uName == "specularMap" ? RawTexture::Specular :
                uName == "ambientMap" ? RawTexture::Ambient :
                uName == "emissiveMap" ? RawTexture::Emissive :
                uName == "heightMap" ? RawTexture::Height :
                uName == "normalsMap" ? RawTexture::Normals :
                uName == "shininessMap" ? RawTexture::Shininess :
                uName == "opacityMap" ? RawTexture::Opacity :
                uName == "displacementMap" ? RawTexture::Displacement :
                uName == "lightMap" ? RawTexture::Lightmap :
                uName == "reflectionMap" ? RawTexture::Reflection :
                uName == "unknowMap" ? RawTexture::Unknow : -1;
            // gbuffer samplers to read
            addSamplerType == -1 ? addSamplerType =
                uName == "gPosition" ? (int)GBufferTextureId::Position :
                uName == "gNormal" ? (int)GBufferTextureId::Normal :
                uName == "gAlbedo" ? (int)GBufferTextureId::Albedo :
                uName == "gSpecular" ? (int)GBufferTextureId::Specular : -1 : -1;

            if(addSamplerType != -1)
            {
                samplers[addSamplerType].reset(new UniformSampler(program, uName));
                aSamplers.push_back((RawTexture::TextureType)addSamplerType);
            }
        }
        else if(uType == SLDataType::FloatMat4)
        {
            int addMat4Type = -1;
            addMat4Type =
                uName == "matrices.modelView" ? TransformMatrices::ModelView :
                uName == "matrices.modelViewProjection" ?
                TransformMatrices::ModelViewProjection :
                uName == "matrices.model" ? TransformMatrices::Model :
                uName == "matrices.view" ? TransformMatrices::View :
                uName == "matrices.projection" ? TransformMatrices::Projection :
                uName == "matrices.normal" ? TransformMatrices::Normal : -1;

            if(addMat4Type != -1)
            {
                matrices[addMat4Type].reset(new Uniform<glm::mat4x4>(program, uName));
                aMatrices.push_back((TransformMatrices::TransformMatrixId)addMat4Type);
            }
        }
        else if(uType == SLDataType::FloatVec3)
        {
            int addF3MatType = -1;
            addF3MatType =
                uName == "material.ambient" ? Material::Ambient :
                uName == "material.diffuse" ? Material::Diffuse :
                uName == "material.specular" ? Material::Specular :
                uName == "material.emissive" ? Material::Emissive :
                uName == "material.transparent" ? Material::Transparent : -1;

            if(addF3MatType != -1)
            {
                materialFloat3[addF3MatType].reset(new Uniform<glm::vec3>(program, uName));
                aFloat3Material.push_back((Material::MaterialFloat3PropertyId)addF3MatType);
            }
        }
        else if(uType == SLDataType::Float)
        {
            int addF1MatType = -1;
            addF1MatType =
                uName == "material.opacity" ? Material::Opacity :
                uName == "material.shininess" ? Material::Shininess :
                uName == "material.shininessStrenght" ? Material::ShininessStrenght :
                uName == "material.refractionIndex" ? Material::RefractionIndex : -1;

            if(addF1MatType != -1)
            {
                materialFloat1[addF1MatType].reset(new Uniform<float>(program, uName));
                aFloat1Material.push_back((Material::MaterialFloat1PropertyId)addF1MatType);
            }
        }
        else if(uType == SLDataType::UnsignedInt)
        {
            int addUint1MatType = -1;
            addUint1MatType =
                uName == "material.useNormalsMap" ? Material::NormalMapping : -1;

            if(addUint1MatType != -1)
            {
                materialUInt1[addUint1MatType].reset(new Uniform<unsigned int>(program, uName));
                aUInt1Material.push_back((Material::MaterialUInt1PropertyId)addUint1MatType);
            }
        }
    }
}

void DeferredProgram::SetUniform(TransformMatrices::TransformMatrixId mId,
                                 const glm::mat4x4 &val) const
{
    if(matrices[mId])
    {
        matrices[mId]->Set(val);
    }
}

void DeferredProgram::SetUniform(RawTexture::TextureType tId,
                                 const int val) const
{
    if(samplers[tId])
    {
        samplers[tId]->Set(val);
    }
}

void DeferredProgram::SetUniform(Material::MaterialFloat3PropertyId mF3Id,
                                 const glm::vec3 &val) const
{
    if(materialFloat3[mF3Id])
    {
        materialFloat3[mF3Id]->Set(val);
    }
}

void DeferredProgram::SetUniform(Material::MaterialFloat1PropertyId mF1Id,
                                 const float val) const
{
    if(materialFloat1[mF1Id])
    {
        materialFloat1[mF1Id]->Set(val);
    }
}

void DeferredProgram::SetUniform(Material::MaterialUInt1PropertyId mF1Id,
                                 const unsigned int val) const
{
    if(materialUInt1[mF1Id])
    {
        materialUInt1[mF1Id]->Set(val);
    }
}

void DeferredProgram::SetUniform(GBufferTextureId tId, const int val) const
{
    if(samplers[(unsigned int)tId])
    {
        samplers[(unsigned int)tId]->Set(val);
    }
}
