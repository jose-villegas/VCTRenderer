#pragma once

class DeferredHandler
{
    private:
        // deferred shader programs
        oglplus::Program geometryPass;
        oglplus::VertexShader vsGeometryPass;
        oglplus::FragmentShader fsGeometryPass;

        oglplus::Program lightPass;
        oglplus::VertexShader vLightPass;
        oglplus::FragmentShader fsLightPass;
    public:
        DeferredHandler();
        virtual ~DeferredHandler();

        void UseGeometryPass() const { geometryPass.Use(); }
        void UseLightPass() const { lightPass.Use(); }

        oglplus::Program &GetGeometryPass() { return geometryPass; }
        oglplus::Program &GetLightPass() { return lightPass; }
};

