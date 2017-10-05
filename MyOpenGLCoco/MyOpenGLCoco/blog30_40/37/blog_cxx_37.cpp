//
//  blog_cxx_37.cpp
//  blog_cxx_37
//
//  Created by lichong on 2017/10/6.
//  Copyright © 2017年 lichong. All rights reserved.
//

#include <stdio.h>
#include "stdafx.h"
#include<gl/glut.h>
#include<gl/glu.h>
#include<gl/gl.h>
#include <gl\GLAUX.h>

#include "Camera.hpp"
#include "CBMPLoader.hpp"

/** 定义光源的属性值 */
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };    /**< 环境光参数 */
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    /**< 漫射光参数 */
GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   /**< 镜面光参数 */
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };   /**< 光源位置 */

Camera m_Camera;
CBMPLoader m_Texture;

/** 加载位图纹理(木箱贴图) */
bool LoadTexture()
{
    if (!m_Texture.LoadBitmap("image.bmp"))                                         /**< 载入位图文件 */
    {
        MessageBox(NULL, (LPCWSTR)"loadTexture error", (LPCWSTR)"error", MB_OK);    /**< 如果载入失败则弹出对话框 */
        return false;
    }
    
    glGenTextures(1, &m_Texture.ID);                                                /**< 生成一个纹理对象名称 */
    glBindTexture(GL_TEXTURE_2D, m_Texture.ID);                                     /**< 创建纹理对象 */
    
    /** 控制滤波 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    /** 创建纹理 */
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_Texture.imageWidth,
                      m_Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
                      m_Texture.image);
    
    return true;
}

/** 绘制网格地面 */
void DrawGrid()
{
    /** 获得场景中一些状态  */
    GLboolean  lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);
    
    /** 关闭纹理和光照 */
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    /** 绘制过程 */
    glPushAttrib(GL_CURRENT_BIT);   /**< 保存当前属性 */
    glPushMatrix();                 /**< 压入堆栈 */
    glTranslatef(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);    /**< 设置颜色 */
    
    /** 在X,Z平面上绘制网格 */
    for (float i = -50; i <= 50; i += 1)
    {
        /** 绘制线 */
        glBegin(GL_LINES);
        
        /** X轴方向 */
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
        
        /** Z轴方向 */
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);
        
        glEnd();
    }
    glPopMatrix();
    glPopAttrib();
    
    /** 恢复场景状态 */
    if (tp)
        glEnable(GL_TEXTURE_2D);
    if (lp)
        glEnable(GL_LIGHTING);
}

/** 绘制球体 */
void DrawSphere()
{
    /** 设置材质属性 */
    GLfloat mat_ambient[] = { 0.9f, 0.5f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.9f, 0.5f, 0.8f, 1.0f };
    GLfloat mat_shininess[] = { 100.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    /** 获得纹理启用状态 */
    GLboolean tp;
    glGetBooleanv(GL_TEXTURE_2D, &tp);
    glDisable(GL_TEXTURE_2D);                   /**< 关闭纹理 */
    
    /** 绘制过程 */
    glPushMatrix();
    glTranslatef(-5.0f, 2.0f, -10.0f);
    GLUquadricObj * sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, 2.0, 50, 50);
    gluDeleteQuadric(sphere);
    glPopMatrix();
    
    /** 恢复状态 */
    if (tp)
        glEnable(GL_TEXTURE_2D);
}

/** 绘制木箱 */
void DrawBox()
{
    /** 设置材质属性 */
    GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    glPushMatrix();
    glTranslatef(5.0f, 2.0f, -10.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    /** 选择纹理 */
    glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
    /** 开始绘制四边形 */
    glBegin(GL_QUADS);
    /// 前侧面
    glNormal3f(0.0f, 0.0f, 1.0f);                               /**< 指定法线指向观察者 */
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    /// 后侧面
    glNormal3f(0.0f, 0.0f, -1.0f);                              /**< 指定法线背向观察者 */
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    /// 顶面
    glNormal3f(0.0f, 1.0f, 0.0f);                               /**< 指定法线向上 */
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    /// 底面
    glNormal3f(0.0f, -1.0f, 0.0f);                              /**< 指定法线朝下 */
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    /// 右侧面
    glNormal3f(1.0f, 0.0f, 0.0f);                               /**< 指定法线朝右 */
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    /// 左侧面
    glNormal3f(-1.0f, 0.0f, 0.0f);                              /**< 指定法线朝左 */
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}

void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    /** 设置光源的属性值 */
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     /**< 设置环境光 */
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     /**< 设置漫射光 */
    glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);   /**< 设置漫射光 */
    
    /** 启用光源和纹理 */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_TEXTURE_2D);
    
    /** 载入纹理 */
    if (!LoadTexture())
        MessageBox(NULL, (LPCWSTR)"载入纹理失败!", (LPCWSTR)"错误", MB_OK);
    
    /** 设置摄像机 */
    m_Camera.setCamera(0.0f, 1.5f, 6.0f, 0.0f, 1.5f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void display(void)
{
    /** 用户自定义的绘制过程 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    /** 放置摄像机 */
    m_Camera.setLook();
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   /**< 设置光源位置 */
    
    /**< 绘制过程 */
    DrawGrid();
    DrawSphere();
    DrawBox();
    
    glFlush();
}

void ChangeSize(int width, int height)
{
    glViewport(0, 0, width, height);                                    /**< 重新设置视口 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void motion(int x, int y)
{
    m_Camera.setViewByMouse();
    
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 49:
            m_Camera.setSpeed(0.2f);
            break;
        case 50:
            m_Camera.setSpeed(0.6f);
            break;
        case 119:
            m_Camera.moveCamera(m_Camera.getSpeed());
            break;
        case 115:
            m_Camera.moveCamera(-m_Camera.getSpeed());
            break;
        case 97:
            m_Camera.yawCamera(-m_Camera.getSpeed());
            break;
        case 100:
            m_Camera.yawCamera(m_Camera.getSpeed());
            break;
    }
    
    glutPostRedisplay();
    printf("========%d", key);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) >> 1) - 400, (GetSystemMetrics(SM_CYSCREEN) >> 1) - 300);
    glutCreateWindow("摄像机漫游");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}


