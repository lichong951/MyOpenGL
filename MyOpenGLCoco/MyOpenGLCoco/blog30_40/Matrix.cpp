//
//  Matrix.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/10/6.
//  Copyright © 2017年 lichong. All rights reserved.
//

#include "Matrix.hpp"

CMatrix4x4::CMatrix4x4()
{
    ///初始化各元素
    Clear();
}


CMatrix4x4::CMatrix4x4(const CMatrix4x4 &m)
{
    matrix[0]  = m.matrix[0];
    matrix[4]  = m.matrix[4];
    matrix[8]  = m.matrix[8];
    matrix[12] = m.matrix[12];
    
    matrix[1]  = m.matrix[1];
    matrix[5]  = m.matrix[5];
    matrix[9]  = m.matrix[9];
    matrix[13] = m.matrix[13];
    
    matrix[2]  = m.matrix[2];
    matrix[6]  = m.matrix[6];
    matrix[10] = m.matrix[10];
    matrix[14] = m.matrix[14];
    
    matrix[3]  = m.matrix[3];
    matrix[7]  = m.matrix[7];
    matrix[11] = m.matrix[11];
    matrix[15] = m.matrix[15];
}


CMatrix4x4::CMatrix4x4(float r11, float r12, float r13, float r14,
                       float r21, float r22, float r23, float r24,
                       float r31, float r32, float r33, float r34,
                       float r41, float r42, float r43, float r44)
{
    
    matrix[0]  = r11; matrix[1]  = r12; matrix[2]  = r13; matrix[3]  = r14;
    matrix[4]  = r21; matrix[5]  = r22; matrix[6]  = r23; matrix[7]  = r24;
    matrix[8]  = r31; matrix[9]  = r32; matrix[10] = r33; matrix[11] = r34;
    matrix[12] = r41; matrix[13] = r42; matrix[14] = r43; matrix[15] = r44;
}


CMatrix4x4::~CMatrix4x4()
{
    
}


void CMatrix4x4::operator =(CMatrix4x4 m)
{
    
    matrix[0]  = m.matrix[0]; matrix[1]   = m.matrix[1]; matrix[2]   = m.matrix[2];
    matrix[3]  = m.matrix[3];
    matrix[4]  = m.matrix[4]; matrix[5]   = m.matrix[5]; matrix[6]   = m.matrix[6];
    matrix[7]  = m.matrix[7];
    matrix[8]  = m.matrix[8]; matrix[9]   = m.matrix[9]; matrix[10]  = m.matrix[10];
    matrix[11] = m.matrix[11];
    matrix[12] = m.matrix[12]; matrix[13] = m.matrix[13]; matrix[14] = m.matrix[14];
    matrix[15] = m.matrix[15];
}

/// 矩阵相减
CMatrix4x4 CMatrix4x4::operator -(CMatrix4x4 m)
{
    
    return CMatrix4x4(matrix[0] - m.matrix[0], matrix[1] - m.matrix[1], matrix[2] - m.matrix[2],
                      matrix[3] - m.matrix[3], matrix[4] - m.matrix[4], matrix[5] - m.matrix[5],
                      matrix[6] - m.matrix[6], matrix[7] - m.matrix[7], matrix[8] - m.matrix[8],
                      matrix[9] - m.matrix[9], matrix[10] - m.matrix[10], matrix[11] - m.matrix[11],
                      matrix[12] - m.matrix[12], matrix[13] - m.matrix[13],
                      matrix[14] - m.matrix[14], matrix[15] - m.matrix[15]);
}

/// 矩阵相加
CMatrix4x4 CMatrix4x4::operator +(CMatrix4x4 m)
{
    
    return CMatrix4x4(matrix[0] + m.matrix[0], matrix[1] + m.matrix[1], matrix[2] + m.matrix[2],
                      matrix[3] + m.matrix[3], matrix[4] + m.matrix[4], matrix[5] + m.matrix[5],
                      matrix[6] + m.matrix[6], matrix[7] + m.matrix[7], matrix[8] + m.matrix[8],
                      matrix[9] + m.matrix[9], matrix[10] + m.matrix[10], matrix[11] + m.matrix[11],
                      matrix[12] + m.matrix[12], matrix[13] + m.matrix[13],
                      matrix[14] + m.matrix[14], matrix[15] + m.matrix[15]);
}

/// 矩阵相乘
CMatrix4x4 CMatrix4x4::operator *(CMatrix4x4 m)
{
    
    float newMatrix[16];
    const float *m1 = matrix, *m2 = m.matrix;
    
    newMatrix[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2];
    newMatrix[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2];
    newMatrix[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2];
    newMatrix[3] = 0;
    
    newMatrix[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6];
    newMatrix[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6];
    newMatrix[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6];
    newMatrix[7] = 0;
    
    newMatrix[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10];
    newMatrix[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10];
    newMatrix[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10];
    newMatrix[11] = 0;
    
    newMatrix[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12];
    newMatrix[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13];
    newMatrix[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14];
    newMatrix[15] = 1;
    
    return CMatrix4x4(newMatrix[0], newMatrix[1], newMatrix[2], newMatrix[3], newMatrix[4],
                      newMatrix[5], newMatrix[6], newMatrix[7], newMatrix[8], newMatrix[9],
                      newMatrix[10], newMatrix[11], newMatrix[12], newMatrix[13], newMatrix[14],
                      newMatrix[15]);
}

