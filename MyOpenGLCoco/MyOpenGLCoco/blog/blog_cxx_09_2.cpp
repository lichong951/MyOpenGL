//
//  blog_cxx_09_2.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/8.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////利用混合做遮挡透明
//http://blog.csdn.net/u010223072/article/details/44981677
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
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
static int leftFirst = GL_TRUE;
static void init(void)
{
    glEnable (GL_BLEND);//开启混合
    //设置源因子与目标因子取值
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel (GL_FLAT);
    // 为色彩缓冲区指定用于清除的值
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

static void drawLeftTriangle(void)
{
    //绘制三角形
    glBegin (GL_TRIANGLES);
    //初始化了gl_Color值，可给着色器使用
    /**
     （1）有光照时：像素点最终颜色P1的alpha恒为1，P1的RGB=像素RGB*光照RGB。
     
     （2）无光照时：像素点最终颜色P1的alpha为glColor4f()的alpha值，P1的RGB=像素RGB*glColor4f()的RGB。
     */

    glColor4f(1.0, 1.0, 0.0, 0.75);
    //指定顶点
    glVertex3f(0.1, 0.9, 0.0);
    glVertex3f(0.1, 0.1, 0.0);
    glVertex3f(0.7, 0.5, 0.0);
    glEnd();
}

static void drawRightTriangle(void)
{
    //绘制三角形
    glBegin (GL_TRIANGLES);
    //初始化了gl_Color值，可给着色器使用
    /**
     （1）有光照时：像素点最终颜色P1的alpha恒为1，P1的RGB=像素RGB*光照RGB。
     
     （2）无光照时：像素点最终颜色P1的alpha为glColor4f()的alpha值，P1的RGB=像素RGB*glColor4f()的RGB。
     */

    glColor4f(0.0, 1.0, 1.0, 0.75);
    glVertex3f(0.9, 0.9, 0.0);
    glVertex3f(0.3, 0.5, 0.0);
    glVertex3f(0.9, 0.1, 0.0);
    glEnd();
}

void display(void)
{
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (leftFirst) {
        drawLeftTriangle();//目标颜色
        drawRightTriangle();//源颜色
    }
    else {
        drawRightTriangle();
        drawLeftTriangle();
    }
    //让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    glFlush();
}

void reshape(int w, int h)
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
    if (w <= h)
        //参数设置的是视景体
        gluOrtho2D (0.0, 1.0, 0.0, 1.0*(GLfloat)h/(GLfloat)w);
    else
        //参数设置的是视景体
        gluOrtho2D (0.0, 1.0*(GLfloat)w/(GLfloat)h, 0.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 't':
        case 'T':
            leftFirst = !leftFirst;
            //重新绘制窗口
            glutPostRedisplay();
            break;
        case 27:  /*  Escape key  */
            exit(0);
            break;
        default:
            break;
    }
}


int main(int argc, char** argv)
{
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize (200, 200);
    glutCreateWindow (argv[0]);
    init();
    //注册回调函数
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc (display);
    glutMainLoop();
    return 0;
}



























