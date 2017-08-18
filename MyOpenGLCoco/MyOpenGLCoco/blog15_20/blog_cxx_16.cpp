//
//  blog_cxx_16.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/18.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////图像
//http://blog.csdn.net/u010223072/article/details/45059019

#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageHeight][checkImageWidth][3];

static GLdouble zoomFactor = 1.0;
static GLint height;

void makeCheckImage(void)
{
    int i, j, c;
    //棋盘图像数据
    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i&0x8)==0)^((j&0x8))==0))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
        }
    }
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);// 为色彩缓冲区指定用于清除的值
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
    makeCheckImage();
    /*设置像素的储存模式，参数1包括：GL_PACK_ALIGNMENT：打包
     与GL_UNPACK_ALIGNMENT：解包两种。参数2是对齐方式，如这里
     是取字母，一个字母占1字节，所以以1字节对齐*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//设置像素存储模式
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//清除一个或一组特定的缓冲区
    glRasterPos2i(0, 0);//设置光栅化位置
    
    glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB, GL_UNSIGNED_BYTE, checkImage);//绘制图像
    glFlush();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);//设置视口，即显示区域
    height = (GLint) h;
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//鼠标拖动
void motion(int x, int y)
{
    static GLint screeny;
    
    screeny = height - (GLint) y;
    glRasterPos2i (x, screeny);//光栅位置
    glPixelZoom (zoomFactor, zoomFactor);//x,y方向进行缩放
    //复制颜色缓冲区指定的矩形区域到帧缓冲区
    glCopyPixels (0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
    glPixelZoom (1.0, 1.0);//x,y方向进行缩放
    glFlush ();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'r':
        case 'R':
            zoomFactor = 1.0;
            glutPostRedisplay();//重新绘制窗口
            printf ("zoomFactor reset to 1.0\n");
            break;
        case 'z':
            zoomFactor += 0.5;
            if (zoomFactor >= 3.0)
                zoomFactor = 3.0;
            printf ("zoomFactor is now %4.1f\n", zoomFactor);
            break;
        case 'Z':
            zoomFactor -= 0.5;
            if (zoomFactor <= 0.5)
                zoomFactor = 0.5;
            printf ("zoomFactor is now %4.1f\n", zoomFactor);
            break;
        case 27:
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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    //注册回调函数
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
    
}






















