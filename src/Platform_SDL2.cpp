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
static Window*  default_window = NULL;
static bool     app_running    = false;

static event_handlers_struct_t event_handlers;
static event_info_t            event_info;

static void eventSystemCB_commonEventHandler(void* data){

}

static void eventSystemCB_windowEventHandler(void* data){
    if(default_window){
        default_window->window_handler_event(data);
    }
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

// Enyx
// Init Enyx GL subsystems (And SDL)
int Enyx::init(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr, "[%s:%d]: FATAL: Cannot init SDL2: Error: %s\n", __FILE__, __LINE__, SDL_GetError());
        exit(-1);
        return -1; // Never reached
    }

    return 0;
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

powerstate_t System::getPowerstate(){
    switch(SDL_GetPowerInfo(NULL, NULL)){
        case SDL_POWERSTATE_UNKNOWN:
            return POWERSTATE_UNKNOWN;
            break;
        case SDL_POWERSTATE_ON_BATTERY:
            return POWERSTATE_ON_BATTERY;
            break;
        case SDL_POWERSTATE_NO_BATTERY:
            return POWERSTATE_NO_BATTERY;
            break;
        case SDL_POWERSTATE_CHARGING:
            return POWERSTATE_CHARGING;
            break;
        case SDL_POWERSTATE_CHARGED:
            return POWERSTATE_CHARGED;
            break;
        default:
            return POWERSTATE_UNKNOWN;
    }
}

int System::getBatteryPercent(){
    int percent;
    if(SDL_GetPowerInfo(NULL, &percent) == SDL_POWERSTATE_UNKNOWN){
        return -1;
    }
    return percent;
}

int System::getBatteryLife(){
    int seconds;
    if(SDL_GetPowerInfo(&seconds, NULL) == SDL_POWERSTATE_UNKNOWN){
        return -1;
    }
    return seconds;
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

// Window class implementation

Window::Window(){
    this->window = NULL;
}

Window::Window(const char* title){
    this->window = NULL;
    this->init(title);
}

Window::~Window(){
    if(this->window){
        this->close();
    }
}


int Window::init(const char* title){
    this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if(this->window){
        if(default_window == NULL) default_window == this;
        return 0;
    } else {
        fprintf(stderr, "[%s:%d] ERROR: Cannot create Window! Error: %s\n", __FILE__,__LINE__, SDL_GetError());
        return -1;
    }
}

void Window::close(){
    if(default_window) default_window = NULL;
    fprintf(stderr, "[%s:%d]: INFO: Deleting window %p\n", __FILE__, __LINE__, this);
    SDL_DestroyWindow(this->window);
    this->window = NULL;
}

void Window::minimize(){
    SDL_MinimizeWindow(this->window);
}

void Window::maximize(){
    SDL_MaximizeWindow(this->window);
}

void Window::restore(){
    SDL_RestoreWindow(this->window);
}

void Window::hide(){
    SDL_HideWindow(this->window);
}

void Window::show(){
    SDL_ShowWindow(this->window);
}

void Window::resize(int width, int height){
    SDL_SetWindowSize(this->window, width, height);
}

void Window::setTitle(const char* title){
    SDL_SetWindowTitle(this->window, title);
}

// This returns false in case of:
//  1. Non resizable window (duh)
//  2. Android device, fullscreen
//  3. Raspberry Pi in video framebuffer mode
bool Window::isResizable(){
    if(SDL_GetWindowFlags(this->window) & SDL_WINDOW_RESIZABLE){
        return true;
    }

    if(strcmp(SDL_GetPlatform(), "Android") == 0){
        return false;
    }

    return false;
}


void Window::getPosition(int* x, int* y){
    SDL_GetWindowPosition(this->window, x, y);
}

void Window::setPosition(int x, int y){
    SDL_SetWindowPosition(this->window, x, y);
}

int Window::getWidth(){
    int width;
    SDL_GetWindowSize(this->window, &width, NULL);
    return width;
}

int Window::getHeight(){
    int height;
    SDL_GetWindowSize(this->window, NULL, &height);
    return height;
}

void Window::window_handler_event(void* data){
    // Process WINDOW type events
    
}

// SDL OpenGL Methods for the renderer (internal use only)
void Window::GL_MakeCurrent(SDL_GLContext context){
    SDL_GL_MakeCurrent(this->window, context);
}

int Window::GL_SetAttribute(SDL_GLattr attr, int value){
    return SDL_GL_SetAttribute(attr, value);
}

int Window::GL_SetSwapInterval(int interval){
    return SDL_GL_SetSwapInterval(interval);
}

bool Window::GL_ExtensionSupported(const char* extension){
    return (SDL_GL_ExtensionSupported(extension) == SDL_TRUE) ? true : false;
}

void* Window::GL_GetProcAddress(const char* proc){
    return SDL_GL_GetProcAddress(proc);
}

SDL_GLContext Window::GL_CreateContext(){
    return SDL_GL_CreateContext(this->window);
}

void Window::GL_DeleteContext(SDL_GLContext context){
    SDL_GL_DeleteContext(context);
}

void Window::GL_SwapWindow(){
    SDL_GL_SwapWindow(this->window);
}


