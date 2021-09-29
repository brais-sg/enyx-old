/**
 * @file MouseSDL2.h
 * @author Brais Solla González
 * @brief Mouse SDL2 driver for Enyx GL
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _MOUSESDL2_INCLUDED
#define _MOUSESDL2_INCLUDED

#include "Mouse.h"

// TODO: Use events for this!
class MouseSDL2 : public Mouse {
    private:
        uint32_t lastState;
        uint32_t currentState;

        MousePosition heldPosition;
        MousePosition lastPosition;
        MousePosition currentPosition;
    public:
        MouseSDL2();
        ~MouseSDL2();

        MouseStatus   getMouse();
        int           setMouse(MousePosition position);
        int           setMouseGlobal(MousePosition position);
        int           showCursor(bool show);



        // Interface for SDL mouse events
        int           SDL_MouseUpdateEvent(MouseStatus newStatus);
};
#endif