/**
 * @file image2raw.cpp
 * @author Brais Solla González
 * @brief Image2Raw Tool for Enyx
 * @version 0.1
 * @date 2021-11-19
 * 
 * @copyright Copyright (c) 2021
 * 
 * This tool converts image to raw bitstreams
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s image.png > image.raw\n", argv[0]);
        return -1;
    }

    int x,y,n;
    int ok = stbi_info(argv[1], &x, &y, &n);
    if(ok){
        fprintf(stderr, "Image: %s\n", argv[1]);
        fprintf(stderr, "Resolution: %dx%d, Channels: %d\n", x, y, n);
    } else {
        fprintf(stderr, "Error: Image %s cannot be opened or is not valid.\n", argv[1]);
        return -2;
    }

    unsigned char *data = stbi_load(argv[1], &x, &y, &n, 0);
    if(data){
        size_t length = x * y * n;
        fprintf(stderr,"Writing image %s (%dx%dx%d) (%d bytes)\n", argv[1], x, y, n, (int) length);
        fwrite(data, 1, length, stdout);
        fflush(stdout);
        fprintf(stderr, "Image write completed!\n");
    } else {
        fprintf(stderr, "Error: Image %s cannot be decoded\n", argv[1]);
        return -3;
    }


    stbi_image_free(data);
    return 0;
}

