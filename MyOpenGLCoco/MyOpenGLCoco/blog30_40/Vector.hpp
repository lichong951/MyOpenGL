//
//  Vector.hpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/10/6.
//  Copyright © 2017年 lichong. All rights reserved.
//

#ifndef __opengl__Vector__
#define __opengl__Vector__

#include <stdio.h>
#include <math.h>
#define M_PI 3.141592654

/** 四维向量类 */
class CVector4
{
public:
    ///构造函数
    CVector4();
    CVector4(float X, float Y, float Z);
    CVector4(float X, float Y, float Z, float W);
    CVector4(const CVector4 &v);
    
    ///运算符重载
    void operator=(CVector4 v);
    CVector4 operator-(CVector4 v);
    CVector4 operator+(CVector4 v);
    CVector4 operator*(CVector4 v);
    CVector4 operator/(CVector4 v);
    CVector4 operator+(float f);
    CVector4 operator-(float f);
    CVector4 operator*(float f);
    CVector4 operator/(float f);
    
    void operator +=(CVector4 v);
    void operator -=(CVector4 v);
    void operator *=(CVector4 v);
    void operator /=(CVector4 v);
    void operator +=(float f);
    void operator -=(float f);
    void operator *=(float f);
    void operator /=(float f);
    
    bool operator ==(CVector4 v);
    bool operator !=(CVector4 v);
    
    ///叉乘
    void CrossProduct(CVector4 v1, CVector4 v2);
    void CrossProduct3(CVector4 v1, CVector4 v2,
                       CVector4 v3);
    
    ///点乘
    float DotProduct3(CVector4 v1);
    float DotProduct4(CVector4 v1);
    
    ///返回向量长度
    float GetLength();
    
    ///归一化向量
    void Normal();
    
public:
    float x, y, z, w;
};



#endif /* defined(__opengl__Vector__) */
