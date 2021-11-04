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


    while(Events::isAppRunning()){
        Events::processEvents();
        agl.clear();
        agl.origin();

        // Pixel lines
        for(int i = 0; i < 800; i++){
            if(i % 3){
                agl.drawPixel(i, 4, RED);
            } else {
                agl.drawPixel(i, 4, BLUE);
            }

            agl.drawPixel(i, 12, WHITE);
        }

        // Lines
        agl.drawLine(2, 18, 798, 18, MAGENTA);
        agl.drawLine(2, 24, 798, 24, RED, GREEN);

        // Rectangles
        agl.drawFillRect(40,40,32,32, GREEN);
        agl.drawRect(40,40,32,32, WHITE);

        agl.drawFillCircle(400,300,100, MAGENTA, CYAN);
        agl.drawFillTriangle(350, 300, 300, 400, 400, 400, RED, GREEN, BLUE);
        agl.drawTriangle(350, 300, 300, 400, 400, 400, WHITE);

        agl.render();
    }

    agl.destroy();
    window.close();
    return 0;
}