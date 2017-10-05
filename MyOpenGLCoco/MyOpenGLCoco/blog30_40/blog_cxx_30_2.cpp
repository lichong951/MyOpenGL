//
//  blog_cxx_30_2.cpp
//  blog_cxx_30_2
//
//  Created by lichong on 2017/10/5.
//  Copyright © 2017年 lichong. All rights reserved.
//平面镜反射实例
//

#include <stdio.h>
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

void drawSphere()
{
    // 设置光源
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {
        GLfloat
        pos[]      = {5.0f, 5.0f, 0.0f, 1.0f},
        ambient[] = {0.0f, 0.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    }
    
    // 绘制一个球体
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, 2);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void display(void)
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    // 绘制前面球体
    glDisable(GL_STENCIL_TEST);
    drawSphere();
    
    // 绘制一个平面镜。
    // 为了保证平面镜之后的镜像能够正确绘制，在绘制平面镜时需要将深度缓冲区设置为只读的。
    // 在绘制时暂时关闭光照效果
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_STENCIL_TEST);
    
    glDisable(GL_LIGHTING);
    glColor3f(0.5f, 0.5f, 0.5f);
    glDepthMask(GL_FALSE);//深度缓冲区不能写入(只读)
    glRectf(-1.5f, -1.5f, 1.5f, 1.5f);//即这个绘制不会覆盖以前像素，而是丢弃自己
    glDepthMask(GL_TRUE);//恢复写入
    
    // 绘制一个与先前球体关于平面镜对称的球体，注意光源的位置也要发生对称改变
    // 因为平面镜是在X轴和Y轴所确定的平面，所以只要Z坐标取反即可实现对称
    // 为了保证球体的绘制范围被限制在平面镜内部，使用模板测试
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glScalef(1.0f, 1.0f, -1.0f);
    drawSphere();
    
    glFlush();
}

void reshape(int w, int h)
{
    if(h == 0)
        h = 1;
    
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat) w/(GLfloat) h, 5, 25);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(5, 0, 6.5, 0, 0, 0, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB
                         | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
