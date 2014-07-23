#include <Window.h>
#include <GLheaders.h>
#include <SDL2/SDL_opengl.h>

Window::Window(int w, int h, WindowType t)
    : width(w), height(h), type(t) {
    SDL_Init(SDL_INIT_VIDEO);

    // setup OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error %s\n", glewGetErrorString(err));
        exit(1);
    }

    SDL_GL_SetSwapInterval(1);
    initOpenGL();
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
