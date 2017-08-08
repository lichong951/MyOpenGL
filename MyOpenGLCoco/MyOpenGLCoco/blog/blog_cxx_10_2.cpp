//
//  blog_cxx_10_2.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/8.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////使用多重采样的抗锯齿
//http://blog.csdn.net/u010223072/article/details/44995675
#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

/*多重采样步骤：
 1，获得一个支持多重采样的窗口
 2，查看系统是否支持
 3，激活多重采样
 */

static int bgtoggle = 1;
void init(void)
{
    static GLint buf[1], sbuf[1];
    int i, j;
    // 为色彩缓冲区指定用于清除的值
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*查看系统是否支持多重采样*/ //返回一个选中参数的值，glGetDoublev, glGetFloatv, glGetIntegerv的函数意义与此相同
    glGetIntegerv (GL_SAMPLE_BUFFERS_ARB, buf);
    printf ("number of sample buffers is %d\n", buf[0]);
    //返回一个选中参数的值，glGetDoublev, glGetFloatv, glGetIntegerv的函数意义与此相同
    glGetIntegerv (GL_SAMPLES_ARB, sbuf);
    printf ("number of samples is %d\n", sbuf[0]);
    
    /*显示列表形式*/
    //线段与三角形交替环绕
    //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glNewList (1, GL_COMPILE);
    for (i = 0; i < 19; i++) {
        /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
         缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
         设置矩阵模式*/
        glPushMatrix();
        //旋转
        glRotatef(360.0*(float)i/19.0, 0.0, 0.0, 1.0);
         //设置颜色
        glColor3f (1.0, 1.0, 1.0);
        //设置线宽度
        glLineWidth((i%3)+1.0);
        glBegin (GL_LINES);//线段
        glVertex2f (0.25, 0.05);
        glVertex2f (0.9, 0.2);
        glEnd ();
        //设置颜色
        glColor3f (0.0, 1.0, 1.0);
        glBegin (GL_TRIANGLES);//三角形
        glVertex2f (0.25, 0.0);
        glVertex2f (0.9, 0.0);
        glVertex2f (0.875, 0.10);
        glEnd ();
        glPopMatrix();
    }
    //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glEndList ();
    
    //二维正方形背景
    //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glNewList (2, GL_COMPILE);
     //设置颜色
    glColor3f (1.0, 0.5, 0.0);
    glBegin (GL_QUADS);//正方形
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            if (((i + j) % 2) == 0) {
                glVertex2f (-2.0 + (i * 0.25), -2.0 + (j * 0.25));
                glVertex2f (-2.0 + (i * 0.25), -1.75 + (j * 0.25));
                glVertex2f (-1.75 + (i * 0.25), -1.75 + (j * 0.25));
                glVertex2f (-1.75 + (i * 0.25), -2.0 + (j * 0.25));
            }
        }
    }
    glEnd ();
    //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glEndList ();
}

void display(void)
{
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (bgtoggle)
        glCallList (2);//二维正方形背景绘制
    
    //左边
    glEnable (GL_MULTISAMPLE_ARB);//激活多重采样
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    //平移
    glTranslatef (-1.0, 0.0, 0.0);
    glCallList (1);//线段和三角形环绕绘制
    glPopMatrix();
    
    //右边
    glDisable (GL_MULTISAMPLE_ARB);//关闭多重采样
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    //平移
    glTranslatef (1.0, 0.0, 0.0);
    //线段和三角形环绕绘制
    glCallList (1);
    glPopMatrix();
    //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
}
void reshape(int w, int h)
{
    //设置视口，即显示区域
    glViewport(0, 0, w, h);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= (2 * h))
         //参数设置的是视景体
        gluOrtho2D (-2.0, 2.0,
                    -2.0*(GLfloat)h/(GLfloat)w, 2.0*(GLfloat)h/(GLfloat)w);
    else
         //参数设置的是视景体
        gluOrtho2D (-2.0*(GLfloat)w/(GLfloat)h,
                    2.0*(GLfloat)w/(GLfloat)h, -2.0, 2.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'b':
        case 'B':
            bgtoggle = !bgtoggle;
            //重新绘制窗口
            glutPostRedisplay();
            break;
        case 27:  /*  Escape Key  */
            exit(0);
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    //初始化GLUT库
    glutInit(&argc, argv);
    //创建一个支持多重采样，双缓冲，RGB颜色模式的窗口
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
     //GLUT窗口大小，标题窗口
    glutInitWindowSize (600, 300);
    glutCreateWindow (argv[0]);
    init();
    //注册回调函数
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc (display);
    glutMainLoop();
    return 0;
}




























