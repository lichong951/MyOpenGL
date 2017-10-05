//
//  Matrix.hpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/10/6.
//  Copyright © 2017年 lichong. All rights reserved.
//


#ifndef __opengl__Matrix__
#define __opengl__Matrix__

#include <stdio.h>
#include "Vector.hpp"

/** 4x4矩阵类 */
class  CMatrix4x4
{
public:
    ///构造函数
    CMatrix4x4();
    CMatrix4x4(const CMatrix4x4 &m);
    CMatrix4x4(float r11, float r12, float r13, float r14,
               float r21, float r22, float r23, float r24,
               float r31, float r32, float r33, float r34,
               float r41, float r42, float r43, float r44);
    ~CMatrix4x4();
    
    ///重置矩阵
    void Clear();
    void Zero();
    
    ///运算符重载
    void operator=(CMatrix4x4 m);
    CMatrix4x4 operator-(CMatrix4x4 m);
    CMatrix4x4 operator+(CMatrix4x4 m);
    CMatrix4x4 operator*(CMatrix4x4 m);
    CMatrix4x4 operator/(CMatrix4x4 m);
    
    void operator +=(CMatrix4x4 m);
    void operator -=(CMatrix4x4 m);
    void operator *=(CMatrix4x4 m);
    void operator /=(CMatrix4x4 m);
    
    CMatrix4x4 operator-(float f);
    CMatrix4x4 operator+(float f);
    CMatrix4x4 operator*(float f);
    CMatrix4x4 operator/(float f);
    
    void operator -=(float f);
    void operator +=(float f);
    void operator *=(float f);
    void operator /=(float f);
    
    bool operator ==(CMatrix4x4 v);
    bool operator !=(CMatrix4x4 v);
    
    ///平移一个点
    void Translate(CVector4 Translate);
    void Translate(float x, float y, float z);
    void inverseTranslate();
    
    ///旋转一个点
    void Rotate(double angle, float x, float y,
                float z);
    
    ///向量与矩阵相乘
    CVector4 VectorMatrixMultiply(CVector4 v);
    CVector4 VectorMatrixMultiply3x3(CVector4 v);
    
    
public:
    float matrix[16];
};

#endif /* defined(__opengl__Matrix__) */
