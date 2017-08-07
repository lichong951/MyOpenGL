//
//  blog_cxx_06_4.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/6.
//  Copyright © 2017年 lichong. All rights reserved.
//
/**
 模拟机器人手臂
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

static int shoulder=0,elbow=0;

void init(void)
{
    // 为色彩缓冲区指定用于清除的值
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
}

/*把握一点，opengl是一个状态机，所有的操作都是在改变状态而已*/
void display(void){
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    /*这里为什么不是先画长方体再执行这里的移动旋转，也是由于opengl是一个状态
     机，先画了，再执行这些变换就已经没作用了*/
    //平移
    glTranslatef(-1.0, 0.0, 0.0);
     //旋转
    glRotatef((GLfloat) shoulder, 0.0, 0.0, 1.0);
    //平移
    glTranslatef(1.0, 0.0, 0.0);
    /*push时记住当前的状态，pop就恢复，如果不这样，那么像这里进行了缩放操作
     的状态就会一直影响以后所有的绘制*/
    glPushMatrix();
    //表示将当前图形沿x,y,z轴分别放大
    glScaled(2.0, 0.4, 1.0);
    //绘制立体
    glutWireCube(1.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glPopMatrix();
    //平移
    glTranslatef(1.0, 0.0, 0.0);
    //旋转
    glRotatef((GLfloat) elbow, 0.0, 0.0, 1.0);
    //平移
    glTranslatef(1.0, 0.0, 0.0);
    /*push时记住当前的状态，pop就恢复，如果不这样，那么像这里进行了缩放操作
     的状态就会一直影响以后所有的绘制*/
    glPushMatrix();
    //表示将当前图形沿x,y,z轴分别放大
    glScalef(2.0, 0.4, 1.0);
    //绘制立体
    glutWireCube(1.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}
void reshape(int w,int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    
}

void keyboard(unsigned char key,int x,int y){
    switch (key) {
        case 's':
            shoulder=(shoulder+5)%360;
            glutPostRedisplay();
            break;
            case 'S':
            shoulder=(shoulder-5)%360;
            glutPostRedisplay();
            break;
            case 'e':
            elbow=(elbow+5)%360;
            glutPostRedisplay();
            break;
            case 'E':
            elbow=(elbow-5)%360;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}



























