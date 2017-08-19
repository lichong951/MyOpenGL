//
//  blog_cxx_11_2.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/8.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////雾坐标形式使用雾(自己控制雾方程式)
//http://blog.csdn.net/u010223072/article/details/45022745
#include <stdio.h>

#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
/*不认识glFogCoordfEXT()的解决方案*/
#define GL_FOG_COORDINATE_SOURCE_EXT 0x8450
#define GL_FOG_COORDINATE_EXT 0x8451
PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;

static GLfloat f1, f2, f3;

static void init(void)
{
    GLfloat fogColor[4] = {0.0, 0.25, 0.25, 1.0};
    f1 = 1.0f;
    f2 = 5.0f;
    f3 = 10.0f;
    
    glEnable(GL_FOG);//激活雾效果
    glFogi (GL_FOG_MODE, GL_EXP);//设置雾方程式
    glFogfv (GL_FOG_COLOR, fogColor);//雾颜色
    glFogf (GL_FOG_DENSITY, 0.25);//雾密度
    glHint (GL_FOG_HINT, GL_DONT_CARE);//没有偏向
    glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);//使用雾坐标
    glClearColor(0.0, 0.25, 0.25, 1.0);// 为色彩缓冲区指定用于清除的值
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//清除一个或一组特定的缓冲区
    
    glColor3f (1.0f, 0.75f, 0.0f);//设置颜色
    glBegin (GL_TRIANGLES);//绘制三角形
    /*使用雾坐标，手动指定每个顶点距离观察点的距离，
     不会随观察点的变化而变化*/
    glFogCoordfEXT (f1);
    glVertex3f (2.0f, -2.0f, 0.0f);//指定顶点
    glFogCoordfEXT (f2);
    glVertex3f (-2.0f, 0.0f, -5.0f);//指定顶点
    glFogCoordfEXT (f3);
    glVertex3f (0.0f, 2.0f, -10.0f);//指定顶点
    glEnd();
    
    glutSwapBuffers();//后台缓冲区交换到前台缓冲区显示
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);//设置视口，即显示区域
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0, 1.0, 0.25, 25.0);/*透视投影,参数：角度，宽高比，近平面，远平面*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    
    glTranslatef (0.0, 0.0, -5.0);//平移
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'c':
            //不使用雾坐标
            glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FRAGMENT_DEPTH_EXT);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'C':
            //使用雾坐标
            glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);
            glutPostRedisplay();//重新绘制窗口
            break;
            //改变顶点雾坐标
        case '1':
            f1 = f1 + 0.25;
            glutPostRedisplay();//重新绘制窗口
            break;
        case '2':
            f2 = f2 + 0.25;
            glutPostRedisplay();//重新绘制窗口
            break;
        case '3':
            f3 = f3 + 0.25;
            glutPostRedisplay();//重新绘制窗口
            break;
        case '8':
            if (f1 > 0.25) {
                f1 = f1 - 0.25;
                glutPostRedisplay();//重新绘制窗口
            }
            break;
        case '9':
            if (f2 > 0.25) {
                f2 = f2 - 0.25;
                glutPostRedisplay();//重新绘制窗口
            }
            break;
        case '0':
            if (f3 > 0.25) {
                f3 = f3 - 0.25;
                glutPostRedisplay();//重新绘制窗口
            }
            break;
            //前后移动观察点
        case 'b':
            /**
             GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
             GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
             GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
             http://blog.csdn.net/u013720169/article/details/19675181
             */
            glMatrixMode (GL_MODELVIEW);
            glTranslatef (0.0, 0.0, -0.25);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'f':
            /**
             GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
             GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
             GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
             http://blog.csdn.net/u013720169/article/details/19675181
             */
            glMatrixMode (GL_MODELVIEW);
            glTranslatef (0.0, 0.0, 0.25);
            glutPostRedisplay();//重新绘制窗口
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
    glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) glutGetProcAddress("glFogCoordfEXT");
   
    glutInit(&argc, argv);//初始化GLUT库
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(500, 500);
    glutCreateWindow(argv[0]);
    init();
    //注册回调函数
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc (display);
    glutMainLoop();
    return 0;
    
}



























