/**
 * @file enyx.h
 * @author Brais Solla González
 * @brief Enyx Graphics Library
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "AGL.h"
#include "Pixmap.h"
#include "ImageDriver.h"
#include "Platform_SDL2.h"
// #include "Renderer_SDL2_GLES2.h"

#ifdef GL1_BUILD
#include "RGL1/RGL1.h"
#else
#include "RGLES2/RGLES2.h"
#endif