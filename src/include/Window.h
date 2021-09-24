/**
 * @file Window.h
 * @author Brais Solla González
 * @brief Window class abstraction
 * @version 0.1
 * @date 2021-09-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _WINDOW_INCLUDED
#define _WINDOW_INCLUDED

class Window {
    public:
        // Init window
        virtual int init(const char* title) = 0;
        virtual int destroy()               = 0;

        // Called every frame to update buffers
        virtual int update()                = 0;

        // Dimensions
        virtual int  getWidth()  = 0;
        virtual int  getHeight() = 0;
        virtual int  getDepth()  = 0;

        // Resize 
        virtual int resize(int width, int heigth) = 0;
        virtual int maximize()                    = 0;
        virtual int minimize()                    = 0;
        virtual int restore()                     = 0;
        virtual int close()                       = 0;
        virtual int move(int px, int py)          = 0;
        virtual int getPosition(int* px, int* py) = 0;
        virtual int setTitle(const char* title)   = 0;

        // Screen size
        virtual int getScreenResolution(int* px, int* py) = 0;

        virtual int fullscreen()                  = 0;

        virtual bool isFullscreen()               = 0;
        // 0 No, 1: Yes, -1: Enable late swap
        virtual int setVSync(int vsync)           = 0;


        // Events: Keyboard + Mouse
        // RELATIVE TO WINDOW!
        virtual int getMouse(int* px, int* py)    = 0;





};

#endif