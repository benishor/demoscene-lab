#ifdef __APPLE__
    #define NO_SDL_GLEXT
#endif

#include <Window.h>
#include <GLheaders.h>
#include <GLMheaders.h>
#include <DemoData.h>
#include <SDL2/SDL_opengl.h>

namespace Acidrain {

Window::Window(int w, int h, WindowType t)
    : width(w), height(h), type(t) {
    SDL_Init(SDL_INIT_VIDEO);

    // Setup OpenGL version. on OSX this has the effect of not drawing anything with them shaders. Need to investigate further
#ifndef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

    // setup stencil buffer
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // use double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // setup depth buffer
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // setup anti aliasing
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);


    Uint32 windowFlags = SDL_WINDOW_OPENGL;
    if (type == WindowType::Fullscreen)
        windowFlags |= SDL_WINDOW_FULLSCREEN;

    displayWindow = SDL_CreateWindow("funDAmental",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     width,
                                     height,
                                     windowFlags);
    glContext = SDL_GL_CreateContext(displayWindow);

#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error %s\n", glewGetErrorString(err));
        exit(1);
    }
#endif

    SDL_GL_SetSwapInterval(1);

    DemoData::WIDTH  = width;
    DemoData::HEIGHT = height;
}

Window::~Window() {
    // restore our video mode
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(displayWindow);
    SDL_Quit();
}

void Window::present() {
    SDL_GL_SwapWindow(displayWindow);
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

} // namespace Acidrain