/// 矩阵相除
CMatrix4x4 CMatrix4x4::operator /(CMatrix4x4 m)
{
    return CMatrix4x4(matrix[0] / m.matrix[0] + matrix[4] / m.matrix[1] + matrix[8] /
                      m.matrix[2] + matrix[12] / m.matrix[3],
                      matrix[1] / m.matrix[0] + matrix[5] / m.matrix[1] + matrix[9] /
                      m.matrix[2] + matrix[13] / m.matrix[3],
                      matrix[2] / m.matrix[0] + matrix[6] / m.matrix[1] + matrix[10] /
                      m.matrix[2] + matrix[14] / m.matrix[3],
                      matrix[3] / m.matrix[0] + matrix[7] / m.matrix[1] + matrix[11] /
                      m.matrix[2] + matrix[15] / m.matrix[3],
                      matrix[0] / m.matrix[4] + matrix[4] / m.matrix[5] + matrix[8] /
                      m.matrix[6] + matrix[12] / m.matrix[7],
                      matrix[1] / m.matrix[4] + matrix[5] / m.matrix[5] + matrix[9] /
                      m.matrix[6] + matrix[13] / m.matrix[7],
                      matrix[2] / m.matrix[4] + matrix[6] / m.matrix[5] + matrix[10] /
                      m.matrix[6] + matrix[14] / m.matrix[7],
                      matrix[3] / m.matrix[4] + matrix[7] / m.matrix[5] + matrix[11] /
                      m.matrix[6] + matrix[15] / m.matrix[7],
                      matrix[0] / m.matrix[8] + matrix[4] / m.matrix[9] + matrix[8] /
                      m.matrix[10] + matrix[12] / m.matrix[11],
                      matrix[1] / m.matrix[8] + matrix[5] / m.matrix[9] + matrix[9] /
                      m.matrix[10] + matrix[13] / m.matrix[11],
                      matrix[2] / m.matrix[8] + matrix[6] / m.matrix[9] + matrix[10] /
                      m.matrix[10] + matrix[14] / m.matrix[11],
                      matrix[3] / m.matrix[8] + matrix[7] / m.matrix[9] + matrix[11] /
                      m.matrix[10] + matrix[15] / m.matrix[11],
                      matrix[0] / m.matrix[12] + matrix[4] / m.matrix[13] + matrix[8] /
                      m.matrix[14] + matrix[12] / m.matrix[15],
                      matrix[1] / m.matrix[12] + matrix[5] / m.matrix[13] + matrix[9] /
                      m.matrix[14] + matrix[13] / m.matrix[15],
                      matrix[2] / m.matrix[12] + matrix[6] / m.matrix[13] + matrix[10] /
                      m.matrix[14] + matrix[14] / m.matrix[15],
                      matrix[3] / m.matrix[12] + matrix[7] / m.matrix[13] + matrix[11] /
                      m.matrix[14] + matrix[15] / m.matrix[15]);
}


CMatrix4x4 CMatrix4x4::operator -(float f)
{
    return CMatrix4x4(matrix[0]  - f, matrix[1]  - f, matrix[2]  - f, matrix[3]  - f,
                      matrix[4]  - f, matrix[5]  - f, matrix[6]  - f, matrix[7]  - f,
                      matrix[8]  - f, matrix[9]  - f, matrix[10] - f, matrix[11] - f,
                      matrix[12] - f, matrix[13] - f, matrix[14] - f, matrix[15] - f);
}


CMatrix4x4 CMatrix4x4::operator +(float f)
{
    return CMatrix4x4(matrix[0]  + f, matrix[1]  + f, matrix[2]  + f, matrix[3]  + f,
                      matrix[4]  + f, matrix[5]  + f, matrix[6]  + f, matrix[7]  + f,
                      matrix[8]  + f, matrix[9]  + f, matrix[10] + f, matrix[11] + f,
                      matrix[12] + f, matrix[13] + f, matrix[14] + f, matrix[15] + f);
}


CMatrix4x4 CMatrix4x4::operator *(float f)
{
    return CMatrix4x4(matrix[0]  * f, matrix[1]  * f, matrix[2]  * f, matrix[3]  * f,
                      matrix[4]  * f, matrix[5]  * f, matrix[6]  * f, matrix[7]  * f,
                      matrix[8]  * f, matrix[9]  * f, matrix[10] * f, matrix[11] * f,
                      matrix[12] * f, matrix[13] * f, matrix[14] * f, matrix[15] * f);
}


