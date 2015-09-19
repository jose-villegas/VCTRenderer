#pragma once

class DeferredHandler
{
    private:
        // deferred shader programs
        oglplus::Program geometryPass;
        oglplus::Program lightPass;
    public:
        DeferredHandler();
        virtual ~DeferredHandler();

        void LoadDeferredShaders();

        void UseGeometryPass() const { geometryPass.Use(); }
        void UseLightPass() const { lightPass.Use(); }
};

