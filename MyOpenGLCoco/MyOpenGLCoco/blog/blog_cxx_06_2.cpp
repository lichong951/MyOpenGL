//
//  blog_cxx_06_2.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/6.
//  Copyright © 2017年 lichong. All rights reserved.
//

/**
 绘制一个裁剪掉了3/4的线框圆(除了视景体的6个裁剪平面外，还可以另外再指定最多可达6个的其他裁剪平面，对视景体施加进一步的限制)
 http://blog.csdn.net/u010223072/article/details/44620871
 */
#include <stdio.h>
#include <glut/glut.h>

void init(void){
    //背景颜色
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
    
}

void display(void){
    //其它裁剪面方程式Ax + By + Cz + D = 0系数
    GLdouble eqn[4] = {0.0, 1.0, 0.0, 0.0};
    GLdouble eqn2[4] = {1.0, 0.0, 0.0, 0.0};
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    //设置颜色
    glColor3f(1.0, 1.0, 1.0);
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    //Z轴负方向平移5个单位
    glTranslatef(0.0, 0.0, -5.0);
    
    /* 裁剪掉y < 0 的部分 */
    glClipPlane(GL_CLIP_PLANE0, eqn);
    //激活
    glEnable(GL_CLIP_PLANE0);
    /* 裁剪掉x < 0 的部分 */
    glClipPlane(GL_CLIP_PLANE1, eqn2);
    //激活
    glEnable(GL_CLIP_PLANE1);
    /*逆时针方向绕着从原点到(1.0,0.0,0.0)的直线旋转90°*/
    glRotatef(90.0, 1.0, 0.0, 0.0);
    //绘制圆
    glutWireSphere(1.0, 20, 16);
    glPopMatrix();
    //让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    glFlush();
}

void reshape(int w,int h){
    //设置视口，即显示区域
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    //重置当前指定的矩阵为单位矩阵,类似于一个复位操作
    glLoadIdentity();
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key,int x,int y){
    switch (key) {
        case 27:
//            exit(0);
            break;
            
        default:
            break;
    }
}

int main(int argc,char** argv){
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    
    init();
    //注册回调函数 渲染
    glutDisplayFunc(display);
    //注册回调函数 窗口改变
    glutReshapeFunc(reshape);
    //键盘按键回调
    glutKeyboardFunc(keyboard);
    //循环
    glutMainLoop();
    return 0;
}

















