//
//  blog_cxx_14.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/11.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////显示列表
//http://blog.csdn.net/u010223072/article/details/45038395
#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define PI_ 3.14159265358979323846

GLuint theTorus;//正整形
//绘制圆环
static void torus(int numc, int numt)
{
    int i, j, k;
    double s, t, x, y, z, twopi;
    
    twopi = 2 * PI_;
    for (i = 0; i < numc; i++) {
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
        //http://blog.csdn.net/csxiaoshui/article/details/54923763
        glBegin(GL_QUAD_STRIP);//四边形条带
        for (j = 0; j <= numt; j++) {
            for (k = 1; k >= 0; k--) {
                s = (i + k) % numc + 0.5;
                t = j % numt;
                
                x = (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt);
                y = (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt);
                z = .1 * sin(s * twopi / numc);
                glVertex3f(x, y, z);//指定顶点
            }
        }
        glEnd();
    }
}
static void init(void)
{
    theTorus = glGenLists (1);//分配一个未使用的索引值  //使用显示列表绘制
    /*创建显示列表*/
    glNewList(theTorus, GL_COMPILE);//显示列表的开始  //OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    torus(8, 25);
    glEndList();//显示列表结束
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);// 为色彩缓冲区指定用于清除的值
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//清除一个或一组特定的缓冲区
    glColor3f (1.0, 1.0, 1.0);//设置颜色
    glCallList(theTorus);//调用显示列表执行
    glFlush();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
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
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0); //设置摄像机位置
}

void keyboard(unsigned char key, int x, int y)
{
    //绕X,Y旋转，还原
    switch (key) {
        case 'x':
        case 'X':
            glRotatef(30.,1.0,0.0,0.0);//旋转
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'y':
        case 'Y':
            glRotatef(30.,0.0,1.0,0.0);//旋转
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'i':
        case 'I':
            glLoadIdentity();
            gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);//设置摄像机位置
            glutPostRedisplay();//重新绘制窗口
            break;
        case 27:
            exit(0);
            break;
    }
}

int main(int argc, char **argv)
{
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(200, 200);
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow(argv[0]);
    init();
    //注册回调函数
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
    
}




























