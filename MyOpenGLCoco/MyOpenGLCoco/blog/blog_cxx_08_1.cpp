//
//  blog_cxx_08_1.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/7.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////绘制光照球体
//http://blog.csdn.net/u010223072/article/details/44975185

#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

void init(void){
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_ambient[] = {0.0, 0.0, 1.0, 1.0};
     GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
     GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    /*方向性光源(最后参数为0)：光源位于无限远处，当光线到达物体表面时，可以认为所有光线都是平行的。如太阳光
     位置性光源(最后参数非零)：不同角度位置光照强度不同，表现更加逼真。如聚光灯*/
     GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   // 为色彩缓冲区指定用于清除的值
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    
    /*设置材料与光源属性*/
     glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //光源位置
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    //环境光
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
     //散射光
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    //镜面强度
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //激活光照并启动0号光源(启动了就会有默认属性)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    
}

void display(void)
{
     //清除一个或一组特定的缓冲区
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     //该函数用于渲染一个球体
    glutSolidSphere (1.0, 20, 16);//绘制球体(它同时也定义了它的法线向量)
    //让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    glFlush ();
}

void reshape (int w, int h)
{
    //设置视口，即显示区域
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    //设置矩阵操作模式，简单的说就是设置状态，接下来将要进行投影矩阵的操作
    glMatrixMode (GL_PROJECTION);
    //指定当前矩阵（一般与glLoadIdentity一起使用）
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
    //重置当前指定的矩阵为单位矩阵,类似于一个复位操作
    glLoadIdentity();
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
    //注册回调函数 渲染
    glutDisplayFunc(display);
    //注册回调函数 窗口改变
    glutReshapeFunc(reshape);
     //键盘按键回调
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

