CMatrix4x4 CMatrix4x4::operator /(float f)
{
    
    if(f == 0) f = 1;
    f = 1/f;
    
    return CMatrix4x4(matrix[0]  * f, matrix[1]  * f, matrix[2]  * f, matrix[3]  * f,
                      matrix[4]  * f, matrix[5]  * f, matrix[6]  * f, matrix[7]  * f,
                      matrix[8]  * f, matrix[9]  * f, matrix[10] * f, matrix[11] * f,
                      matrix[12] * f, matrix[13] * f, matrix[14] * f, matrix[15] * f);
}


bool CMatrix4x4::operator ==(CMatrix4x4 m)
{
    
    for(int i = 0; i < 16; i++)
    {
        if(matrix[i] != m.matrix[i])
            return false;
    }
    
    return true;
}


void CMatrix4x4::operator +=(CMatrix4x4 m)
{
    (*this) = (*this) + m;
}


void CMatrix4x4::operator -=(CMatrix4x4 m)
{
    (*this) = (*this) - m;
}


void CMatrix4x4::operator *=(CMatrix4x4 m)
{
    (*this) = (*this) * m;
}


void CMatrix4x4::operator /=(CMatrix4x4 m)
{
    (*this) = (*this) / m;
}


bool CMatrix4x4::operator !=(CMatrix4x4 m)
{
    return !((*this) == m);
}


void CMatrix4x4::operator -=(float f)
{
    (*this) = (*this) - f;
}


void CMatrix4x4::operator +=(float f)
{
    (*this) = (*this) + f;
}


void CMatrix4x4::operator *=(float f)
{
    (*this) = (*this) * f;
}


void CMatrix4x4::operator /=(float f)
{
    (*this) = (*this) / f;
}


void CMatrix4x4::Clear()
{
    ///重置矩阵为单位矩阵
    matrix[0]  = 1.0f; matrix[1]  = 0.0f; matrix[2]  = 0.0f; matrix[3]  = 0.0f;
    matrix[4]  = 0.0f; matrix[5]  = 1.0f; matrix[6]  = 0.0f; matrix[7]  = 0.0f;
    matrix[8]  = 0.0f; matrix[9]  = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
    
}


void CMatrix4x4::Zero()
{
    ///重置矩阵各元素为0
    matrix[0]  = 0.0f; matrix[1]  = 0.0f; matrix[2]  = 0.0f; matrix[3]  = 0.0f;
    matrix[4]  = 0.0f; matrix[5]  = 0.0f; matrix[6]  = 0.0f; matrix[7]  = 0.0f;
    matrix[8]  = 0.0f; matrix[9]  = 0.0f; matrix[10] = 0.0f; matrix[11] = 0.0f;
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 0.0f;
}

///平移变换
void CMatrix4x4::Translate(CVector4 Translate)
{
    
    matrix[12] = Translate.x;
    matrix[13] = Translate.y;
    matrix[14] = Translate.z;
    matrix[15] = 1.0f;
}

void CMatrix4x4::Translate(float x, float y, float z)
{
    
    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
    matrix[15] = 1.0f;
}

void CMatrix4x4::inverseTranslate()
{
    matrix[12] = -matrix[12];
    matrix[13] = -matrix[13];
    matrix[14] = -matrix[14];
}

/// 旋转变换
void CMatrix4x4::Rotate(double angle, float x, float y, float z)
{
    float sine = (float)sin(angle);
    float cosine = (float)cos(angle);
    
    float sinAngle = (float)sin(3.14 * angle / 180);
    float cosAngle = (float)cos(3.14 * angle / 180);
    float oneSubCos = 1.0f - cosAngle;
    
    matrix[0] = (x * x) * oneSubCos + cosAngle;
    matrix[4] = (x * y) * oneSubCos - (z * sinAngle);
    matrix[8] = (x * z) * oneSubCos + (y * sinAngle);
    
    matrix[1] = (y * x) * oneSubCos + (sinAngle * z);
    matrix[5] = (y * y) * oneSubCos + cosAngle;
    matrix[9] = (y * z) * oneSubCos - (x * sinAngle);
    
    matrix[2] = (z * x) * oneSubCos - (y * sinAngle);
    matrix[6] = (z * y) * oneSubCos + (x * sinAngle);
    matrix[10] = (z * z) * oneSubCos + cosAngle;
}

///向量与矩阵相乘
CVector4 CMatrix4x4::VectorMatrixMultiply(CVector4 v)
{
    CVector4 out;
    
    out.x = (v.x * matrix[0]) + (v.y * matrix[4]) + (v.z * matrix[8]) + matrix[12];
    out.y = (v.x * matrix[1]) + (v.y * matrix[5]) + (v.z * matrix[9]) + matrix[13];
    out.z = (v.x * matrix[2]) + (v.y * matrix[6]) + (v.z * matrix[10]) + matrix[14];
    
    return out;
}


CVector4 CMatrix4x4::VectorMatrixMultiply3x3(CVector4 v)
{
    CVector4 out;
    
    out.x = (v.x * matrix[0]) + (v.y * matrix[4]) + (v.z * matrix[8]);
    out.y = (v.x * matrix[1]) + (v.y * matrix[5]) + (v.z * matrix[9]);
    out.z = (v.x * matrix[2]) + (v.y * matrix[6]) + (v.z * matrix[10]);
    
    return out;
}
