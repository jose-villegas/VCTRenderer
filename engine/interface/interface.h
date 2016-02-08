#pragma once

#include "../3rdparty/imgui/imgui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

class RenderWindow;

/// <summary>
/// Main class to implement custom interfaces
/// setups IMGUI to render in the current context
/// </summary>
class Interface
{
    public:
        Interface();
        virtual ~Interface();

        /// <summary>
        /// Setups the interface to render in the current
        /// rendering context
        /// </summary>
        /// <param name="activeWindow">The rendering window.</param>
        /// <param name="instantCallbacks">if set to <c>true</c> [instant callbacks].</param>
        void Initialize(const RenderWindow &activeWindow,
                        bool instantCallbacks = true);
        /// <summary>
        /// Deletes all rendering objects and UI data
        /// </summary>
        void Terminate();
        /// <summary>
        /// Renders the UI
        /// </summary>
        void Render() const;
        /// <summary>
        /// Sets up the GUI for a new frame.
        /// </summary>
        void NewFrame();
    protected:
        // application specific UI code
        virtual void Draw() = 0;
    private:
        struct UIData
        {
            GLFWwindow * window = nullptr;
            double       time = 0.0f;
            bool         mousePressed[3] = { false, false, false };
            float        mouseWheel = 0.0f;
            GLuint       fontTexture = 0;
            int          shaderHandle = 0, vertHandle = 0, fragHandle = 0;
            int          attribLocationTex = 0, attribLocationProjMtx = 0;
            int          attribLocationPosition = 0, attribLocationUV = 0,
                         attribLocationColor = 0;
            unsigned int vboHandle = 0, vaoHandle = 0, elementsHandle = 0;
        };

        static std::unique_ptr<UIData> uiData;

        void InvalidateDeviceObjects();
        void CreateFontsTexture();
        void CreateDeviceObjects();

        static void RenderDrawList(ImDrawData * drawData);
        static void MousePosCallback(GLFWwindow * window, double xpos, double ypos);
        static void MouseButtonCallback(GLFWwindow * window, int button,
                                        int action, int mods);
        static void ScrollCallback(GLFWwindow * window, double xoffset,
                                   double yoffset);
        static void KeyCallback(GLFWwindow * window, int key, int scancode,
                                int action, int mods);
        static void CharCallback(GLFWwindow * window, unsigned int c);
        static void SetClipboardText(const char * text);
        static const char * GetClipboardText();
};