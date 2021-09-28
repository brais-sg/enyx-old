/**
 * @file WindowSDL2.h
 * @author Brais Solla González
 * @brief Window SDL2 implementation for Enyx GL
 * @version 0.1
 * @date 2021-09-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _WINDOWSDL2_INCLUDED
#define _WINDOWSDL2_INCLUDED

#include <SDL2/SDL.h>
#include "Window.h"

#ifndef DEFAULT_WINDOW_WIDTH
#define DEFAULT_WINDOW_WIDTH 800
#endif

#ifndef DEFAULT_WINDOW_HEIGHT
#define DEFAULT_WINDOW_HEIGHT 600
#endif


class WindowSDL2 : public Window {
    private:
        int wx, wy;

        SDL_Window* window;
        SDL_GLContext gContext;
    public:
        WindowSDL2();
        ~WindowSDL2();

        int init(const char* title);
        int destroy();

        int  getWidth();
        int  getHeight();
        int  getDepth();

        int resize(int width, int heigth);
        int maximize();
        int minimize();
        int restore();
        int close();
        int move(int px, int py);
        int getPosition(int* px, int* py);
        int setTitle(const char* title);

        //int getScreenResolution(int* px, int* py);

        //int fullscreen();

        //bool isFullscreen();

        // SDL2 specifics to interface renderer
        int createGLContext();
        SDL_GLContext getGLContext();
        SDL_Window*   getWindow();

        void GL_swapWindow();
        int SDL_GL_SetAttribute(SDL_GLattr attr, int value);
        int SDL_GL_GetAttribute(SDL_GLattr attr, int *value);

        void updateDimensions();





};

#endif