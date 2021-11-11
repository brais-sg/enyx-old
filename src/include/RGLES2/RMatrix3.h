/**
 * @file RMatrix3.h
 * @author Brais Solla González
 * @brief Enyx RGLES2 RMatrix4 object
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RMATRIX3_INCLUDED
#define _ENYX_RGLES2_RMATRIX3_INCLUDED

class RMatrix3 {
    public:
        float e[9];

        RMatrix3();
        RMatrix3(const RMatrix3& other);

        RMatrix3 operator+(const RMatrix3& other) const;
        RMatrix3 operator-(const RMatrix3& other) const;
        RMatrix3 operator*(const RMatrix3& other) const;
        RMatrix3 operator*(float scalar) const;

        RMatrix3& operator+=(const RMatrix3& other);
        RMatrix3& operator-=(const RMatrix3& other);
        RMatrix3& operator*=(const RMatrix3& other);
        RMatrix3& operator=(const RMatrix3& other);
        RMatrix3& operator*=(float scalar);


        // Methods
        float* getArray();
        void   loadIdentity();






};

#endif