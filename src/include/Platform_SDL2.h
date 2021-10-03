/**
 * @file Platform_SDL2.h
 * @author Brais Solla González
 * @brief Enyx GL Platform definition for libSDL2 backend
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef PLATFORM_SDL2
#define PLATFORM_SDL2

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define WINDOW_DEFAULT_WIDTH  800
#define WINDOW_DEFAULT_HEIGHT 600

typedef void (*event_cb_t)(void*);

enum event_t {
    EVENT_UNKNOWN    = 0,
    EVENT_COMMON     = 1,
    EVENT_WINDOW     = 2,
    EVENT_KEYBOARD   = 3,
    EVENT_MOUSE      = 4,
    EVENT_CONTROLLER = 5,
    EVENT_TOUCH      = 6,
    EVENT_EXIT       = 7
};

struct event_info_t {
    uint32_t total_events;
    uint32_t unknown_events;
    uint32_t common_events;
    uint32_t window_events;
    uint32_t keyboard_events;
    uint32_t mouse_events;
    uint32_t touch_events;
    uint32_t exit_events;
};


namespace Events {
    int  initEventSystem();

    bool isAppRunning(); // Returns false when exit event is triggered
    void processEvents();
    void attachCallback(event_t event, event_cb_t event_cb);

    event_info_t getEventsInfo();
};

namespace System {
    // Time functions
    uint32_t millis();
    uint64_t micros();
    void     delay(uint32_t millis);
    void     delayMicroseconds(uint64_t micros);

    // Platform info (Windows, Mac OS X, Linux, iOS, Android, ...)
    const char* getPlatform();

    // Processor info
    int getCPUCount();
    int getCPUCacheLineSize();
    int getSystemRAM();        // Total amount of RAM in KILOBYTES!

    // System power information

    // Screen resolution


    // Android specific
    void*          AndroidGetJNI();

    const char*    AndroidGetInternalStoragePath();
    const char*    AndroidGetExternalStoragePath();
    int            AndroidGetExternalStorageState();
};


class Window {
    private:
        SDL_Window* window;
    public:
        Window();
        Window(const char* title);
        ~Window();

        void close();
        void minimize();
        void maximize();
        void restore();
        void hide();
        void show();
        void resize(int width, int height);

        bool isResizable();

        // SDL OpenGL context
        void GL_MakeCurrent(SDL_GLContext context);
        int  GL_SetAttribute(SDL_GLattr attr, int value);
        int  GL_SetSwapInterval(int interval);

        bool  GL_ExtensionSupported(const char* extension);
        void* GL_GetProcAddress(const char* proc);

        SDL_GLContext GL_CreateContext();
        void GL_DeleteContext(SDL_GLContext context);
        void GL_SwapWindow();
};







#endif