#ifdef __APPLE__
#define NO_SDL_GLEXT
#endif

#include <Window.h>
#include <GLheaders.h>

#ifdef _WIN32
#include <SDL_syswm.h>
#include <windows.h>
#endif

#include <string>
#include <map>
#include <iostream>

namespace Acidrain {

    static std::map<GLenum, std::string> OGL_DEBUG_SOURCE_MAP = {
            {GL_DEBUG_SOURCE_API,             "API"},
            {GL_DEBUG_SOURCE_WINDOW_SYSTEM,   "WINDOW_SYSTEM"},
            {GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
            {GL_DEBUG_SOURCE_THIRD_PARTY,     "THIRD_PARTY"},
            {GL_DEBUG_SOURCE_APPLICATION,     "APPLICATION"},
            {GL_DEBUG_SOURCE_OTHER,           "OTHER"}
    };

    static std::map<GLenum, std::string> OGL_DEBUG_TYPE_MAP = {
            {GL_DEBUG_TYPE_ERROR,               "ERROR"},
            {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
            {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,  "UNDEFINED_BEHAVIOR"},
            {GL_DEBUG_TYPE_PORTABILITY,         "PORTABILITY"},
            {GL_DEBUG_TYPE_PERFORMANCE,         "PERFORMANCE"},
            {GL_DEBUG_TYPE_MARKER,              "MARKER"},
            {GL_DEBUG_TYPE_PUSH_GROUP,          "PUSH_GROUP"},
            {GL_DEBUG_TYPE_POP_GROUP,           "POP_GROUP"},
            {GL_DEBUG_TYPE_OTHER,               "OTHER"},
    };

    static std::map<GLenum, std::string> OGL_DEBUG_SEVERITY_MAP = {
            {GL_DEBUG_SEVERITY_HIGH,         "HIGH"},
            {GL_DEBUG_SEVERITY_MEDIUM,       "MEDIUM"},
            {GL_DEBUG_SEVERITY_LOW,          "LOW"},
            {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}
    };

    static void openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        if (severity != GL_DEBUG_SEVERITY_NOTIFICATION && severity != GL_DEBUG_SEVERITY_LOW) {
            std::cout << "OGL >> " << OGL_DEBUG_SOURCE_MAP[source] << " [" << OGL_DEBUG_TYPE_MAP[type] << ", " << OGL_DEBUG_SEVERITY_MAP[severity] << "] " << std::string(message, length) << std::endl;
        }
    }

    Window::Window(int w, int h, WindowType t)
            : width(w), height(h), type(t) {

        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Set up OGL 3.2 core profile. No more deprecated shit
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Set up context debug flag
        int result = SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        Uint32 windowFlags = SDL_WINDOW_OPENGL;
        if (type == WindowType::Fullscreen)
            windowFlags |= SDL_WINDOW_FULLSCREEN;

        displayWindow = SDL_CreateWindow("FunDaMental",
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         width,
                                         height,
                                         windowFlags);
        glContext = SDL_GL_CreateContext(displayWindow);
        glEnable(GL_MULTISAMPLE);

//        int queriedMultiSampleBuffers;
//        int queriedMultiSampleSamples;
//        result = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &queriedMultiSampleBuffers);
//        LOG(INFO) << "getting multisample buffers returned " << result;
//        result = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &queriedMultiSampleSamples);
//        LOG(INFO) << "getting multisample buffers returned " << result;
//        LOG(INFO) << "Querying previously set values. Got multi-sample buffers set to " << queriedMultiSampleBuffers << " and multi-sample samples set to " << queriedMultiSampleSamples;


#ifndef __APPLE__
        // Thanks, derhass!
        // [17:49] <derhass> my thought is that you run into the GLEW bug with core profiles
        // [17:50] <benishor> derhass: sounds reasonable, if there is such a bug. any way to fix it?
        // [17:50] <derhass> benishor: did you set glewExpreimental = GL_TRUE before the glewInit()?
        // [17:50] <benishor> no, I did not
        // [17:50] <benishor> should I have?
        // [17:50] <derhass> yes
        // [17:51] <derhass> this is the "workaround" for GLEW in core profiles
        // [17:53] <derhass> note that glewExperimental is just a HACK
        // [17:53] <derhass> it will a) trigger a GL error itself during glewInit (which you should read away with glGetError)
        // [17:54] <derhass> and it will totally screw up the detection of the available extensions
        // [17:54] <derhass> benishor: http://stackoverflow.com/questions/22619406/why-does-glew-say-that-i-dont-have-extensions/22619644#22619644
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        glGetError(); // see above
        if (GLEW_OK != err) {
            std::cerr << "Failed to initialize glew. Reason: " << glewGetErrorString(err);
        }
#endif

        if (glDebugMessageCallback) {
            glDebugMessageCallback(openGlDebugCallback, 0);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            std::cout << "OpenGL debug callback installed";
        } else {
            std::cout << "glDebugMessageCallback not available";
        }

        SDL_GL_SetSwapInterval(1);
    }

    Window::~Window() {
        // restore our video mode
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(displayWindow);
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
