#pragma once

#include <memory>

#include "../3rdparty/imgui/imgui.h"

class RenderWindow;

/// <summary>
/// Main class to implement custom interfaces
/// setups IMGUI to render in the current context
/// </summary>
class InterfaceRenderer
{
    public:
        InterfaceRenderer();
        virtual ~InterfaceRenderer();

        /// <summary>
        /// Setups the interface to render in the current
        /// rendering context
        /// </summary>
        /// <param name="activeWindow">The rendering window.</param>
        /// <param name="instantCallbacks">
        /// if set to <c>true</c> [instant callbacks].
        /// </param>
        static void Initialize(const RenderWindow &activeWindow,
                               bool instantCallbacks = true);
        /// <summary>
        /// Deletes all rendering objects and UI data
        /// </summary>
        static void Terminate();
        /// <summary>
        /// Renders the UI
        /// </summary>
        static void Render();
        /// <summary>
        /// Sets up the GUI for a new frame.
        /// </summary>
        static void NewFrame();
    protected:
        struct RendererData
        {
            GLFWwindow * window;
            double time;
            bool mousePressed[3];
            float mouseWheel;
            GLuint fontTexture;
            int shaderHandle;
            int vertHandle;
            int fragHandle;
            int attribLocationTex;
            int attribLocationProjMtx;
            int attribLocationPosition;
            int attribLocationUV;
            int attribLocationColor;
            unsigned int vboHandle;
            unsigned int vaoHandle;
            unsigned int elementsHandle;
            bool disabled;

            RendererData()
            {
                window = nullptr;
                time = 0.0f;
                mousePressed[0] = mousePressed[1] = mousePressed[2] = false;
                mouseWheel = 0.0f;
                fontTexture = 0;
                shaderHandle = 0, vertHandle = 0, fragHandle = 0;
                attribLocationTex = 0, attribLocationProjMtx = 0;
                attribLocationPosition = 0, attribLocationUV = 0,
                attribLocationColor = 0;
                vboHandle = 0, vaoHandle = 0, elementsHandle = 0;
                disabled = false;
            }
        };

        static std::unique_ptr<RendererData> renderer;
        static void InvalidateDeviceObjects();
        static void CreateFontsTexture();
        static void CreateDeviceObjects();

        static void RenderDrawList(ImDrawData * drawData);
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

