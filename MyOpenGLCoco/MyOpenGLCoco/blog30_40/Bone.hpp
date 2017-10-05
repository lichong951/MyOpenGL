//
//  Bone.hpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/10/6.
//  Copyright © 2017年 lichong. All rights reserved.
//

#ifndef __opengl__Bone__
#define __opengl__Bone__

#include <stdio.h>
#include "Matrix.hpp"

/** 骨骼类 */
class CBone
{
public:
    CBone();
    
    /** 设置骨骼 */
    void SetBone(unsigned int P, float L);
    void SetBone(unsigned int P, float L, CMatrix4x4 R, CMatrix4x4 A);
    
    unsigned int parent;  /**< 父结点 */
    float length;         /**< 长度 */
    
    /** 变换矩阵 */
    CMatrix4x4 relative;
    CMatrix4x4 absolute;
};

/** 骨骼顶点类，即绑定到骨骼上的皮肤顶点数据 */
class CBoneVertex
{
public:
    CBoneVertex();
    
    /** 设置顶点数据 */
    void SetVertex(CVector4 p, CVector4 n, int b1, int b2, int b3, int b4,
                   float w1, float w2, float w3, float w4, float R, float G,
                   float B, float A, int numB);
    
    
    CVector4 pos;         /**< 位置 */
    CVector4 normal;      /**< 法线 */
    
    int boneIndex[4];     /**< 骨骼索引 */
    float weights[4];     /**< 权值 */
    int numBones;         /**< 影响的骨骼数目 */
    
    float r, g, b, a;     /**< 颜色 */
};

#endif /* defined(__opengl__Bone__) */
