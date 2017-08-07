//
//  blog_cxx_05_2.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/2.
//  Copyright © 2017年 lichong. All rights reserved.
//http://blog.csdn.net/u010223072/article/details/44517343
//使用缓冲区对象绘制正方体框

#include "GLTools.h"
#include "GLShaderManager.h"
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

#define VERTICES 0
#define INDECES 1
#define NUM_DUFFERS 2
GLuint buffers[NUM_DUFFERS];

static GLfloat vertices[] ={
    -1.0f, -1.0f, -5.0f, //前面的正方形
    1.0f, -1.0f,-5.0f,
    1.0f, 1.0f, -5.0f,
    -1.0f, 1.0f, -5.0f,
    -1.0f, -1.0f, -10.0f,//背面的正方形
    1.0f, -1.0f, -10.0f,
    1.0f, 1.0f, -10.0f,
    -1.0f, 1.0f, -10.0f
};

static GLubyte indices[]={
    0, 1, 2, 3, //前面
    0, 3, 7, 4, //左面
    5, 6, 2, 1, //右面
    7, 6, 5, 4, //后面
    3, 2, 6, 7, //上面
    1, 0, 4, 5 //地面

};

void init(){
    //背景颜色
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //多边形背部使用线条描框  
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
    //glGenBuffers是glew库的接口，要初始化它才能认识
    glewInit();
     //生成缓冲区标示符
    glGenBuffers(NUM_DUFFERS,buffers);
    //绑定顶点缓冲区对象，并设置顶点数组
    glBindBuffer(GL_ARRAY_BUFFER,buffers[VERTICES]);//绑定
    //初始化数据，即将数据拷贝到服务器内存
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
      //第二步：指定数组的数据
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    
    
}

void reshape(int w,int h){
    if (h==0) {
        h=1;
    }
    //设置视口，即显示区域
    glViewport(0, 0, w, h);
    GLfloat fAspect=(GLfloat)w/(GLfloat)h;
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective(35.0f, fAspect, 1.0f, 100.0f);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void){
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    //设置颜色
    glColor3f(0.0f, 0.0f, 1.0f);
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix();
    //激活顶点数组
    glEnableClientState(GL_VERTEX_ARRAY);
    //绑定索引缓冲区对象，并进行渲染
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffers[INDECES]);
    //初始化数据，即将数据拷贝到服务器内存
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //是用索引的形式绘制，这样比直接顶点数据渲染效率要
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);
    //关闭顶点数组
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
}

int main(int argc, char** argv){
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize (350, 350);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("VBO");
    
    init();
    //注册回调函数 渲染
    glutDisplayFunc(display);
    //注册回调函数 窗口改变
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}









































