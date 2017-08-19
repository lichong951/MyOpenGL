//
//  blog_cxx_13.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/9.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////多边形偏移
//http://blog.csdn.net/u010223072/article/details/45029663
#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

GLuint list;//正整形
GLint spinx = 0;
GLint spiny = 0;
GLfloat tdist = 0.0;
GLfloat polyfactor = 1.0;
GLfloat polyunits = 1.0;

void display (void)
{
    GLfloat gray[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除一个或一组特定的缓冲区
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix ();
    glTranslatef (0.0, 0.0, tdist);//平移
    glRotatef ((GLfloat) spinx, 1.0, 0.0, 0.0);//旋转
    glRotatef ((GLfloat) spiny, 0.0, 1.0, 0.0);//旋转
    
    //在开启光照与多边形偏移下，绘制一个实心球体
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);//环境颜色 散射颜色
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);//材料的镜面颜色
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0);//镜面指数
    glEnable(GL_LIGHTING);//激活光照并启动0号光源(启动了就会有默认属性)
    glEnable(GL_LIGHT0);
    glEnable(GL_POLYGON_OFFSET_FILL);//激活多边形偏移
    glPolygonOffset(polyfactor, polyunits);//计算出一个偏移值，以后每个片段的深度值都加上这个值
    glCallList (list);/* execute the display list */
    //在关闭光照与多边形偏移下，同一位置绘制一个线框球体
    glDisable(GL_POLYGON_OFFSET_FILL);//关闭多边形偏移
    glDisable(GL_LIGHTING);//激活光照并启动0号光源(启动了就会有默认属性)
    glDisable(GL_LIGHT0);
    glColor3f (1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//多边形背部使用线条描框
    glCallList (list);/* execute the display list */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//多边形背部使用线条描框
    
    glPopMatrix ();
    glFlush ();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
}

void gfxinit (void)
{
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    
    GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    
    glClearColor (0.0, 0.0, 0.0, 1.0);
    
    list = glGenLists(1);
    glNewList (list, GL_COMPILE);//OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glutSolidSphere(1.0, 20, 12);//该函数用于渲染一个球体。球体球心位于原点，参数分别为
    glEndList ();//OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    
    glEnable(GL_DEPTH_TEST);//开启深度测试
    
    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);//环境光
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);//材料的散射颜色
    glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);//材料的镜面颜色
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);//光源位置
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, global_ambient);//环境光强度
}

void reshape(int width, int height)
{
    glViewport (0, 0, width, height);//设置视口，即显示区域
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, (GLdouble)width/(GLdouble)height,
                   1.0, 10.0);/*透视投影,参数：角度，宽高比，近平面，远平面*/
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    //设置摄像机位置
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            switch (state) {
                case GLUT_DOWN:
                    spinx = (spinx + 5) % 360;
                    glutPostRedisplay();//重新绘制窗口
                    break;
                default:
                    break;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            switch (state) {
                case GLUT_DOWN:
                    spiny = (spiny + 5) % 360;
                    glutPostRedisplay();//重新绘制窗口
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
            //调节观察点距离
        case 't':
            if (tdist < 4.0) {
                tdist = (tdist + 0.5);
                glutPostRedisplay();//重新绘制窗口
            }
            break;
        case 'T':
            if (tdist > -5.0) {
                tdist = (tdist - 0.5);
                glutPostRedisplay();//重新绘制窗口
            }
            break;
            //调节计算偏移值的两个参数
        case 'F':
            polyfactor = polyfactor + 0.1;
            printf ("polyfactor is %f\n", polyfactor);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'f':
            polyfactor = polyfactor - 0.1;
            printf ("polyfactor is %f\n", polyfactor);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'U':
            polyunits = polyunits + 1.0;
            printf ("polyunits is %f\n", polyunits);
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'u':
            polyunits = polyunits - 1.0;
            printf ("polyunits is %f\n", polyunits);
            glutPostRedisplay();//重新绘制窗口
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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    //注册回调函数
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    gfxinit();
    glutMainLoop();
    return 0;
}


