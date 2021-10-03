/**
 * @file ImageDriver.cpp
 * @author Brais Solla González
 * @brief Enyx GL Image driver for STB
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ImageDriver.h"

// Enable on NEON (ARM) build
// #define STBI_NEON

#define STBI_NO_SIMD // Enabled for debugging for now
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

static void _id_progress_report(float progress){
    if(callback_fnc){
        callback_fnc(progress);
    }
}

#define STBIR_PROGRESS_REPORT(val) _id_progress_report(val)
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"


int ID::infoImage(const char* fileName, int* sx, int* sy, int* n){
    return stbi_info(fileName, sx, sy, n);
}

uint8_t* ID::loadImage(const char* fileName, int* sx, int* sy, int* n){
    fprintf(stderr, "[%s:%d]: ID::loadImage: Loading image %s...\n", __FILE__, __LINE__, fileName);
    return (uint8_t*) stbi_load(fileName, sx, sy, n, 0);
}

float* ID::loadImageF(const char* fileName, int* sx, int* sy, int* n){
    fprintf(stderr, "[%s:%d]: ID::loadImageF: Loading image %s...\n", __FILE__, __LINE__, fileName);
    return (float*) stbi_loadf(fileName, sx, sy, n, 0);
}

void ID::freeImage(void* image_ptr){
    fprintf(stderr, "[%s:%d]: ID::freeImage: Freeing image pointer %p...\n", __FILE__, __LINE__, image_ptr);
    stbi_image_free(image_ptr);
}

int ID::writeImage(const char* fileName, int w, int h, int n, void* data){
    // Find last '.' for extension
    int lenptr = strlen(fileName);
    while(lenptr > 0){
        if(fileName[lenptr] == '.') break;
        lenptr--;
    }

    if(lenptr != 0){
        const char* extension = fileName + lenptr;
        
        if(strstr(extension, "jpg") || strstr(extension,"jpeg")){
            fprintf(stderr, "[%s:%d]: ID::writeImage: Writing JPG image %s...\n", __FILE__, __LINE__, fileName);
            stbi_write_jpg(fileName, w, h, n, data, 100);
            return 0;
        } else if(strstr(extension, "tga")){
            fprintf(stderr, "[%s:%d]: ID::writeImage: Writing TGA image %s...\n", __FILE__, __LINE__, fileName);
            stbi_write_tga(fileName, w, h, n, data);
            return 0;
        } else if(strstr(extension, "bmp")){
            fprintf(stderr, "[%s:%d]: ID::writeImage: Writing BMP image %s...\n", __FILE__, __LINE__, fileName);
            stbi_write_bmp(fileName, w, h, n, data);
            return 0;
        } else if(strstr(extension,"png")){
            fprintf(stderr, "[%s:%d]: ID::writeImage: Writing PNG image %s...\n", __FILE__, __LINE__, fileName);
            stbi_write_png(fileName, w, h, n, data, 0);
            return 0;
        } else {
            fprintf(stderr, "[%s:%d]: ERROR ID::writeImage: Format not supported\n", __FILE__, __LINE__);
            return -2;
        }
    } else {
        fprintf(stderr, "[%s:%d]: ERROR ID::writeImage: Cannot determine extension\n", __FILE__, __LINE__);
        return 1;
    }

    return 0;
}

int ID::writeImage(const char* fileName, int w, int h, int n, void* data, int q){
    fprintf(stderr, "[%s:%d]: ID::writeImage: Writing JPG image %s with quality %d...\n", __FILE__, __LINE__, fileName, q);
    stbi_write_jpg(fileName, w, h, n, data, q);
    return 0;
}

void ID::resizingCallback(progress_callback_t callback){
    callback_fnc = callback;
}

int ID::resizeImage(void* input, int in_w, int in_h, void* output, int out_w, int out_h, int n){
    fprintf(stderr, "[%s:%d]: ID::resizeImage: Resizing image (%dx%d)->(%dx%d)...\n", __FILE__, __LINE__, in_w, in_h, out_w, out_h);
    stbir_resize_uint8((uint8_t*) input, in_w, in_h, 0, (uint8_t*) output, out_w, out_h, 0, n);
    return 0;
}