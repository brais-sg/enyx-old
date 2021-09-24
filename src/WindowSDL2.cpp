/**
 * @file WindowSDL2.cpp
 * @author Brais Solla González
 * @brief Window SDL2 implementation for Enyx GL Engine
 * @version 0.1
 * @date 2021-09-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "WindowSDL2.h"

WindowSDL2::WindowSDL2(){
    window   = NULL;
    gContext = NULL;

    wx = DEFAULT_WINDOW_WIDTH;
    wy = DEFAULT_WINDOW_HEIGHT;
}


WindowSDL2::~WindowSDL2(){
    if(window){
        this->destroy();
    }
}


int WindowSDL2::init(const char* title){
    fprintf(stderr,"[%s:%d] WindowSDL2::init: Starting SDL2...\n", __FILE__, __LINE__);
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr,"[%s:%d] WindowSDL2::init: SDL2 error: %s\n", __FILE__, __LINE__, SDL_GetError());
        return -1;
    }

    fprintf(stderr,"[%s:%d] WindowSDL2::init: Creating window...\n", __FILE__, __LINE__);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wx, wy, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if(window == NULL){
        fprintf(stderr,"[%s:%d] WindowSDL2::init: Cannot create SDL2 Window...\n", __FILE__, __LINE__);
        return -2;
    }

    return 0;
}

int WindowSDL2::destroy(){
    if(gContext){
        SDL_GL_DeleteContext(gContext);
        gContext = NULL;
    }

    if(window){
        SDL_DestroyWindow(window);
        window = NULL;
        return 0;
    }

    return -1;
}

int WindowSDL2::getWidth(){
    this->updateDimensions(); // Optimize this
    return wx;
}

int WindowSDL2::getHeight(){
    this->updateDimensions(); // Optimize this
    return wy;
}

int WindowSDL2::getDepth(){
    // For now assume 24 bits / pixel, depth 3
    return 3;
}

int WindowSDL2::resize(int width, int heigth){
    SDL_SetWindowSize(window, width, heigth);
    this->updateDimensions();
    
    if(wx == width && wy == heigth) return 0;
    return 1; // Error condition
}

int WindowSDL2::maximize(){
    SDL_SetWindowResizable(window, SDL_TRUE);
    SDL_MaximizeWindow(window);

    this->updateDimensions();
    return 0;
}

int WindowSDL2::minimize(){
    SDL_SetWindowResizable(window, SDL_TRUE);
    SDL_MinimizeWindow(window);

    this->updateDimensions();
    return 0;
}

int WindowSDL2::restore(){
    SDL_SetWindowResizable(window, SDL_TRUE);
    SDL_RestoreWindow(window);

    this->updateDimensions();
    return 0;
}

int WindowSDL2::close(){
    return this->destroy();
}

int WindowSDL2::move(int px, int py){
    SDL_SetWindowPosition(window, px, py);
    return 0;
}

int WindowSDL2::getPosition(int* px, int* py){
    SDL_GetWindowPosition(window, px, py);
    return 0;
}

int WindowSDL2::setTitle(const char* title){
    SDL_SetWindowTitle(window, title);
    return 0;
}

void WindowSDL2::updateDimensions(){
    SDL_GetWindowSize(window, &wx, &wy);
}


// Specifics to SDL2 to allow renderer context creation
int WindowSDL2::createGLContext(){
    this->gContext = SDL_GL_CreateContext(window);
    if(this->gContext){
        return 0;
    } else {
        fprintf(stderr, "[%s:%d]: WindowSDL2::createGLContext(): Error creating context: %s\n", __FILE__, __LINE__, SDL_GetError());
        return -1;
    }
}

SDL_GLContext WindowSDL2::getGLContext(){
    return this->gContext;
}

SDL_Window* WindowSDL2::getWindow(){
    return this->window;
}

void WindowSDL2::GL_swapWindow(){
    SDL_GL_SwapWindow(window);
}

int WindowSDL2::SDL_GL_SetAttribute(SDL_GLattr attr, int value){
    return SDL_GL_SetAttribute(attr, value);
}

int WindowSDL2::SDL_GL_GetAttribute(SDL_GLattr attr, int *value){
    return SDL_GL_GetAttribute(attr, value);
}

