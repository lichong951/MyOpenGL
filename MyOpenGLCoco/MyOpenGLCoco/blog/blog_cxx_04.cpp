//
//  blog_cxx_04.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/31.
//  Copyright © 2017年 lichong. All rights reserved.
//

#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

#include <math.h>

const GLfloat factor=0.3f;

void display(void)
{
    GLfloat x;
    //清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    //线段
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);         // 画x轴
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);         // 画y轴
    glEnd();
    
    //线带
    glBegin(GL_LINE_STRIP);
    for(x=-1.0f/factor; x<1.0f/factor; x+=0.01f)
    {
        glVertex2f(x*factor, sin(x)*factor);
    }
    glEnd();
    //用于单缓冲中
    glFlush();
}

int main(int argc,char* argv[]){
    //窗口初始化
    glutInit(&argc, argv);
     //显示模式：单缓冲，RGB模式
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
     //GLUT窗口大小，标题窗口
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("sinFunc");
    //回调函数
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
