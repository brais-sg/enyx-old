/**
 * @file Mouse.cpp
 * @author Brais Solla González
 * @brief SDL2 Mouse driver for Enyx GL
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "Mouse.h"
#include "MouseSDL2.h"

MouseSDL2::MouseSDL2(){
    // Register to the event driver (If any)
}

MouseSDL2::~MouseSDL2(){
    // Nothing to do here
}

MouseStatus MouseSDL2::getMouse(){
    MouseStatus currentMouseStatus = MouseStatus();
    currentMouseStatus.buttons  = this->currentState;
    currentMouseStatus.position = this->currentPosition;

    return currentMouseStatus;
}


int MouseSDL2::setMouse(MousePosition position){
    this->currentPosition = position;
    return 1;
}

int MouseSDL2::setMouseGlobal(MousePosition position){
    return 1;
}

int MouseSDL2::showCursor(bool show){
    return 1;
}

