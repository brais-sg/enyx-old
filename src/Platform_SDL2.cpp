/**
 * @file Platform_SDL2.cpp
 * @author Brais Solla González
 * @brief Enyx GL SDL2 Platform implementation
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "Platform_SDL2.h"

// EVENT SYSTEM CALLBACKS
static SDL_Window* default_window = NULL;
static bool        app_running    = false;

static event_handlers_struct_t event_handlers;
static event_info_t            event_info;

static void eventSystemCB_commonEventHandler(void* data){

}

static void eventSystemCB_windowEventHandler(void* data){

}

static void eventSystemCB_keyboardEventHandler(void* data){

}

static void eventSystemCB_mouseEventHandler(void* data){

}

static void eventSystemCB_controllerEventHandler(void* data){

}

static void eventSystemCB_touchEventHandler(void* data){

}

static void eventSystemCB_exitEventHandler(void* data){
    fprintf(stderr, "[%s:%d] INFO: Exit event handler called!\n",__FILE__, __LINE__);
    // Exit event received, 
    app_running = false;
}


int Events::initEventSystem(){
    // Zero handlers / info structs
    memset(&event_handlers, 0, sizeof(event_handlers_struct_t));
    memset(&event_info, 0, sizeof(event_info_t));

    // Init the event system
    Events::attachCallback(EVENT_COMMON,     eventSystemCB_commonEventHandler);
    Events::attachCallback(EVENT_WINDOW,     eventSystemCB_windowEventHandler);
    Events::attachCallback(EVENT_KEYBOARD,   eventSystemCB_keyboardEventHandler);
    Events::attachCallback(EVENT_MOUSE,      eventSystemCB_mouseEventHandler);
    Events::attachCallback(EVENT_CONTROLLER, eventSystemCB_controllerEventHandler);
    Events::attachCallback(EVENT_TOUCH,      eventSystemCB_touchEventHandler);
    Events::attachCallback(EVENT_EXIT,       eventSystemCB_exitEventHandler);

    // App is running now!
    app_running = true;
    return 0;
}

bool Events::isAppRunning(){
    return app_running;
}

void Events::processEvents(){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_WINDOWEVENT:
                event_info.total_events++;
                event_info.window_events++;
                if(event_handlers.window_handler) event_handlers.window_handler(&e);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                event_info.total_events++;
                event_info.keyboard_events++;
                if(event_handlers.keyboard_handler) event_handlers.keyboard_handler(&e);
                break;
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
                event_info.total_events++;
                event_info.mouse_events++;
                if(event_handlers.mouse_handler) event_handlers.mouse_handler(&e);
                break;
            case SDL_CONTROLLERAXISMOTION:
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERDEVICEADDED:
            case SDL_CONTROLLERDEVICEREMOVED:
            case SDL_CONTROLLERDEVICEREMAPPED:
                event_info.total_events++;
                event_info.controller_events++;
                if(event_handlers.controller_handler) event_handlers.controller_handler(&e);
                break;
            case SDL_FINGERMOTION:
            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
            case SDL_MULTIGESTURE:
                event_info.total_events++;
                event_info.touch_events++;
                if(event_handlers.touch_handler) event_handlers.touch_handler(&e);
                break;
            case SDL_QUIT:
                event_info.total_events++;
                event_info.exit_events++;
                if(event_handlers.exit_handler) event_handlers.exit_handler(&e);
                break;
            default:
                fprintf(stderr,"[%s:%d]: INFO: Unknown event\n", __FILE__, __LINE__);
        }
    }
}


void Events::attachCallback(event_t event, event_cb_t event_cb){
    switch(event){
        case EVENT_UNKNOWN:
            event_handlers.unknown_handler    = event_cb;
            break;
        case EVENT_COMMON:
            event_handlers.common_handler     = event_cb;
            break;
        case EVENT_WINDOW:
            event_handlers.window_handler     = event_cb;
            break;
        case EVENT_KEYBOARD:
            event_handlers.keyboard_handler   = event_cb;
            break;
        case EVENT_MOUSE:
            event_handlers.mouse_handler      = event_cb;
            break;
        case EVENT_CONTROLLER:
            event_handlers.controller_handler = event_cb;
            break;
        case EVENT_TOUCH:
            event_handlers.touch_handler      = event_cb;
            break;
        case EVENT_EXIT:
            event_handlers.exit_handler       = event_cb;
            break;
        default:
            fprintf(stderr, "[%s:%d] Events::attachCallback(): WARNING: Unknown event type.\n", __FILE__, __LINE__);
    }
}

event_info_t Events::getEventsInfo(){
    return event_info;
}


// System
uint32_t System::millis(){
    return (uint32_t) SDL_GetTicks();
}

uint64_t System::micros(){
    // TODO: Improve resolution!
    return (uint64_t) SDL_GetTicks() * 1000;
}

void System::delay(uint32_t millis){
    SDL_Delay((uint32_t) millis);
}

void System::delayMicroseconds(uint64_t micros){
    // TODO: Improve resolution!
    SDL_Delay((uint32_t) micros / 1000);
}

const char* System::getPlatform(){
    return SDL_GetPlatform();
}

int System::getCPUCount(){
    return SDL_GetCPUCount();
}

int System::getCPUCacheLineSize(){
    return SDL_GetCPUCacheLineSize();
}

int System::getSystemRAM(){
    return SDL_GetSystemRAM() * 1024;
}


// Android specific (NOT IMPLEMENTED FOR NOW)
void* System::AndroidGetJNI(){
    return NULL;
}

const char* System::AndroidGetInternalStoragePath(){
    return NULL;
}

const char* System::AndroidGetExternalStoragePath(){
    return NULL;
}

int System::AndroidGetExternalStorageState(){
    return 0;
}

