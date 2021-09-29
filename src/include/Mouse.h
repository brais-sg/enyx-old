/**
 * @file Mouse.h
 * @author Brais Solla González
 * @brief Mouse interface for Enyx GL
 * @version 0.1
 * @date 2021-09-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _MOUSE_INCLUDED
#define _MOUSE_INCLUDED
#include <stdint.h>
#include <SDL2/SDL.h>

#ifndef _BV
#define _BV(x) ((1) << (x))
#endif 

#define MOUSE_BUTTON_LEFT    _BV(0)
#define MOUSE_BUTTON_MIDDLE  _BV(1)
#define MOUSE_BUTTON_RIGHT   _BV(2)
#define MOUSE_BUTTON_X1      _BV(3)
#define MOUSE_BUTTON_X2      _BV(4)


struct MousePosition {
    int x, y;
};

struct MouseStatus {
    MousePosition position;
    uint32_t      buttons;
};

class Mouse {
    public:
        virtual MouseStatus   getMouse() = 0;
        virtual int           setMouse(MousePosition position) = 0;
        virtual int           setMouseGlobal(MousePosition position) = 0;
        virtual int           showCursor(bool show) = 0;
};


#endif