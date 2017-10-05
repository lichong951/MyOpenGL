//
//  blog_cxx_30_1.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/10/1.
//  Copyright © 2017年 lichong. All rights reserved.
//http://blog.csdn.net/u010223072/article/details/45271827
/**
 模板测试：通俗点讲就是为屏幕每个像素设置了一个模板值，然后拿一个参考值与之比较，满足条件就通过，则对应像素就会绘制，否则不绘制。类似深度测试一样，只是服务情形不同，深度测试主要用来消除背面的，而模板测试主要可以实现裁剪，指明哪里可以绘制，哪里不绘制。(比较规则:glStencilFunc，修改模板缓冲区的值:glStencilOp)
 
 */

#include <stdio.h>
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
#define YELLOWMAT 1
#define BLUEMAT 2

void init(void)
{
    GLfloat yellow_diffuse[]={0.7, 0.7, 0.0, 1.0};
    GLfloat yellow_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat blue_diffuse[] = { 0.1, 0.1, 0.7, 1.0 };
    GLfloat blue_specular[] = { 0.1, 1.0, 1.0, 1.0 };
    
    GLfloat position_one[] = { 1.0, 1.0, 1.0, 0.0 };
    
    glNewList(YELLOWMAT, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 64.0);
    glEndList();
    
    glNewList(BLUEMAT, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 45.0);
    glEndList();
    
    glLightfv(GL_LIGHT0, GL_POSITION, position_one);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    glClearStencil(0x0);//设置以 0 来重置模板缓冲区的值
    glEnable(GL_STENCIL_TEST);//激活模板测试
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* 绘制圆 */
    /*模板控制的像素筛选方式*/
    glStencilFunc (GL_EQUAL, 0x1, 0x1);//模板缓冲区值=1对应的像素才绘制
    /*模板缓冲区中的值修改方式(3个参数对应3种情况下的操作：1，模板测试失败 2，模板测试通过，深度测试失败 3，模板深度测试都通过)*/
    glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);//模板缓冲区的值保存不变
    glCallList (BLUEMAT);
    glutSolidSphere (0.5, 15, 15);
    
    /* 绘制两个圆环 */
    glStencilFunc (GL_NOTEQUAL, 0x1, 0x1);//不等于1才绘制
    glPushMatrix();
    glRotatef (45.0, 0.0, 0.0, 1.0);
    glRotatef (45.0, 0.0, 1.0, 0.0);
    glCallList (YELLOWMAT);
    glutSolidTorus (0.275, 0.85, 15, 15);
    
    glPushMatrix();
    
     glRotatef (90.0, 1.0, 0.0, 0.0);
    glutSolidTorus (0.275, 0.85, 15, 15);
    glPopMatrix();
    glPopMatrix();
    glFlush();
    
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluOrtho2D(-3.0, 3.0, -3.0*(GLfloat)h/(GLfloat)w,
                   3.0*(GLfloat)h/(GLfloat)w);
    else
        gluOrtho2D(-3.0*(GLfloat)w/(GLfloat)h,
                   3.0*(GLfloat)w/(GLfloat)h, -3.0, 3.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_STENCIL_BUFFER_BIT);//清除模板缓冲区
    glStencilFunc (GL_ALWAYS, 0x1, 0x1);//总是通过
    glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);//用上面设置的参考值0x1替换模板缓冲区的值
    glBegin(GL_QUADS);//绘制方形
    glVertex2f (-1.0, 0.0);
    glVertex2f (0.0, 1.0);
    glVertex2f (1.0, 0.0);
    glVertex2f (0.0, -1.0);
    glEnd();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 3.0, 7.0);
     glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
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
    init ();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

















