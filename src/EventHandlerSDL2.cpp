/**
 * @file EventHandlerSDL2.cpp
 * @author Brais Solla González
 * @brief Enyx GL Event Handler for SDL2
 * @version 0.1
 * @date 2021-09-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "EventHandlerSDL2.h"

EventHandler::EventHandler(){
    this->window = NULL;
    this->mouse  = NULL;   
}


bool EventHandler::isRunning(){
    return this->running;
}

void EventHandler::attachKeyboard(void* keyboard){
    // TODO
}

void EventHandler::attachMouse(MouseSDL2* mouse){
    this->mouse = mouse;
}

void EventHandler::attachWindow(WindowSDL2* window){
    this->window = window;
}


void EventHandler::processEvents(){
    // Process SDL events
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            this->running = false;
        }

        if(event.type == SDL_MOUSEMOTION){
            SDL_MouseMotionEvent* mouseEvent = (SDL_MouseMotionEvent*) &event;

            
        }

    }

}