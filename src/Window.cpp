#include <Window.h>
#include <GLheaders.h>
#include <SDL2/SDL_opengl.h>

Window::Window(int w, int h, WindowType t)
    : width(w), height(h), type(t) {
    SDL_Init(SDL_INIT_VIDEO);

    Uint32 windowFlags = SDL_WINDOW_OPENGL;
    if (type == WindowType::Fullscreen)
        windowFlags |= SDL_WINDOW_FULLSCREEN;

    SDL_CreateWindowAndRenderer(width, height, windowFlags, &displayWindow, &displayRenderer);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error %s\n", glewGetErrorString(err));
        exit(1);
    }

    initOpenGL();
}

Window::~Window() {
    // restore our video mode
    SDL_Quit();
}

void Window::present() {
    SDL_RenderPresent(displayRenderer);
}

bool Window::shouldQuit() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return true;
        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            return true;
    }

    return false;
}

void Window::initOpenGL() {
    // Set up our viewing matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const double ASPECT_RATIO = width / (double)height;
    const double FIELD_OF_VIEW_IN_DEGREES = 45.0;
    const double Z_NEAR = 0.2;
    const double Z_FAR = 255;
    gluPerspective(FIELD_OF_VIEW_IN_DEGREES, ASPECT_RATIO, Z_NEAR, Z_FAR);

    // No transformations on the model
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
