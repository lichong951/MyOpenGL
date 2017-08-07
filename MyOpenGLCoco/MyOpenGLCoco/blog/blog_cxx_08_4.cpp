//
//  blog_cxx_08_4.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/7.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////颜色材料模式：频繁更改材料属性效率更高的方式
//http://blog.csdn.net/u010223072/article/details/44975185
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

//需要频繁改变的材料属性
GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };

void init(void)
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    // 为色彩缓冲区指定用于清除的值
    glClearColor (0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel (GL_SMOOTH);
     //开启深度测试
    glEnable(GL_DEPTH_TEST);
    //材料的散射颜色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    //材料的镜面颜色
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //镜面指数
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
    //光源位置
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    //激活光照并启动0号光源(启动了就会有默认属性)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    /*频繁更改材料属性，这个效率更高，它只需要更改相应颜色值
     就可以更改材料属性，减少glMaterial*()调用次数*/
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    
    //激活颜色材料模式
    glEnable(GL_COLOR_MATERIAL);
}

void display(void)
{
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glutSolidSphere(1.0, 20, 16);
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
    if (w <= h)
    /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
     这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
        glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
                 1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
    /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
     这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
        glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
                 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
    //更改材料散射颜色
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                diffuseMaterial[0] += 0.1;
                if (diffuseMaterial[0] > 1.0)
                    diffuseMaterial[0] = 0.0;
                /**
                 （1）有光照时：像素点最终颜色P1的alpha恒为1，P1的RGB=像素RGB*光照RGB。
                 
                 （2）无光照时：像素点最终颜色P1的alpha为glColor4f()的alpha值，P1的RGB=像素RGB*glColor4f()的RGB。
                 */
                glColor4fv(diffuseMaterial);
                 //重新绘制窗口
                glutPostRedisplay();
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN) {
                diffuseMaterial[1] += 0.1;
                if (diffuseMaterial[1] > 1.0)
                    diffuseMaterial[1] = 0.0;
                /**
                 （1）有光照时：像素点最终颜色P1的alpha恒为1，P1的RGB=像素RGB*光照RGB。
                 
                 （2）无光照时：像素点最终颜色P1的alpha为glColor4f()的alpha值，P1的RGB=像素RGB*glColor4f()的RGB。
                 */
                glColor4fv(diffuseMaterial);
                 //重新绘制窗口
                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                diffuseMaterial[2] += 0.1;
                if (diffuseMaterial[2] > 1.0)
                    diffuseMaterial[2] = 0.0;
                /**
                 （1）有光照时：像素点最终颜色P1的alpha恒为1，P1的RGB=像素RGB*光照RGB。
                 
                 （2）无光照时：像素点最终颜色P1的alpha为glColor4f()的alpha值，P1的RGB=像素RGB*glColor4f()的RGB。
                 */
                glColor4fv(diffuseMaterial);
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
    switch (key) {
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
    //注册回调函数 窗口改变
    glutDisplayFunc(display);
      //注册回调函数 渲染
    glutReshapeFunc(reshape);
    
    glutMouseFunc(mouse);
     //键盘按键回调
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
    
}
















