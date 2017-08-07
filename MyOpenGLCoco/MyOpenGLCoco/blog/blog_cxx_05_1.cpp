//
//  blog_cxx_05_1.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/1.
//  Copyright © 2017年 lichong. All rights reserved.
//
//使用顶点数组绘制一个三角形
//http://blog.csdn.net/u010223072/article/details/44517343

#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

static GLint vertices[] = {
    25, 25,
    100, 325,
    175, 25,
    175, 325,
    250, 25,
    325, 325};

static GLfloat colors[] = {
    1.0, 0.2, 0.2,
    0.2, 0.2, 1.0,
    0.8, 1.0, 0.2,
    0.75, 0.75, 0.75,
    0.35, 0.35, 0.35,
    0.5, 0.5, 0.5};

void display(void)
{
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    //第三步：解引用和渲染 //绘制三角形
    glBegin(GL_TRIANGLES);
    /*glArrayElement(int v)：获取所有开启顶点数组的第v个数据，
     作为他们对应的设置数据，执行顺序是先执行的其它类型的顶
     点数组，最后才执行顶点坐标数据glVertex*v() */
    glArrayElement(2);
    glArrayElement (3);
    glArrayElement (5);
    glEnd();
    
    //    //上面等价于下面这种形式
    //    glBegin(GL_TRIANGLES);
    //    glColor3fv(colors + (2 * 3));
    //    glVertex2iv(vertices + (2 * 2));
    //    glColor3fv(colors + (3 * 3));
    //    glVertex2iv(vertices + (3 * 2));
    //    glColor3fv(colors + (5 * 3));
    //    glVertex2iv(vertices + (5 * 2));
    //    glEnd();
    
    glFlush();
    
}
void setupPointers(){
    //第一步：激活要使用的顶点数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //第二步：指定数组的数据
    glVertexPointer(2, GL_INT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
}


void init()
{
    // 为色彩缓冲区指定用于清除的值
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_SMOOTH);
    setupPointers();
}

void reshape(int w,int h)
{
    //设置视口，即显示区域
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //参数设置的是视景体
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

int main(int argc,char** argv)
{
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize (350, 350);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("varray");
    init();
    //注册回调函数 渲染
    glutDisplayFunc(display);
    //注册回调函数 窗口改变
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}





















