//
//  blog_cxx_06_3.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/6.
//  Copyright © 2017年 lichong. All rights reserved.
//
/**
 模拟行星系统
 http://blog.csdn.net/u010223072/article/details/44620871
 */
#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

static int day=200;
void display()
{
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    //重置当前指定的矩阵为单位矩阵,类似于一个复位操作
    glLoadIdentity();
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective(75, 1, 1, 400);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    //重置当前指定的矩阵为单位矩阵,类似于一个复位操作
    glLoadIdentity();
    //视点转换 参考http://blog.csdn.net/augusdi/article/details/20470813
    gluLookAt(0, -200, 200, 0, 0, 0, 0, 0, 1);
    
    //太阳
    //设置颜色
    glColor3f(1.0f, 0.0f, 0.0f);
    //该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glutSolidSphere(69.6, 50, 50);
    //地球
    glColor3f(0.0f, 0.0f, 1.0f);
    /*正确地顺序是要先平移再旋转，代码的写法要相反*/
    //旋转
    glRotatef(day , 0.0f, 0.0f, -1.0f);
     //平移
    glTranslatef(150, 0.0f, 0.0f);
    //该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glutSolidSphere(15.945, 50, 50);
    //月亮
    //设置颜色
    glColor3f(1.0f, 1.0f, 0.0f);
    //旋转
    glRotatef(day / 30.0*360.0 - day , 0.0f, 0.0f, -1.0f);
    //平移
    glTranslatef(38, 0.0f, 0.0f);
    //该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glutSolidSphere(4.345, 50, 50);
    //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
    
}

void play()
{
    day++;
    if (day>=360) {
        day=0;
    }
    display();
    //重新绘制窗口
    glutPostRedisplay();
}

int main(int argc,char** argv){
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    //注册回调函数 渲染
     glutDisplayFunc(display);
    //消息循环处于空闲的时候响应 http://blog.sina.com.cn/s/blog_680374c30100l7lh.html
    glutIdleFunc(play);
    glutMainLoop();
    return 0;
}






















