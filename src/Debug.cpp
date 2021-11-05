/**
 * @file Debug.cpp
 * @author Brais Solla González
 * @brief Debug print for Enyx
 * @version 0.1
 * @date 2021-10-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Debug.h"

void Debug::info(const char* format, ...){
    va_list args;
    fprintf(stdout, COLOR_BLUE "[INFO]: " COLOR_RESET);
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

void Debug::warning(const char* format, ...){
    va_list args;
    fprintf(stderr, COLOR_YELLOW "[WARN]: " COLOR_RESET);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void Debug::error(const char* format, ...){
    va_list args;
    fprintf(stderr, COLOR_RED "[ERROR]: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr,"%s",COLOR_RESET);
}

void Debug::critical(const char* format, ...){
    va_list args;
    fprintf(stderr, COLOR_RED "[CRITICAL]: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr,"%s",COLOR_RESET);
    exit(-1);
}

