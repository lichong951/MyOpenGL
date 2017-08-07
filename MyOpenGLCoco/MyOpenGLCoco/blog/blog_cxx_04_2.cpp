//
//  blog_cxx_04_2.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/31.
//  Copyright © 2017年 lichong. All rights reserved.
//

/**
 直线点画模式(其实就是画虚线)
 //@link http://blog.csdn.net/u010223072/article/details/44416333
 */
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

//带参宏定义画线
#define drawOneLine(x1, y1, x2, y2) glBegin(GL_LINES);  \
glVertex2f(x1, y1);  glVertex2f(x2, y2);    glEnd();

void init()
{
    //清除一个或一组特定的缓冲区
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
}

void display(){
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    //设置颜色
    glColor3f(1.0, 0.0, 0.0);
    //激活点画线功能
    glEnable(GL_LINE_STIPPLE);
    
    /*注：glLineStipple设置当前点画模式，参数1表重复因子(即本来连续像素 * 参数1
     = 现在连续个数)，参数2是一个由1或0组成的16位序列，从低位开始，如果为1则绘制这
     个像素，否则就不绘制。*/
    glLineStipple(1,0x0101);
    drawOneLine(50.0, 125.0, 150.0, 125.0);
    glLineStipple(1, 0x00ff);
    drawOneLine(150.0, 125.0, 250.0, 125.0);
    glLineStipple(1, 0x1c47);
    drawOneLine(250.0, 125.0, 350.0, 125.0);
    //设置线宽度
    glLineWidth(5.0);
    glLineStipple(1, 0x0101);
    drawOneLine(50.0, 100.0, 150.0, 100.0);
    glLineStipple(1, 0x00ff);
    drawOneLine(150.0, 100.0, 250.0, 100.0);
    glLineStipple(1, 0x1c47);
    drawOneLine(250.0, 100.0, 350.0, 100.0);
    
    /*重复因子设置为5(即：如果有2个连续为1的位，则后面2 * 5 = 10个连续绘制,反
     之，如果3个连续为0的位，则这里3 * 5 = 15个像素连续不绘制)*/
    glLineStipple(5, 0x1c47);
    drawOneLine(50, 50, 350, 50);
    
    glDisable(GL_LINE_STIPPLE); //关闭点画线功能
     glFlush();
    
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
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //参数设置的是视景体
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
    
}

int main(int argc,char* argv[]){
    //窗口初始化
    glutInit(&argc,argv);
    //显示模式：单缓冲，RGB模式
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
//GLUT窗口大小，标题窗口
    glutInitWindowSize(400, 150);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("line");
    
    init();
    //回调函数
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
    
}

