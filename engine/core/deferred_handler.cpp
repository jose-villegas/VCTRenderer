#include "stdafx.h"
#include "deferred_handler.h"
#include "scene\light.h"

DeferredHandler::DeferredHandler(unsigned int windowWith,
                                 unsigned int windowHeight)
{
    LoadShaders();
    SetupGBuffer(windowWith, windowHeight);
    CreateFullscreenQuad();
}


void DeferredHandler::CreateFullscreenQuad()
{
    using namespace oglplus;
    // bind vao for full screen quad
    fullscreenQuadVertexArray.Bind();
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
    fullscreenQuadVertexBuffer.Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, fsQuadVertexBufferData);
    // set up attrib points
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, 5 * sizeof(float), (const GLvoid *)0);
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(2, DataType::Float, false, 5 * sizeof(float), (const GLvoid *)12);
    // data for element buffer array
    static const std::array<unsigned int, 6> indexData =
    {
        0, 1, 2, // first triangle
        2, 1, 3, // second triangle
    };
    // bind and fill element array
    fullscreenQuadElementBuffer.Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, indexData);
    // unbind vao
    NoVertexArray().Bind();
}

/// <summary>
/// Renders a full screen quad, useful for the light pass stage
/// </summary>
void DeferredHandler::RenderFullscreenQuad()
{
    fullscreenQuadVertexArray.Bind();
    gl.DrawElements(
        oglplus::PrimitiveType::Triangles, 6,
        oglplus::DataType::UnsignedInt
    );
}

LightingProgram::LightingProgram()
{
    samplers.Resize(static_cast<size_t>(GBufferTextureId::TEXTURE_TYPE_MAX));
}

