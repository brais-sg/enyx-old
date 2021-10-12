/**
 * @file Test.cpp
 * @author Brais Solla González
 * @brief Enyx GL Test 
 * @version 0.1
 * @date 2021-09-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Enyx.h"

Window window;
Renderer_SDL2_GLES2 agl;

int main(){
    printf("Enyx begin!\n");

    Events::initEventSystem();
    window.init("Enyx main window");
    agl.setWindow(&window);
    if(agl.init()){
        fprintf(stderr, "Cannot start AGL!\n");
        return -1;
    };


    while(Events::isAppRunning()){
        Events::processEvents();
        agl.clear();
        agl.origin();

        agl.drawFillRect(100,100,200,200, RGBA(255,255,255,127));
        agl.drawFillRect(80,80,240, 240, RGBA(255,127,64,64));




        agl.render();
    }


    window.close();
    return 0;
}