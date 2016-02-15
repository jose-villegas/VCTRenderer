#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "interface.h"

Interface::Interface() : io(ImGui::GetIO())
{
}

Interface::~Interface()
{
}

void Interface::DrawAll()
{
    for (auto &ui : instances)
    {
        ui->Draw();
    }
}
