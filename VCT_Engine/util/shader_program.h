#pragma once
class ShaderInfo
{
    public:
        enum ShaderClass
        {
            Vertex,
            TessControl,
            TessEvaluation,
            Geometry,
            Fragment,
            Compute
        };
    public:
        ShaderInfo() {};
        virtual ~ShaderInfo() {};

        void CleanSource();
        void SourceFromFile(ShaderClass shaderType, const std::string &sFilepath);
        void SourceFromString(ShaderClass shaderType, const std::string &sourceString);
    protected:
        ShaderClass shaderType;
        std::string filePath;
        std::string shaderSource;
};

class OGLShader : public ShaderInfo
{
    private:
        std::unique_ptr<oglplus::Shader> shaderHandler;
    public:
        void Compile();
        OGLShader() {};
        virtual ~OGLShader() {};
};

class OGLShaderProgram
{
    private:
        std::vector<OGLShader *> attachedShaders;
        oglplus::Program shaderProgram;
    public:

        OGLShaderProgram() {};
        virtual ~OGLShaderProgram() {};
};

