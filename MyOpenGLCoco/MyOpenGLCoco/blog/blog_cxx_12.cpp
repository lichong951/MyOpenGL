//
//  blog_cxx_12.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/9.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////点参数
//http://blog.csdn.net/u010223072/article/details/45027277
#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif


/*不识别glPointParameterfEXT(),glPointParameterfvEXT()的解决方案*/
#define GL_DISTANCE_ATTENUATION_EXT 0x8129
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT 0x8128
#define GL_POINT_SIZE_MIN_EXT 0x8126
#define GL_POINT_SIZE_MAX_EXT 0x8127

#define GL_POINT_FADE_THRESHOLD_SIZE_ARB  0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129

PFNGLPOINTPARAMETERFARBPROC glPointParameterfEXT;
PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvEXT;

static GLfloat psize = 7.0;
static GLfloat pmax[1];
static GLfloat constant[3] = {1.0, 0.0, 0.0};
static GLfloat linear[3] = {0.0, 0.12, 0.0};
static GLfloat quadratic[3] = {0.0, 0.0, 0.01};

void init(void)
{
    glPointParameterfEXT = (PFNGLPOINTPARAMETERFARBPROC)glutGetProcAddress("glPointParameterfARB");
    glPointParameterfvEXT = (PFNGLPOINTPARAMETERFVARBPROC)glutGetProcAddress("glPointParameterfvARB");
    
    int i;
    
    srand (12345);
    
    //随机绘制250个点
    glNewList(1, GL_COMPILE);//OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glBegin (GL_POINTS);//批次处理
    for (i = 0; i < 250; i++) {
        //设置颜色
        glColor3f (1.0, ((rand()/(float) RAND_MAX) * 0.5) + 0.5,
                   rand()/(float) RAND_MAX);
        //指定顶点
        /*-5 < x < 5;  -5 < y < 5;  -5 < z < -45  */
        glVertex3f ( ((rand()/(float)RAND_MAX) * 10.0) - 5.0,
                    ((rand()/(float)RAND_MAX) * 10.0) - 5.0,
                    ((rand()/(float)RAND_MAX) * 40.0) - 45.0);
    }
    glEnd();
    glEndList();//OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    
    glEnable(GL_DEPTH_TEST);//开启深度测试
    glEnable(GL_POINT_SMOOTH);//点抗锯齿(点是正方形的，开启抗锯齿修正为圆点)
    glEnable(GL_BLEND);//使用混合形式抗锯齿
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置源因子与目标因子取值
    glPointSize(psize);//设置点大小
    glGetFloatv(GL_POINT_SIZE_MAX_EXT, pmax);//获得系统支持的最大点
    
    /*param就是一个包含三个浮点值(a,b,c)的数组，分别包含了常
     数，线性或二次衰减系数用于根据点与观察点的距离d来计算点的大
     小和亮度*/
    glPointParameterfvEXT (GL_DISTANCE_ATTENUATION_EXT, linear);
    
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除一个或一组特定的缓冲区
    glCallList (1);//绘制250个点
    glutSwapBuffers ();//后台缓冲区交换到前台缓冲区显示
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);//设置视口，即显示区域
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective (35.0, 1.0, 0.25, 200.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode (GL_MODELVIEW);
    glTranslatef (0.0, 0.0, -10.0);//平移
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
            //调整观察点位置
        case 'b':
            /**
             GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
             GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
             GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
             http://blog.csdn.net/u013720169/article/details/19675181
             */
            glMatrixMode (GL_MODELVIEW);
            glTranslatef (0.0, 0.0, -0.5);
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
            glTranslatef (0.0, 0.0, 0.5);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'c':
            //根据常数的形式来算点大小与亮度
            glPointParameterfvEXT (GL_DISTANCE_ATTENUATION_EXT, constant);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'l':
            //根据线性的形式计算点大小与亮度
            glPointParameterfvEXT (GL_DISTANCE_ATTENUATION_EXT, linear);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'q':
            //根据二次衰减系数的形式计算点大小与亮度
            glPointParameterfvEXT (GL_DISTANCE_ATTENUATION_EXT, quadratic);
            glutPostRedisplay();//重新绘制窗口
            break;
            //调整点大小
        case '+':
            if (psize < (pmax[0] + 1.0))
                psize = psize + 1.0;
            glPointSize (psize);//设置点大小
            glutPostRedisplay();//重新绘制窗口
            break;
        case '-':
            if (psize >= 2.0)
                psize = psize - 1.0;
            glPointSize (psize);//设置点大小
            glutPostRedisplay();//重新绘制窗口
            break;
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
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    //注册回调函数
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutMainLoop();
    return 0;
}
























