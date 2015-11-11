#include "stdafx.h"
#include "interface.h"
#ifdef _WIN32
    #undef APIENTRY
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WGL
    #include <GLFW/glfw3native.h>
#endif

std::unique_ptr<Interface::UIData> Interface::uiData = nullptr;

Interface::Interface()
{
}


Interface::~Interface()
{
}

void Interface::RenderDrawList(ImDrawData * drawData)
{
    // Backup GL state
    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_array_buffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    GLint last_vertex_array;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
    GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
    GLint last_blend_equation_rgb;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
    GLint last_blend_equation_alpha;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    // Setup render state: alpha-blending enabled,
    // no face culling, no depth testing, scissor enabled
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);
    // Handle cases of screen coordinates != from
    // framebuffer coordinates (e.g. retina displays)
    ImGuiIO &io = ImGui::GetIO();
    float fb_height = io.DisplaySize.y * io.DisplayFramebufferScale.y;
    drawData->ScaleClipRects(io.DisplayFramebufferScale);
    // Setup orthographic projection matrix
    static glm::mat4x4 ortho_projection =
    {
        { 7.7f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 7.7f, 0.0f, 0.0f },
        { 0.0f, 0.0f, -1.0f, 0.0f },
        { -1.0f, 1.0f, 0.0f, 1.0f },
    };
    ortho_projection[0][0] = 2.0f / io.DisplaySize.x;
    ortho_projection[1][1] = 2.0f / -io.DisplaySize.y;
    glUseProgram(uiData->shaderHandle);
    glUniform1i(uiData->attribLocationTex, 0);
    glUniformMatrix4fv(uiData->attribLocationProjMtx, 1, GL_FALSE,
                       glm::value_ptr(ortho_projection));
    glBindVertexArray(uiData->vaoHandle);

    for (int index = 0; index < drawData->CmdListsCount; index++)
    {
        const ImDrawList * cmd_list = drawData->CmdLists[index];
        const ImDrawIdx * idx_buffer_offset = 0;
        glBindBuffer(GL_ARRAY_BUFFER, uiData->vboHandle);
        glBufferData(GL_ARRAY_BUFFER,
                     (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert),
                     (GLvoid *)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiData->elementsHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx),
                     (GLvoid *)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

        for (const ImDrawCmd * pcmd = cmd_list->CmdBuffer.begin();
             pcmd != cmd_list->CmdBuffer.end(); pcmd++)
        {
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x,
                          (int)(fb_height - pcmd->ClipRect.w),
                          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                               GL_UNSIGNED_SHORT, idx_buffer_offset);
            }

            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBindVertexArray(last_vertex_array);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFunc(last_blend_src, last_blend_dst);

    if (last_enable_blend) { glEnable(GL_BLEND); }
    else { glDisable(GL_BLEND); }

    if (last_enable_cull_face) { glEnable(GL_CULL_FACE); }
    else { glDisable(GL_CULL_FACE); }

    if (last_enable_depth_test) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }

    if (last_enable_scissor_test) { glEnable(GL_SCISSOR_TEST); }
    else { glDisable(GL_SCISSOR_TEST); }
}

void Interface::Initialize(const RenderWindow &activeWindow,
                           bool instantCallbacks /* = true */)
{
    if (!uiData) { uiData = std::make_unique<UIData>(); }

    uiData->window = activeWindow.Handler();
    ImGuiIO &io = ImGui::GetIO();
    // Keyboard mapping. ImGui will use those indices to peek
    // into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
    io.RenderDrawListsFn = RenderDrawList;
    io.SetClipboardTextFn = SetClipboardText;
    io.GetClipboardTextFn = GetClipboardText;
#ifdef _WIN32
    io.ImeWindowHandle = glfwGetWin32Window(uiData->window);
#endif

    if (instantCallbacks)
    {
        glfwSetMouseButtonCallback(uiData->window, MouseButtonCallback);
        glfwSetScrollCallback(uiData->window, ScrollCallback);
        glfwSetKeyCallback(uiData->window, KeyCallback);
        glfwSetCharCallback(uiData->window, CharCallback);
    }
}