void LightingProgram::ExtractUniform(oglplus::SLDataType uType,
                                     std::string uName)
{
    using namespace oglplus;

    // extract light uniform data

    if (uType == SLDataType::FloatVec3)
    {
        Uniform<glm::vec3> * vec3ToSet =
            uName == "viewPosition" ? &viewPosUniform :
            uName == "directionalLight.ambient" ? &directionalLight.ambient :
            uName == "directionalLight.diffuse" ? &directionalLight.diffuse :
            uName == "directionalLight.specular" ? &directionalLight.specular :
            uName == "directionalLight.position" ? &directionalLight.position :
            uName == "directionalLight.direction" ? &directionalLight.direction : nullptr;

        if (vec3ToSet != nullptr)
        {
            *vec3ToSet = Uniform<glm::vec3>(program, uName);
        }
    }

    if (uType == SLDataType::Float)
    {
        Uniform<float> * vec3ToSet =
            uName == "directionalLight.angleInnerCone" ? &directionalLight.angleInnerCone :
            uName == "directionalLight.angleOuterCone" ? &directionalLight.angleOuterCone :
            uName == "directionalLight.attenuation.constant" ?
            &directionalLight.attenuation.constant :
            uName == "directionalLight.attenuation.linear" ?
            &directionalLight.attenuation.linear :
            uName == "directionalLight.attenuation.quadratic" ?
            &directionalLight.attenuation.quadratic : nullptr;

        if (vec3ToSet != nullptr)
        {
            *vec3ToSet = Uniform<float>(program, uName);
        }
    }

    if (uType == SLDataType::UnsignedInt)
    {
        Uniform<unsigned int> * vec3ToSet =
            uName == "directionalLight.lightType" ? &directionalLight.lightType : nullptr;

        if (vec3ToSet != nullptr)
        {
            *vec3ToSet = Uniform<unsigned int>(program, uName);
        }
    }

    if (uType == SLDataType::Sampler2D)
    {
        int addGSamplerType = -1;
        // gbuffer samplers to read
        addGSamplerType =
            uName == "gPosition" ? Position :
            uName == "gNormal" ? Normal :
            uName == "gAlbedo" ? Albedo :
            uName == "gSpecular" ? Specular : -1;

        // g buffer active samplers
        if (addGSamplerType != -1)
        {
            samplers
            .Save((GBufferTextureId)addGSamplerType,
                  UniformSampler(program, uName));
        }
    }
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
    lightingProgram.vertexShader.Source(vsLightPassSource.c_str());
    lightingProgram.vertexShader.Compile();
    // fragment shader
    std::ifstream fsLightPassFile("resources\\shaders\\light_pass.frag");
    std::string fsLightPassSource(
        (std::istreambuf_iterator<char>(fsLightPassFile)),
        std::istreambuf_iterator<char>()
    );
    lightingProgram.fragmentShader.Source(fsLightPassSource.c_str());
    lightingProgram.fragmentShader.Compile();
    // create a new shader program and attach the shaders
    lightingProgram.program.AttachShader(lightingProgram.vertexShader);
    lightingProgram.program.AttachShader(lightingProgram.fragmentShader);
    // link attached shaders
    lightingProgram.program.Link().Use();
    // extract relevant uniforms
    lightingProgram.ExtractActiveUniforms();
    // close source files
    vsLightPassFile.close();
    fsLightPassFile.close();
    // geometry pass shader source code and compile
    std::ifstream vsGeomPassFile("resources\\shaders\\geometry_pass.vert");
    std::string vsGeomPassSource(
        (std::istreambuf_iterator<char>(vsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    geometryProgram.vertexShader.Source(vsGeomPassSource.c_str());
    geometryProgram.vertexShader.Compile();
    // fragment shader
    std::ifstream fsGeomPassFile("resources\\shaders\\geometry_pass.frag");
    std::string fsGeomPassSource(
        (std::istreambuf_iterator<char>(fsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    geometryProgram.fragmentShader.Source(fsGeomPassSource.c_str());
    geometryProgram.fragmentShader.Compile();
    // create a new shader program and attach the shaders
    geometryProgram.program.AttachShader(geometryProgram.vertexShader);
    geometryProgram.program.AttachShader(geometryProgram.fragmentShader);
    // link attached shaders
    geometryProgram.program.Link().Use();
    // extract relevant uniforms
    geometryProgram.ExtractActiveUniforms();
    // close source files
    vsGeomPassFile.close();
    fsGeomPassFile.close();
}

void DeferredHandler::SetupGBuffer(unsigned int windowWith,
                                   unsigned int windowHeight)
{
    using namespace oglplus;
    colorBuffers.clear();
    geometryBuffer.Bind(FramebufferTarget::Draw);
    // position color buffer
    gl.Bound(Texture::Target::_2D,
             bufferTextures[DeferredProgram::Position])
    .Image2D(
        0, PixelDataInternalFormat::RGB16F, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // high precision texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_0,
        bufferTextures[DeferredProgram::Position], 0
    );
    colorBuffers.push_back(FramebufferColorAttachment::_0);
    // normal color buffer
    gl.Bound(Texture::Target::_2D,
             bufferTextures[DeferredProgram::Normal])
    .Image2D(
        0, PixelDataInternalFormat::RGB16F, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // high precision texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_1,
        bufferTextures[DeferredProgram::Normal], 0
    );
    colorBuffers.push_back(FramebufferColorAttachment::_1);
    // albedo color buffer
    gl.Bound(Texture::Target::_2D,
             bufferTextures[DeferredProgram::Albedo])
    .Image2D(
        0, PixelDataInternalFormat::RGB8, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // 8 bit per component texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_2,
        bufferTextures[DeferredProgram::Albedo],
        0
    );
    colorBuffers.push_back(FramebufferColorAttachment::_2);
    // specular intensity and shininess buffer
    gl.Bound(Texture::Target::_2D,
             bufferTextures[DeferredProgram::Specular])
    .Image2D(
        0, PixelDataInternalFormat::R8, windowWith, windowHeight,
        0, PixelDataFormat::Red, PixelDataType::Float, nullptr
    ) // 8 bit per component texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferColorAttachment::_3,
        bufferTextures[DeferredProgram::Specular], 0
    );
    colorBuffers.push_back(FramebufferColorAttachment::_3);
    // depth buffer
    gl.Bound(Texture::Target::_2D, bufferTextures[DeferredProgram::Depth])
    .Image2D(
        0, PixelDataInternalFormat::DepthComponent32F, windowWith, windowHeight,
        0, PixelDataFormat::DepthComponent, PixelDataType::Float, nullptr
    ); // high precision float texture
    geometryBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferAttachment::Depth, bufferTextures[DeferredProgram::Depth], 0
    );
    gl.DrawBuffers(colorBuffers.size(), colorBuffers.data());

    if (!Framebuffer::IsComplete(Framebuffer::Target::Draw))
    {
        FramebufferStatus status = Framebuffer::Status(Framebuffer::Target::Draw);
        std::cout << "(DeferredHandler) Framebuffer Error:"
                  << (unsigned int)status;
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}

DeferredHandler::~DeferredHandler() {}

/// <summary>
/// Binds the geometry buffer for reading or drawing
/// </summary>
/// <param name="bindingMode">The binding mode.</param>
void DeferredHandler::BindGeometryBuffer(const oglplus::FramebufferTarget
        & bindingMode)
{
    gl.Bind(bindingMode, geometryBuffer);
}

/// <summary>
/// Reads from the specified color buffer attached to the geometry buffer.
/// </summary>
/// <param name="gBufferTexType">The texture target to read from.</param>
void DeferredHandler::ReadGeometryBuffer(const DeferredProgram::GBufferTextureId
        & gBufferTextureId)
{
    gl.ReadBuffer(colorBuffers[static_cast<unsigned int>(gBufferTextureId)]);
}

/// <summary>
/// Activates texture slots for the geometry buffer
/// texture targets and binds the textures
/// </summary>
void DeferredHandler::ActivateBindTextureTargets()
{
    // bind and active position gbuffer texture
    oglplus::Texture::Active(DeferredProgram::Position);
    bufferTextures[DeferredProgram::Position]
    .Bind(oglplus::TextureTarget::_2D);
    // bind and active normal gbuffer texture
    oglplus::Texture::Active(DeferredProgram::Normal);
    bufferTextures[DeferredProgram::Normal]
    .Bind(oglplus::TextureTarget::_2D);
    // bind and active albedo gbuffer texture
    oglplus::Texture::Active(DeferredProgram::Albedo);
    bufferTextures[DeferredProgram::Albedo]
    .Bind(oglplus::TextureTarget::_2D);
    // bind and active specular gbuffer texture
    oglplus::Texture::Active(DeferredProgram::Specular);
    bufferTextures[DeferredProgram::Specular]
    .Bind(oglplus::TextureTarget::_2D);
}

/// <summary>
/// Calls ExtractUniform virtual method per every active uniform
/// in the shader program
/// </summary>
void DeferredProgram::ExtractActiveUniforms()
{
    auto uRange = program.ActiveUniforms();

    for (unsigned int i = 0; i < uRange.Size(); i++)
    {
        auto uName = uRange.At(i).Name();
        auto uType = uRange.At(i).Type();
        // call virtual implemented virtual method
        ExtractUniform(uType, uName);
    }
}

GeometryProgram::GeometryProgram()
{
    samplers.Resize(RawTexture::TYPE_MAX);
    transformMatrices.Resize(TransformMatrices::MATRIX_ID_MAX);
    materialFloat3.Resize(OGLMaterial::FLOAT3_PROPERTY_ID_MAX);
    materialFloat1.Resize(OGLMaterial::FLOAT1_PROPERTY_ID_MAX);
    materialUInt1.Resize(OGLMaterial::UINT1_PROPERTY_ID_MAX);
}

void GeometryProgram::ExtractUniform(oglplus::SLDataType uType,
                                     std::string uName)
{
    using namespace oglplus;

    if (uType == SLDataType::Sampler2D)
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

        // geometry pass active samplers
        if (addSamplerType != -1)
        {
            samplers
            .Save(static_cast<RawTexture::TextureType>(addSamplerType),
                  UniformSampler(program, uName));
        }
    }
    else if (uType == SLDataType::FloatMat4)
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

        if (addMat4Type != -1)
        {
            transformMatrices
            .Save(static_cast<TransformMatrices::MatrixId>(addMat4Type),
                  Uniform<glm::mat4x4>(program, uName));
        }
    }
    else if (uType == SLDataType::FloatVec3)
    {
        int addF3MatType = -1;
        addF3MatType =
            uName == "material.ambient" ? OGLMaterial::Ambient :
            uName == "material.diffuse" ? OGLMaterial::Diffuse :
            uName == "material.specular" ? OGLMaterial::Specular :
            uName == "material.emissive" ? OGLMaterial::Emissive :
            uName == "material.transparent" ? OGLMaterial::Transparent : -1;

        if (addF3MatType != -1)
        {
            materialFloat3.
            Save(static_cast<OGLMaterial::Float3PropertyId>(addF3MatType),
                 Uniform<glm::vec3>(program, uName));
        }
    }
    else if (uType == SLDataType::Float)
    {
        int addF1MatType = -1;
        addF1MatType =
            uName == "material.opacity" ? OGLMaterial::Opacity :
            uName == "material.shininess" ? OGLMaterial::Shininess :
            uName == "material.shininessStrenght" ? OGLMaterial::ShininessStrenght :
            uName == "material.refractionIndex" ? OGLMaterial::RefractionIndex : -1;

        if (addF1MatType != -1)
        {
            materialFloat1.
            Save(static_cast<OGLMaterial::Float1PropertyId>(addF1MatType),
                 Uniform<float>(program, uName));
        }
    }
    else if (uType == SLDataType::UnsignedInt)
    {
        int addUint1MatType = -1;
        addUint1MatType =
            uName == "material.useNormalsMap" ? OGLMaterial::NormalMapping : -1;

        if (addUint1MatType != -1)
        {
            materialUInt1.
            Save(static_cast<OGLMaterial::UInt1PropertyId>(addUint1MatType),
                 Uniform<unsigned int>(program, uName));
        }
    }
}

void GeometryProgram::SetUniform(TransformMatrices::MatrixId mId,
                                 const glm::mat4x4 &val)
{
    if (transformMatrices.Has(mId))
    {
        transformMatrices[mId].Set(val);
    }
}

void GeometryProgram::SetUniform(RawTexture::TextureType tId, const int val)
{
    if (samplers.Has(tId))
    {
        samplers[tId].Set(val);
    }
}

void GeometryProgram::SetUniform(OGLMaterial::Float3PropertyId mF3Id,
                                 const glm::vec3 &val)
{
    if (materialFloat3.Has(mF3Id))
    {
        materialFloat3[mF3Id].Set(val);
    }
}

void GeometryProgram::SetUniform(OGLMaterial::Float1PropertyId mF1Id,
                                 const float val)
{
    if (materialFloat1.Has(mF1Id))
    {
        materialFloat1[mF1Id].Set(val);
    }
}

void GeometryProgram::SetUniform(OGLMaterial::UInt1PropertyId mF1Id,
                                 const unsigned int val)
{
    if (materialUInt1.Has(mF1Id))
    {
        materialUInt1[mF1Id].Set(val);
    }
}

const std::vector<RawTexture::TextureType> &GeometryProgram::ActiveSamplers()
const
{
    return samplers.Actives();
}

const std::vector<TransformMatrices::MatrixId>
&GeometryProgram::ActiveTransformMatrices() const
{
    return transformMatrices.Actives();
}

const std::vector<OGLMaterial::Float3PropertyId>
&GeometryProgram::ActiveMaterialFloat3Properties() const
{
    return materialFloat3.Actives();
}

const std::vector<OGLMaterial::Float1PropertyId>
&GeometryProgram::ActiveMaterialFloat1Properties() const
{
    return materialFloat1.Actives();
}

const std::vector<OGLMaterial::UInt1PropertyId>
&GeometryProgram::ActiveMaterialUInt1Properties() const
{
    return materialUInt1.Actives();
}

void LightingProgram::SetUniform(GBufferTextureId tId, const int val)
{
    if (samplers.Has(tId))
    {
        samplers[tId].Set(val);
    }
}

void LightingProgram::SetUniform(const glm::vec3 &val, bool viewPosition)
{
    if (viewPosition == true)
    {
        viewPosUniform.Set(val);
    }
}
