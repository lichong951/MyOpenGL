//
//  blog_cxx_04.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/31.
//  Copyright © 2017年 lichong. All rights reserved.
//
//@link http://blog.csdn.net/u010223072/article/details/44416333
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

#include <math.h>
//        GL_POINTS       //点
//        GL_LINES        //线段
//        GL_LINE_STRIP   //多段线
//        GL_LINE_LOOP    //线圈
//        GL_TRIANGLES    //三角形
//        GL_TRIANGLE_STRIP //三角形条带
//        GL_TRIANGLE_FAN   //三角形扇
//        GL_QUADS          //四边形
//        GL_QUAD_STRIP     //四边形条带
//        GL_POLYGON        //多边形(凸)
const GLfloat factor=0.3f;

void display(void)
{
    GLfloat x;
     //清除一个或一组特定的缓冲区
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