void Interface::Render() const
{
    ImGui::Render();
}

void Interface::NewFrame()
{
    if (!uiData->fontTexture)
    {
        CreateDeviceObjects();
    }

    auto &io = ImGui::GetIO();
    // setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(uiData->window, &w, &h);
    glfwGetFramebufferSize(uiData->window, &display_w, &display_h);
    io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));
    io.DisplayFramebufferScale = ImVec2(static_cast<float>(display_w) / w,
                                        static_cast<float>(display_h) / h);
    // setup time step
    auto current_time = glfwGetTime();
    io.DeltaTime = uiData->time > 0.0
                   ? static_cast<float>(current_time - uiData->time)
                   : static_cast<float>(1.0f / 60.0f);
    uiData->time = current_time;

    // setup inputs
    // (we already got mouse wheel, keyboard keys
    // & characters from glfw callbacks polled in glfwPollEvents())
    if (glfwGetWindowAttrib(uiData->window, GLFW_FOCUSED))
    {
        // Mouse position in screen coordinates
        // (set to -1,-1 if no mouse / on another screen, etc.)
        double mouse_x, mouse_y;
        glfwGetCursorPos(uiData->window, &mouse_x, &mouse_y);
        io.MousePos = ImVec2(static_cast<float>(mouse_x),
                             static_cast<float>(mouse_y));
    }
    else
    {
        io.MousePos = ImVec2(-1, -1);
    }

    for (auto i = 0; i < 3; i++)
    {
        io.MouseDown[i] = uiData->mousePressed[i] ||
                          glfwGetMouseButton(uiData->window, i) != 0;
        // If a mouse press event came, always pass it as
        // "this frame", so we don't miss click-release events
        // that are shorter than 1 frame.
        uiData->mousePressed[i] = false;
    }

    io.MouseWheel = uiData->mouseWheel;
    uiData->mouseWheel = 0.0f;
    // Hide OS mouse cursor if ImGui is drawing it
    glfwSetInputMode(uiData->window, GLFW_CURSOR, io.MouseDrawCursor
                     ? GLFW_CURSOR_HIDDEN
                     : GLFW_CURSOR_NORMAL);
    // Start the frame
    ImGui::NewFrame();
    // ImGui_ImplGlfwGL3_NewFrame();
}


void Interface::CreateFontsTexture()
{
    ImGuiIO &io = ImGui::GetIO();
    // Build texture atlas
    unsigned char * pixels;
    int width, height;
    // Load as RGBA 32-bits for OpenGL3 demo because it is more
    // likely to be compatible with user's existing shader.
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    // Create OpenGL texture
    glGenTextures(1, &uiData->fontTexture);
    glBindTexture(GL_TEXTURE_2D, uiData->fontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);
    // Store our identifier
    io.Fonts->TexID = (void *)(intptr_t)uiData->fontTexture;
    // Cleanup
    io.Fonts->ClearInputData();
    io.Fonts->ClearTexData();
}

