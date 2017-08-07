//
//  blog_cxx_03.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/31.
//  Copyright © 2017年 lichong. All rights reserved.
//
//@link http://blog.csdn.net/u010223072/article/details/44343533
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

static GLfloat angle=0.0;

void display(){
    //清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
     //旋转
    glRotatef(angle, 0.0, 0.0, 1.0);
    //设置颜色
    glColor3f(1.0, 0.0, 0.0);
    //绘制矩形
    glRectf(-25.0, -25.0, 25.0, 25.0);
    glPopMatrix();
    //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
   
}
void init(){
     //清除一个或一组特定的缓冲区
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
}

void reshape(int w,int h){
    //设置视口，即显示区域
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);//指定当前矩阵（一般与glLoadIdentity一起使用）
    glLoadIdentity();
    //glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0); //参数设置的是视景体
    if (w<=h) {//设置视景体与视口宽高比例一致，这样改变窗口大小，物体也不会变形
        glOrtho(-50.0, 50.0, -50.0 * (GLfloat)h/(GLfloat)w, 50.0 * (GLfloat)h/(GLfloat)w, -1.0, 1.0);
    }else{
        glOrtho(-50.0*(GLfloat)w/(GLfloat)h, 50*(GLfloat)w/(GLfloat)h, -50, 50, -1.0, 1.0);
    }
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void spinDisplay(){
    angle=angle+2.0;
    if (angle>360.0) {
        angle=angle-360.0;
        
    }
    //重新绘制窗口
    glutPostRedisplay();
}

void mouse(int button,int state,int x,int y){
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state==GLUT_DOWN) {
                //消息循环处于空闲的时候响应
                glutIdleFunc(spinDisplay);
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state==GLUT_DOWN) {
                glutIdleFunc(NULL);
                
            }
            break;
        default:
            break;
    }
}



int main(int argc,char* argv[]){
    //窗口初始化
    glutInit(&argc, argv);
    //显示模式：双缓冲，RGB模式
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("double");
    
    init();
    
    //
    glutDisplayFunc(display);
    //窗口大小改变调用的回调函数
    glutReshapeFunc(reshape);
    //鼠标按钮回调
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}




























