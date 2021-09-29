/**
 * @file Events.h
 * @author Brais Solla González
 * @brief Events for Enyx GL (LibSDL2)
 * @version 0.1
 * @date 2021-09-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _EVENTS_INCLUDED
#define _EVENTS_INCLUDED

#include <SDL2/SDL.h>

#include "WindowSDL2.h"
#include "MouseSDL2.h"

class EventHandler {
    private:
        bool running;

        MouseSDL2*  mouse;
        WindowSDL2* window;
    public:
        EventHandler();
        // ~EventHandler();

        bool isRunning();

        void attachMouse(MouseSDL2* mouse);
        void attachKeyboard(void* keyboard); // TODO: Write keyboard interface
        void attachWindow(WindowSDL2* window);


        

        void processEvents();


};

#endif