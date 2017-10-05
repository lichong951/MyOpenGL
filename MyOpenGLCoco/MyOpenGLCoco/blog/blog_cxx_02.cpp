//
//  blog_cxx_02.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/30.
//  Copyright © 2017年 lichong. All rights reserved.
//
/**
 
 http://blog.csdn.net/u010223072/article/details/44310517
 */
#include "GLTools.h"
#include "GLShaderManager.h"
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
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
//http://blog.csdn.net/csxiaoshui/article/details/54923763
void display(){
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
     //设置颜色
    glColor3f(1.0,0.0,0.0);
    //绘制多边形
    glBegin(GL_POLYGON);
    //指定顶点
    glVertex3f(0.25, 0.25, 0.0);
    glVertex3f(0.75, 0.25, 0.0);
    glVertex3f(0.75, 0.75, 0.0);
    glVertex3f(0.25, 0.75, 0.0);
    
    glEnd();
    
    glFlush();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    
}

void init()
{
    // 为色彩缓冲区指定用于清除的值
    glClearColor(0.0, 0.0, 1.0, 0.0);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    //重置当前指定的矩阵为单位矩阵,类似于一个复位操作
    glLoadIdentity();
    /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
     这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc,char* argv[])
{
    //窗口初始化
    glutInit(&argc,argv);
    //显示模式：单缓冲，RGB模式
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //窗口大小
    glutInitWindowSize(250, 250);
    //窗口位置
    glutInitWindowPosition(100, 100);
    glutCreateWindow("hello");
    init();
    //窗口内容改变时重绘的回调函数
    glutDisplayFunc(display);
    glutMainLoop(); //循环
    return 0;
}

 
 

