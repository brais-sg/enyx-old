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
/**
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

        //agl.drawFillRect(100,100,200,200, RGBA(255,255,255,127));
        //agl.drawFillRect(120,120,160,160, RGBA(0,0,0,64));

        agl.drawFillRect(0,0, 800, 600, GREEN);
        
        agl.scissor(200,600-200,50,50);
        agl.drawFillTriangle(200, 200, 100, 400, 300, 400, RED, GREEN, BLUE);
        agl.scissor();



        agl.render();
    }


    window.close();
    return 0;
}
**/
RGLES2 agl;

int main(){
    Events::initEventSystem();
    window.init("RGLES2 Hello world!");
    agl.setWindow(&window);
    if(agl.init() != 0){
        fprintf(stderr, "RGLES2 error!\n");
        return -1;
    }


    float angle = 0.f;

    while(Events::isAppRunning()){
        Events::processEvents();
        agl.clear();
        agl.origin();

        agl.translate(150, 150);
        agl.rotate(angle);
        agl.translate(-50, -50);

        for(int i = 0; i < 150; i++){
            agl.drawPixel(0, i, RED);
            agl.drawPixel(i, 0, BLUE);
        }

        angle+= 0.05f;

        agl.render();
    }


    window.close();
    return 0;
}