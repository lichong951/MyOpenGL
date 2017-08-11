//
//  blog_cxx_10_1.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/8.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////使用混合形式的抗锯齿
//http://blog.csdn.net/u010223072/article/details/44995675
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
//        GL_POINTS       //点
//        GL_LINES        //线段
//        GL_LINE_STRIP   //多段线
//        GL_LINE_LOOP    //线圈
//        GL_TRIANGLES    //三角形
//        GL_TRIANGLE_STRIP //三角形条带
//        GL_TRIANGLE_FAN   //三角形扇
//        GL_QUADS          //四边形
//        GL_QUAD_STRIP     //四边形条带
//        GL_POLYGON        //多边形(凸)
static float rotAngle = 0.;
void init(void)
{
    GLfloat values[2];
    /*获取opengl支持信息*/
    glGetFloatv (GL_LINE_WIDTH_GRANULARITY, values);//线宽变化量（梯度）
    printf ("GL_LINE_WIDTH_GRANULARITY value is %3.1f\n", values[0]);
    glGetFloatv (GL_LINE_WIDTH_RANGE, values);//支持的线宽范围
    printf ("GL_LINE_WIDTH_RANGE values are %3.1f %3.1f\n", values[0], values[1]);
    
    glEnable (GL_LINE_SMOOTH);//启用线抗锯齿,边缘会降低其alpha值
    glEnable (GL_BLEND);//启用混合
    //设置源因子与目标因子取值
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /*对图像质量和渲染速度之间控制权衡关系*/
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);//没有偏向
    //设置线宽度
    glLineWidth (1.5);
    // 为色彩缓冲区指定用于清除的值
    glClearColor(0.0, 0.0, 0.0, 0.0);
}
void display(void)
{
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    //绘制绿线(默认alpha=1)
    glColor3f (0.0, 1.0, 0.0);//设置颜色
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    //旋转
    glRotatef(-rotAngle, 0.0, 0.0, 0.1);
    //线段
    glBegin (GL_LINES);
    glVertex2f (-0.5, 0.5);
    glVertex2f (0.5, -0.5);
    glEnd ();
    glPopMatrix();
    //绘制蓝线
    glColor3f (0.0, 0.0, 1.0);//设置颜色
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    //旋转
    glRotatef(rotAngle, 0.0, 0.0, 0.1);
    //线段
    glBegin (GL_LINES);
    glVertex2f (0.5, 0.5);
    glVertex2f (-0.5, -0.5);
    glEnd ();
    glPopMatrix();
     //让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    glFlush();
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
    if (w <= h)
        //参数设置的是视景体
        gluOrtho2D (-1.0, 1.0,
                    -1.0*(GLfloat)h/(GLfloat)w, 1.0*(GLfloat)h/(GLfloat)w);
    else
        //参数设置的是视景体
        gluOrtho2D (-1.0*(GLfloat)w/(GLfloat)h,
                    1.0*(GLfloat)w/(GLfloat)h, -1.0, 1.0);
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
        case 'r':
        case 'R':
            rotAngle += 20.;
            if (rotAngle >= 360.) rotAngle = 0.;
             //重新绘制窗口
            glutPostRedisplay();
            break;
        case 27:  /*  Escape Key  */
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
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize (200, 200);
    glutCreateWindow (argv[0]);
    init();
    //注册回调函数
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc (display);
    glutMainLoop();
    return 0;
    
}
