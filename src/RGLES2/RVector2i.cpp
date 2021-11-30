/**
 * @file RVector2i.cpp
 * @author Brais Solla González
 * @brief RVector2i implementation
 * @version 0.1
 * @date 2021-11-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "RGLES2/RVector2i.h"

#ifndef sq
#define sq(x) (((x)*(x)))
#endif

// RVector2i implementation
RVector2i::RVector2i(){
    this->x = 0;
    this->y = 0;
}

RVector2i::RVector2i(int x, int y){
    this->x = x;
    this->y = y;
}

RVector2i::RVector2i(const RVector2i& other){
    this->x = other.x;
    this->y = other.y;
}

RVector2i RVector2i::operator+(const RVector2i& other) const {
    RVector2i ret = RVector2i(this->x + other.x, this->y + other.y);
    return ret;
}

RVector2i RVector2i::operator-(const RVector2i& other) const {
    RVector2i ret = RVector2i(this->x - other.x, this->y - other.y);
    return ret;
}

RVector2i RVector2i::operator*(float scalar) const {
    RVector2i ret = RVector2i(this->x * scalar, this->y * scalar);
    return ret;
}

RVector2i& RVector2i::operator+=(const RVector2i& other){
    this->x += other.x;
    this->y += other.y;

    return *this;
}

RVector2i& RVector2i::operator-=(const RVector2i& other){
    this->x -= other.x;
    this->y -= other.y;
    
    return *this;
}

RVector2i& RVector2i::operator=(const RVector2i& other){
    this->x = other.x;
    this->y = other.y;

    return *this;
}

RVector2i& RVector2i::operator*=(float scalar){
    this->x *= scalar;
    this->y *= scalar;
    
    return *this;
}

RVector2i& RVector2i::normalize(){
    float inv_sq = 1.f / this->length();

    this->x *= inv_sq;
    this->y *= inv_sq;

    return *this;
}

RVector2i RVector2i::normalized() const {
    float inv_sq = 1.f / this->length();

    RVector2i ret = RVector2i(this->x * inv_sq, this->y * inv_sq);
    return ret;
}

float RVector2i::length() const {
    return sqrt(sq(this->x) + sq(this->y));
}

float RVector2i::sqLength() const {
    return (float) (sq(this->x) + sq(this->y));
}

float RVector2i::angle() const {
    return (float) atan2((float) this->y, (float) this->x);
}

float RVector2i::dot(const RVector2i& v1, const RVector2i& v2){
    return (v1.x * v2.x + v1.y * v2.y);
}

RVector2i RVector2i::lerp(const RVector2i& v1, const RVector2i& v2, float n){
    return v1 + (v2 - v1) * n;
}