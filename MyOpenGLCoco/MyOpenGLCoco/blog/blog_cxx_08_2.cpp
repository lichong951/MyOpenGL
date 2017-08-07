//
//  blog_cxx_08_2.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/7.
//  Copyright © 2017年 lichong. All rights reserved.
//

#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

static int spin=0;
void init(void){
    //清除一个或一组特定的缓冲区
    glClearColor (0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
     glShadeModel (GL_SMOOTH);
    //激活光照并启动0号光源(启动了就会有默认属性)
    glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    
}
void display(void){
    GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };
    //清除一个或一组特定的缓冲区
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix ();
    //设置摄像机位置
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix ();
    /*光源旋转*/
    glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);
     //光源位置
     glLightfv (GL_LIGHT0, GL_POSITION, position);
    
    /*绘制一个不受光照的正方体表示光源*/
     glTranslated (0.0, 0.0, 1.5);
    
    //关闭光照并启动0号光源
    glDisable (GL_LIGHTING);
    //设置颜色
    glColor3f (0.0, 1.0, 1.0);
    //绘制立方体
    glutWireCube (0.1);
    //激活光照并启
     glEnable (GL_LIGHTING);
    glPopMatrix ();
    //绘制的圆环体 innerRadius:圆环的内半径 outerRadius:圆环的外半径
    glutSolidTorus (0.275, 0.85, 8, 15);
    
     glPopMatrix ();
    //让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    glFlush ();
    
}

void reshape (int w, int h)
{
    //设置视口，即显示区域
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
}

void mouse(int button, int state, int x, int y){
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                spin = (spin + 30) % 360;
                //重新绘制窗口
                glutPostRedisplay();
            }
            break;
        default:
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
    }
}

int main(int argc, char** argv)
{
     //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
     //GLUT窗口大小，标题窗口
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    
    init ();
    //注册回调函数 渲染
    glutDisplayFunc(display);
    //注册回调函数 窗口改变
    glutReshapeFunc(reshape);
    //鼠标按钮回调
    glutMouseFunc(mouse);
     //键盘按键回调
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}














