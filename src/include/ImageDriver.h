/**
 * @file ImageDriver.h
 * @author Brais Solla González
 * @brief ImageDriver for Enyx GL
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _IMAGEDRIVER_INCLUDED
#define _IMAGEDRIVER_INCLUDED

#include <stdlib.h>
#include <stdint.h>

typedef void (*progress_callback_t)(float);
namespace ImageDriver {
    static progress_callback_t callback_fnc = NULL;

    int      infoImage (const char* fileName, int* sx, int* sy, int* n);
    uint8_t* loadImage (const char* fileName, int* sx, int* sy, int* n);
    float*   loadImageF(const char* fileName, int* sx, int* sy, int* n);
    // TODO: Load from memory

    void     freeImage(void* image_ptr);

    // STBI_write
    int      writeImage(const char* fileName, int w, int h, int n, void* data);
    // JPG ONLY!
    int      writeImage(const char* fileName, int w, int h, int n, void* data, int q);

    void     resizingCallback(progress_callback_t callback);
    int      resizeImage(void* input, int in_w, int in_h, void* output, int out_w, int out_h, int n);
};





#endif