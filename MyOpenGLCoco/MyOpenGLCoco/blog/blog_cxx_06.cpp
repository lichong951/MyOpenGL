//
//  blog_cxx_06.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/5.
//  Copyright © 2017年 lichong. All rights reserved.
//
/**
 绘制一个长方体
 http://blog.csdn.net/u010223072/article/details/44620871
 */

#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

void init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    //矩阵操作之前一般把当前矩阵设置为单位矩阵
    glLoadIdentity();
    /////////////////////////////视图变换
    //设置摄像机位置
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    ////////////////////////////模型变换
    //设置物体的位置和方向，包括：旋转，移动，缩放
    glScalef(1.0, 2.0, 1.0);
    //绘制立方体
    glutWireCube(1.0);
    glFlush();
    
}
//当窗口初次创建或改变时会被调用
void reshape(int w,int h){
    //视口变换
    //最终在屏幕上显示的大小
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    //设置当前矩阵为投影矩形
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //投影变换
    //glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    /*设置视景体，将3维坐标投影到2维屏幕坐标*/
    gluPerspective(60.0, 1, 1.5, 20.0);
    //设置当前矩阵为模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
