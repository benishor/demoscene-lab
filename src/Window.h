#pragma once

#include <SDL2/SDL.h>

enum class WindowType {
    Windowed,
    Fullscreen
};

class Window {
public:
    Window(int width, int height, WindowType type);
    virtual ~Window();

    void present();
    void pollEvents();

private:
    void initOpenGL();

    int width;
    int height;
    WindowType type;

    SDL_Window* displayWindow;
    SDL_Renderer* displayRenderer;
};
