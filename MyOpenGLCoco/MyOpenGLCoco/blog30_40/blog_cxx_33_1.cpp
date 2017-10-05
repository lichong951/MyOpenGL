//
//  blog_cxx_33_1.cpp
//  blog_cxx_33_1
//
//  Created by lichong on 2017/10/5.
//  Copyright © 2017年 lichong. All rights reserved.
//曲线
/*
 理论基础
 贝塞尔曲线和曲面：OpenGL只能直接绘制基本图元，对于曲线和曲面我们一般采用一系列线段或多边形来模拟的，这样当线段或多边形增多时必定很耗性能。其实对于这种曲线和曲面，我们可以使用一些控制点，通过求值器程序先计算出坐标等信息，然后直接用这些数据绘制，这样不仅节省内存，还提高了模拟曲线或曲面的精度(本质还是通过线段或多边形绘制的，只是求值器提前算出了曲线或曲面的顶点信息)。
 求值器使用一般步骤：1，启用求值器 2，定义求值器 3，执行求值器。
 注释：OpenGl3.1后，本节内容都已经废弃了，这些顶点着色器都可以实现了。
 http://blog.csdn.net/u010223072/article/details/45363293
 **/

#include <stdio.h>
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

//控制点
GLfloat ctrlpoints[4][3] = {
    { -4.0, -4.0, 0.0}, { -2.0, 4.0, 0.0},
    {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    //定义一维求值器
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    //启动求职器
    glEnable(GL_MAP1_VERTEX_3);
}

void display(void)
{
    int i;
    
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 30; i++)
        glEvalCoord1f((GLfloat) i/30.0);//执行求值器，每执行一次产生一个坐标
    glEnd();
    
    //绘制4个控制点
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (i = 0; i < 4; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
    glFlush();
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
                5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
    else
        glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
                5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc (keyboard);
    glutMainLoop();
    return 0;
}

