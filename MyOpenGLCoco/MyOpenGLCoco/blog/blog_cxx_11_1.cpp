//
//  blog_cxx_11_1.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/8.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////RGBA模式下的5个雾化球体
//http://blog.csdn.net/u010223072/article/details/45022745
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

static GLint fogMode;

static void init(void)
{
    GLfloat position[] = { 0.5, 0.5, 3.0, 0.0 };
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    //光源位置
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    //激活光照并启动0号光源(启动了就会有默认属性)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {   //{}使模块划分更加清楚
        GLfloat mat[3] = {0.1745, 0.01175, 0.01175};
        //环境光
        glMaterialfv (GL_FRONT, GL_AMBIENT, mat);
        mat[0] = 0.61424; mat[1] = 0.04136; mat[2] = 0.04136;
        //材料的散射颜色
        glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
        mat[0] = 0.727811; mat[1] = 0.626959; mat[2] = 0.626959;
        /*设置材料与光源属性*/ //材料的镜面颜色
        glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
        //镜面指数
        glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);
    }

    glEnable(GL_FOG);//激活雾效果
    {
        GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
        /*雾方程式模式，有三种形式，根据物体距离观察点的距离能
         够算得一个雾因子(默认是计算机自动算出的，但我们也可以自
         己控制雾因子)，雾就是通过雾因子把雾颜色与源片段的颜色进
         行混合得到的*/
        fogMode = GL_EXP;
        glFogi (GL_FOG_MODE, fogMode);
        glFogfv (GL_FOG_COLOR, fogColor);//雾颜色
        glFogf (GL_FOG_DENSITY, 0.35);//雾密度
        glHint (GL_FOG_HINT, GL_DONT_CARE);//在渲染质量与速度上没有偏向
        glFogf (GL_FOG_START, 1.0);//雾开始值(距离观察点开始距离)
        glFogf (GL_FOG_END, 5.0);//雾结束值
    }
    // 为色彩缓冲区指定用于清除的值
    glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void renderSphere (GLfloat x, GLfloat y, GLfloat z)
{
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    //平移
    glTranslatef (x, y, z);
    //该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glutSolidSphere(0.4, 16, 16);
    glPopMatrix();
}
void display(void)
{
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderSphere (-2., -0.5, -1.0);
    renderSphere (-1., -0.5, -2.0);
    renderSphere (0., -0.5, -3.0);
    renderSphere (1., -0.5, -4.0);
    renderSphere (2., -0.5, -5.0);
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
    /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
     这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
        glOrtho (-2.5, 2.5, -2.5*(GLfloat)h/(GLfloat)w,
                 2.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
    /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
     这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
        glOrtho (-2.5*(GLfloat)w/(GLfloat)h,
                 2.5*(GLfloat)w/(GLfloat)h, -2.5, 2.5, -10.0, 10.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}
void keyboard(unsigned char key, int x, int y)
{
    //雾效果逼真程度:GL_EXP < GL_EXP2 < GL_LINEAR
    switch (key) {
        case 'f':
        case 'F':
            if (fogMode == GL_EXP) {
                fogMode = GL_EXP2;
                printf ("Fog mode is GL_EXP2\n");
            }
            else if (fogMode == GL_EXP2) {
                fogMode = GL_LINEAR;
                printf ("Fog mode is GL_LINEAR\n");
            }
            else if (fogMode == GL_LINEAR) {
                fogMode = GL_EXP;
                printf ("Fog mode is GL_EXP\n");
            }
            /*雾方程式模式，有三种形式，根据物体距离观察点的距离能
             够算得一个雾因子(默认是计算机自动算出的，但我们也可以自
             己控制雾因子)，雾就是通过雾因子把雾颜色与源片段的颜色进
             行混合得到的*/
            glFogi (GL_FOG_MODE, fogMode);
            //重新绘制窗口
            glutPostRedisplay();
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
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
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














