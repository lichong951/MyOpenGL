//
//  blog_cxx_09_3.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/8.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////使用深度缓冲区进行三维混合(主要是使用glDepthMask来控制不让深度测试丢弃遮挡部分颜色值，然后就可以和基本操作一样进行混合)
//http://blog.csdn.net/u010223072/article/details/44981677
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif


#define MAXZ 8.0
#define MINZ -8.0
#define ZINC 0.4

static float solidZ = MAXZ;
static float transparentZ = MINZ;
static GLuint sphereList, cubeList;

static void init(void)
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.15 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat position[] = { 0.5, 0.5, 1.0, 0.0 };
    /*设置材料与光源属性*/ //材料的镜面颜色
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //镜面指数
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //光源位置
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    //激活光照并启动0号光源(启动了就会有默认属性)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    //使用显示列表绘制球体
    sphereList = glGenLists(1);
    //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glNewList(sphereList, GL_COMPILE);
    //该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glutSolidSphere (0.4, 16, 16);
     //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glEndList();
    
    //使用显示列表绘制立方体
    cubeList = glGenLists(1);
     //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glNewList(cubeList, GL_COMPILE);
    //该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glutSolidCube (0.6);
     //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glEndList();
}

void display(void)
{
    GLfloat mat_solid[] = { 0.75, 0.75, 0.0, 1.0 };
    GLfloat mat_zero[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_transparent[] = { 0.0, 0.8, 0.8, 0.6 };
    GLfloat mat_emission[] = { 0.0, 0.3, 0.3, 0.6 };
    //清除一个或一组特定的缓冲区
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix ();
    //平移
    glTranslatef (-0.15, -0.15, solidZ);
    //材料的发射颜色
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
    //材料的散射颜色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);
    glCallList (sphereList);/* execute the display list */
    glPopMatrix ();
    
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix ();
    //平移
    glTranslatef (0.15, 0.15, transparentZ);
    //旋转
    glRotatef (15.0, 1.0, 1.0, 0.0);
    //旋转
    glRotatef (30.0, 0.0, 1.0, 0.0);
    //材料的发射颜色
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    //材料的散射颜色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
    glEnable (GL_BLEND);//开启混合
    
    glDepthMask (GL_FALSE);//设置深度缓冲区只读(目的是不让它丢弃遮挡部分)
    //设置源因子与目标因子取值
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glCallList (cubeList);/* execute the display list */
    glDepthMask (GL_TRUE);//恢复深度缓冲区读写
    glDisable (GL_BLEND);//关闭混合
    glPopMatrix ();
    //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    //设置视口，即显示区域
    glViewport(0, 0, (GLint) w, (GLint) h);
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

void animate(void)
{
    if (solidZ <= MINZ || transparentZ >= MAXZ)
        //消息循环处于空闲的时候无响应
        glutIdleFunc(NULL);
    else {
        solidZ -= ZINC;
        transparentZ += ZINC;
        //重新绘制窗口
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'a':
        case 'A':
            solidZ = MAXZ;
            transparentZ = MINZ;
            //消息循环处于空闲的时候响应
            glutIdleFunc(animate);
            break;
        case 'r':
        case 'R':
            solidZ = MAXZ;
            transparentZ = MINZ;
            //重新绘制窗口
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
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
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}




















