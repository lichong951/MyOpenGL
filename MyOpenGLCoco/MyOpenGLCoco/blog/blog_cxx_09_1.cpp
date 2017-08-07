//
//  blog_cxx_09_1.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/7.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////背景颜色与几何物体颜色做基本混合
//http://blog.csdn.net/u010223072/article/details/44981677
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

void init(void)
{
    // 为色彩缓冲区指定用于清除的值
    glClearColor(1.0, 1.0, 0.0, 0.0);
    
    glBlendFunc(GL_ONE, GL_ONE);//设置源因子与目标因子取值
    glEnable(GL_BLEND);//开启混合
}
void display(void)
{
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    //这个方形与背景混合
    glColor3f(0.0, 0.0, 1.0);
    //绘制矩形
    glRectf(-0.5,-0.5,0.5,0.5);
    //让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    glFlush();
}

void reshape(int w, int h)
{
    GLdouble aspect = (GLdouble) w / h;
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
    if (aspect < 1.0) {
        aspect = 1.0 / aspect;
        /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
         这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
        glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
    } else
    /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
     这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
        glOrtho(-1.0, 1.0, -aspect, aspect, -1.0, 1.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    //设置混合的计算方式
    switch (key) {
        case 'a': case 'A':
            //源颜色+目标颜色(默认形式)
            glBlendEquation(GL_FUNC_ADD);
            break;
            
        case 's': case 'S':
            //源颜色-目标颜色
            glBlendEquation(GL_FUNC_SUBTRACT);
            break;
            
        case 'r': case 'R':
            //目标颜色-源颜色
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            break;
            
        case 'm': case 'M':
            //分别取目标颜色和源颜色rgba中较小的值
            glBlendEquation(GL_MIN);
            break;
            
        case 'x': case 'X':
            //分别取目标颜色和源颜色rgba中较大的值
            glBlendEquation(GL_MAX);
            break;
            
        case 27:
            exit(0);
    }
    
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(512,512);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    //渲染环境检测
    glewInit();
    //注册回调函数
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
    
}
