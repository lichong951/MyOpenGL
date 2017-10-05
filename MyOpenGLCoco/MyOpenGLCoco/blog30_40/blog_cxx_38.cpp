//
//  blog_cxx_38.cpp
//  blog_cxx_38
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
#include "CSkyBox.hpp"

Camera m_Camera;
CSkyBox m_SkyBox;


void init(void)
{
    /** 用户自定义的初始化过程 */
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    /** 启用纹理 */
    glEnable(GL_TEXTURE_2D);
    
    /** 初始化天空 */
    if (!m_SkyBox.Init())
    {
        MessageBox(NULL, (LPCWSTR)"初始化天空失败!", (LPCWSTR)"错误", MB_OK);
        exit(0);
    }
    
    /** 设置摄像机 */
    m_Camera.setCamera(500, 35, 400, 501, 35, 400, 0, 1, 0);
}

void display(void)
{
    /** 用户自定义的绘制过程 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    /** 放置摄像机 */
    m_Camera.setLook();
    
    /** 绘制天空 */
    m_SkyBox.CreateSkyBox(0, 0, 0, 1.0, 0.5, 1.0);
    
    glFlush();                   /**< 强制执行所有的OpenGL命令 */
}

void ChangeSize(int width, int height)
{
    glViewport(0, 0, width, height);                                    /**< 重新设置视口 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 4000.0f);
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
        case '1':
            m_Camera.setSpeed(0.2f);
            break;
        case '2':
            m_Camera.setSpeed(1.0f);
            break;
        case 'w':
            m_Camera.moveCamera(m_Camera.getSpeed());
            break;
        case 's':
            m_Camera.moveCamera(-m_Camera.getSpeed());
            break;
        case 'a':
            m_Camera.yawCamera(-m_Camera.getSpeed());
            break;
        case 'd':
            m_Camera.yawCamera(m_Camera.getSpeed());
            break;
    }
    
    glutPostRedisplay();
    printf("key::%d", key);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) >> 1) - 400, (GetSystemMetrics(SM_CYSCREEN) >> 1) - 300);
    glutCreateWindow("天空盒");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

