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
