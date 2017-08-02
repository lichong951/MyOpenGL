//
//  blog_cxx_04_3.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/31.
//  Copyright © 2017年 lichong. All rights reserved.
//

//多边形的点画模式
//@link http://blog.csdn.net/u010223072/article/details/44416333

#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

void display(void){
    //位图数据(32 * 32位的)
    GLubyte fly[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x80, 0x01, 0xC0, 0x06, 0xC0, 0x03, 0x60,
        0x04, 0x60, 0x06, 0x20, 0x04, 0x30, 0x0C, 0x20,
        0x04, 0x18, 0x18, 0x20, 0x04, 0x0C, 0x30, 0x20,
        0x04, 0x06, 0x60, 0x20, 0x44, 0x03, 0xC0, 0x22,
        0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
        0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
        0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
        0x66, 0x01, 0x80, 0x66, 0x33, 0x01, 0x80, 0xCC,
        0x19, 0x81, 0x81, 0x98, 0x0C, 0xC1, 0x83, 0x30,
        0x07, 0xe1, 0x87, 0xe0, 0x03, 0x3f, 0xfc, 0xc0,
        0x03, 0x31, 0x8c, 0xc0, 0x03, 0x33, 0xcc, 0xc0,
        0x06, 0x64, 0x26, 0x60, 0x0c, 0xcc, 0x33, 0x30,
        0x18, 0xcc, 0x33, 0x18, 0x10, 0xc4, 0x23, 0x08,
        0x10, 0x63, 0xC6, 0x08, 0x10, 0x30, 0x0c, 0x08,
        0x10, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x08};
    
    GLubyte halftone[] = {
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55};
    //清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    //设置颜色
    glColor3f(1.0, 1.0, 1.0);
    //绘制矩形
    glRectf(25.0, 25.0, 125.0, 125.0);
  //激活多边形点画模式
    glEnable(GL_POLYGON_STIPPLE);
    /*glPolygonStipple:多边形点画模式，参数是一个指向32 * 32位图的指针，
     如果出现1对应的像素就被绘制，如果是0则不绘制*/
    glPolygonStipple(fly);
    //绘制矩形
    glRectf(125.0, 25.0, 225.0, 125.0);
    /*glPolygonStipple:多边形点画模式，参数是一个指向32 * 32位图的指针，
     如果出现1对应的像素就被绘制，如果是0则不绘制*/
    glPolygonStipple(halftone);
    //绘制矩形
    glRectf(225.0, 25.0, 325.0, 125.0);
    //关闭多边形点画模式
    glDisable(GL_POLYGON_STIPPLE);
    glFlush();
    
}
void init(void){
    //背景颜色
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
}
void reshape(int w,int h){
    //设置视口，即显示区域
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    //设置矩阵操作模式，简单的说就是设置状态，接下来将要进行投影矩阵的操作
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //参数设置的是视景体
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
    
    
}

void keyboard(unsigned char key,int x,int y){
    switch (key) {
        case 27://esc键
            exit(0);
            break;
            
        default:
            break;
    }
}
int main(int argc, char** argv){
    //初始化GLUT库
    glutInit(&argc, argv);
    //显示模式：单缓冲，RGB模式
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize (350, 150);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("polys");
    
    init();
    //窗口内容改变时重绘的回调函数
    glutDisplayFunc(display);
    //注册回调函数 渲染
    glutReshapeFunc(reshape);
    //键盘按键回调
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}


























