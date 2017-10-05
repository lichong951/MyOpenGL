//
//  CSkyBox.hpp
//  blog_cxx_38
//
//  Created by lichong on 2017/10/6.
//  Copyright © 2017年 lichong. All rights reserved.
//

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "Vector.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE    0x812F

/** 天空盒类 */
class CSkyBox
{
public:
    /** 构造函数 */
    CSkyBox();
    ~CSkyBox();
    
    /** 初始化 */
    bool Init();
    
    /** 渲染天空 */
    void  CreateSkyBox(float x, float y, float z,
                       float width, float height,
                       float length);
    
private:
    
    CBMPLoader  m_texture[6];   /**< 天空盒纹理 */
    
};


#endif ///__SKYBOX_H__
