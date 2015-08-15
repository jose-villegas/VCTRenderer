#pragma once
struct WindowSettings
{
    unsigned int width;
    unsigned int height;
    WindowSettings() {};
    virtual ~WindowSettings() {};
};

class RenderWindow
{
    public:
        RenderWindow();
        virtual ~RenderWindow();
    private:
        WindowSettings settings;
};