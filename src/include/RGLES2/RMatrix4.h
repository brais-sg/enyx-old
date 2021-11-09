/**
 * @file RMatrix4.h
 * @author Brais Solla González
 * @brief Enyx RGLES2 RMatrix4 object
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RMATRIX4_INCLUDED
#define _ENYX_RGLES2_RMATRIX4_INCLUDED

class RMatrix4 {
    public:
        float e[16];

        RMatrix4();
        RMatrix4(const RMatrix4& other);

        RMatrix4 operator+(const RMatrix4& other) const;
        RMatrix4 operator-(const RMatrix4& other) const;
        RMatrix4 operator*(const RMatrix4& other) const;
        RMatrix4 operator*(float scalar) const;

        RMatrix4& operator+=(const RMatrix4& other);
        RMatrix4& operator-=(const RMatrix4& other);
        RMatrix4& operator*=(const RMatrix4& other);
        RMatrix4& operator=(const RMatrix4& other);
        RMatrix4& operator*=(float scalar);

        // Methods

        float* getArray();
        void   loadIdentity();

        static RMatrix4 translation(float tx, float ty);

        // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)
        static RMatrix4 translation(float tx, float ty, float tz);
        static RMatrix4 rotation(float angle);
        static RMatrix4 scaling(float sx, float sy);

        // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)
        static RMatrix4 scaling(float sx, float sy, float sz); 

        static RMatrix4 ortho(float left, float right, float bottom, float top, float znear, float zfar);

        // This is not going to be a 3D engine, right?
        static RMatrix4 frustum(float left, float right, float bottom, float top, float znear, float zfar);
};

#endif