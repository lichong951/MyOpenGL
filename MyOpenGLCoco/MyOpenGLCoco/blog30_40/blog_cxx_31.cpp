//
//  blog_cxx_31.cpp
//  blog_cxx_31
//
//  Created by lichong on 2017/10/5.
//  Copyright © 2017年 lichong. All rights reserved.
//OpenGL--分格化
//http://blog.csdn.net/u010223072/article/details/45338629
/*
 理论基础
 分格化：OpenGL渲染的都是一些简单的凸多边形，对于那些复杂的多边形，如凹多边形，含孔多边形以及自交叉的多边形，OpenGL函数库不能对他们进行渲染。不过OpenGL实用库(GLU)提供了一系列分格化函数，它们可以将一个复杂的多边形划分为一系列简单凸多边形，我们就把这个过程叫做分格化(又称网格化)。
 分格化一般步骤：
     1，创建分格化对象
     2，注册对应阶段分格处理的回调函数(复杂点的是相交处回调处理)
     3，指定分格化属性(主要是环绕数规则设定，以确认是内部还是外部)
     4，指定一个或多个封闭多边形的轮廓数据
     5，删除分格化对象
 ***/
#include <stdio.h>
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#ifndef CALLBACK
#define CALLBACK
#endif

GLuint startList;

void display (void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glCallList(startList);
    glCallList(startList + 1);
    glFlush();
}

/*注册对应阶段回调函数*/
//开始绘制回调(类似glBegin())
void CALLBACK beginCallback(GLenum which)
{
    glBegin(which);
}

//错误回调
void CALLBACK errorCallback(GLenum errorCode)
{
    const GLubyte *estring;
    
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s\n", estring);
    exit(0);
}
//结束绘制回调(类似glEnd())
void CALLBACK endCallback(void)
{
    glEnd();
}

//顶点数据回调
void CALLBACK vertexCallback(GLdouble *vertex)
{
    const GLdouble *pointer;
    
    pointer = (GLdouble *) vertex;
    glColor3dv(pointer+3);
    glVertex3dv(vertex);
}

//相交处创建新顶点回调
void CALLBACK combineCallback(GLdouble coords[3],
                              GLdouble *vertex_data[4],
                              GLfloat weight[4], GLdouble **dataOut )
{
    GLdouble *vertex;
    int i;
    
    vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));
    
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    for (i = 3; i < 6; i++)
        vertex[i] = weight[0] * vertex_data[0][i]
        + weight[1] * vertex_data[1][i]
        + weight[2] * vertex_data[2][i]
        + weight[3] * vertex_data[3][i];
    *dataOut = vertex;
}

void init (void)
{
    GLUtesselator *tobj;
    //矩形，三角形，五星顶点数据
    GLdouble rect[4][3] = {50.0, 50.0, 0.0,
        200.0, 50.0, 0.0,
        200.0, 200.0, 0.0,
        50.0, 200.0, 0.0};
    GLdouble tri[3][3] = {75.0, 75.0, 0.0,
        125.0, 175.0, 0.0,
        175.0, 75.0, 0.0};
    GLdouble star[5][6] = {250.0, 50.0, 0.0, 1.0, 0.0, 1.0,
        325.0, 200.0, 0.0, 1.0, 1.0, 0.0,
        400.0, 50.0, 0.0, 0.0, 1.0, 1.0,
        250.0, 150.0, 0.0, 1.0, 0.0, 0.0,
        400.0, 150.0, 0.0, 0.0, 1.0, 0.0};
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    startList = glGenLists(2);
    
    //创建分格化对象
    tobj = gluNewTess();
    //对应阶段的回调
    gluTessCallback(tobj, GLU_TESS_VERTEX,
                    (void(__stdcall*)())glVertex3dv);
    gluTessCallback(tobj, GLU_TESS_BEGIN,
                    (void(__stdcall*)())beginCallback);
    gluTessCallback(tobj, GLU_TESS_END,
                    (void(__stdcall*)())endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR,
                    (void(__stdcall*)())errorCallback);
    
    glNewList(startList, GL_COMPILE);
    glShadeModel(GL_FLAT);
    gluTessBeginPolygon(tobj, NULL);//类似glBegin()
    //矩形
    gluTessBeginContour(tobj);//指定一条闭合的轮廓线的开始(即需要进行分格化的凹多边形数据)
    gluTessVertex(tobj, rect[0], rect[0]);
    gluTessVertex(tobj, rect[1], rect[1]);
    gluTessVertex(tobj, rect[2], rect[2]);
    gluTessVertex(tobj, rect[3], rect[3]);
    gluTessEndContour(tobj);//指定轮廓线结束
    //三角形
    gluTessBeginContour(tobj);
    gluTessVertex(tobj, tri[0], tri[0]);
    gluTessVertex(tobj, tri[1], tri[1]);
    gluTessVertex(tobj, tri[2], tri[2]);
    gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);//类似glEnd()
    glEndList();
    
    gluTessCallback(tobj, GLU_TESS_VERTEX,
                    (void(__stdcall*)())vertexCallback);
    gluTessCallback(tobj, GLU_TESS_BEGIN,
                    (void(__stdcall*)())beginCallback);
    gluTessCallback(tobj, GLU_TESS_END,
                    (void(__stdcall*)())endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR,
                    (void(__stdcall*)())errorCallback);
    gluTessCallback(tobj, GLU_TESS_COMBINE,
                    (void(__stdcall*)())combineCallback);
    
    glNewList(startList + 1, GL_COMPILE);
    glShadeModel(GL_SMOOTH);
    //指定分格化属性
    gluTessProperty(tobj, GLU_TESS_WINDING_RULE,
                    GLU_TESS_WINDING_POSITIVE);//环绕数为正数则为内部区域
    gluTessBeginPolygon(tobj, NULL);
    gluTessBeginContour(tobj);
    gluTessVertex(tobj, star[0], star[0]);
    gluTessVertex(tobj, star[1], star[1]);
    gluTessVertex(tobj, star[2], star[2]);
    gluTessVertex(tobj, star[3], star[3]);
    gluTessVertex(tobj, star[4], star[4]);
    gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);
    glEndList();
    
    gluDeleteTess(tobj);//删除分格对象
}

void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