void Interface::CreateDeviceObjects()
{
    // Backup GL state
    GLint last_texture, last_array_buffer, last_vertex_array;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    const GLchar * vertex_shader =
        "#version 330\n"
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 UV;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "	Frag_UV = UV;\n"
        "	Frag_Color = Color;\n"
        "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";
    const GLchar * fragment_shader =
        "#version 330\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
        "}\n";
    uiData->shaderHandle = glCreateProgram();
    uiData->vertHandle = glCreateShader(GL_VERTEX_SHADER);
    uiData->fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(uiData->vertHandle, 1, &vertex_shader, 0);
    glShaderSource(uiData->fragHandle, 1, &fragment_shader, 0);
    glCompileShader(uiData->vertHandle);
    glCompileShader(uiData->fragHandle);
    glAttachShader(uiData->shaderHandle, uiData->vertHandle);
    glAttachShader(uiData->shaderHandle, uiData->fragHandle);
    glLinkProgram(uiData->shaderHandle);
    uiData->attribLocationTex = glGetUniformLocation
                                (uiData->shaderHandle, "Texture");
    uiData->attribLocationProjMtx = glGetUniformLocation
                                    (uiData->shaderHandle, "ProjMtx");
    uiData->attribLocationPosition = glGetAttribLocation
                                     (uiData->shaderHandle, "Position");
    uiData->attribLocationUV = glGetAttribLocation
                               (uiData->shaderHandle, "UV");
    uiData->attribLocationColor = glGetAttribLocation
                                  (uiData->shaderHandle, "Color");
    glGenBuffers(1, &uiData->vboHandle);
    glGenBuffers(1, &uiData->elementsHandle);
    glGenVertexArrays(1, &uiData->vaoHandle);
    glBindVertexArray(uiData->vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, uiData->vboHandle);
    glEnableVertexAttribArray(uiData->attribLocationPosition);
    glEnableVertexAttribArray(uiData->attribLocationUV);
    glEnableVertexAttribArray(uiData->attribLocationColor);
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    glVertexAttribPointer(uiData->attribLocationPosition, 2, GL_FLOAT,
                          GL_FALSE, sizeof(ImDrawVert), (GLvoid *)
                          OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(uiData->attribLocationUV, 2, GL_FLOAT,
                          GL_FALSE, sizeof(ImDrawVert), (GLvoid *)
                          OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(uiData->attribLocationColor, 4, GL_UNSIGNED_BYTE,
                          GL_TRUE, sizeof(ImDrawVert), (GLvoid *)
                          OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
    CreateFontsTexture();
    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindVertexArray(last_vertex_array);
}

void Interface::Terminate()
{
    if (uiData->vaoHandle) { glDeleteVertexArrays(1, &uiData->vaoHandle); }

    if (uiData->vboHandle) { glDeleteBuffers(1, &uiData->vboHandle); }

    if (uiData->elementsHandle) { glDeleteBuffers(1, &uiData->elementsHandle); }

    uiData->vaoHandle = uiData->vboHandle = uiData->elementsHandle = 0;
    glDetachShader(uiData->shaderHandle, uiData->vertHandle);
    glDeleteShader(uiData->vertHandle);
    uiData->vertHandle = 0;
    glDetachShader(uiData->shaderHandle, uiData->fragHandle);
    glDeleteShader(uiData->fragHandle);
    uiData->fragHandle = 0;
    glDeleteProgram(uiData->shaderHandle);
    uiData->shaderHandle = 0;

    if (uiData->fontTexture)
    {
        glDeleteTextures(1, &uiData->fontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        uiData->fontTexture = 0;
    }

    ImGui::Shutdown();
    delete uiData.release();
}

void Interface::InvalidateDeviceObjects()
{
}

void Interface::MouseButtonCallback(GLFWwindow * window, int button, int action,
                                    int mods)
{
    if (action == GLFW_PRESS && button >= 0 && button < 3)
    {
        uiData->mousePressed[button] = true;
    }
}

void Interface::ScrollCallback(GLFWwindow * window, double xoffset,
                               double yoffset)
{
    // Use fractional mouse wheel, 1.0 unit 5 lines.
    uiData->mouseWheel += static_cast<float>(yoffset);
}

void Interface::KeyCallback(GLFWwindow * window, int key, int scancode,
                            int action, int mods)
{
    auto &io = ImGui::GetIO();

    if (action == GLFW_PRESS)
    {
        io.KeysDown[key] = true;
    }

    if (action == GLFW_RELEASE)
    {
        io.KeysDown[key] = false;
    }

    (void)mods; // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] ||
                 io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] ||
                  io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] ||
                io.KeysDown[GLFW_KEY_RIGHT_ALT];
}

void Interface::CharCallback(GLFWwindow * window, unsigned int c)
{
    auto &io = ImGui::GetIO();

    if (c > 0 && c < 0x10000)
    {
        io.AddInputCharacter(static_cast<unsigned short>(c));
    }
}

void Interface::SetClipboardText(const char * text)
{
    glfwSetClipboardString(uiData->window, text);
}

const char * Interface::GetClipboardText()
{
    return glfwGetClipboardString(uiData->window);
}