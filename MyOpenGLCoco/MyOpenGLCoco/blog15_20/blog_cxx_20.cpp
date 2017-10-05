//
//  blog_cxx_20.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/21.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////自动生成纹理坐标
//http://blog.csdn.net/u010223072/article/details/45170351
#include <stdio.h>
#include <GLTools.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define stripeImageWidth 32
GLubyte stripeImage[4*stripeImageWidth];
static GLuint texName;

//贴图纹理
void makeStripeImage(void)
{
    int j;
    
    for (j = 0; j < stripeImageWidth; j++) {
        stripeImage[4*j] = (GLubyte) ((j<=4) ? 255 : 0);
        stripeImage[4*j+1] = (GLubyte) ((j>4) ? 255 : 0);
        stripeImage[4*j+2] = (GLubyte) 0;
        stripeImage[4*j+3] = (GLubyte) 255;
    }
}
static GLfloat xequalzero[] = {1.0, 0.0, 0.0, 0.0};
static GLfloat slanted[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat *currentCoeff;
static GLenum currentPlane;
static GLint currentGenMode;

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);// 为色彩缓冲区指定用于清除的值
    glEnable(GL_DEPTH_TEST);//开启深度测试
    glShadeModel(GL_SMOOTH);//GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色
    
    makeStripeImage();
    /*设置像素的储存模式，参数1包括：GL_PACK_ALIGNMENT：打包
     与GL_UNPACK_ALIGNMENT：解包两种。参数2是对齐方式，如这里
     是取字母，一个字母占1字节，所以以1字节对齐*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &texName);//生成1个未使用的纹理对象标志
    glBindTexture(GL_TEXTURE_1D, texName);//创建纹理对象(存储纹理数据的容器)
    
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);/*设置纹理过滤方式*/
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
    /*设置纹理和物体表面颜色处理方式*/
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//和物体表面颜色做与运算
    
    /*设置自动生成纹理坐标的计算方式*/
    currentCoeff = xequalzero;
    currentGenMode = GL_OBJECT_LINEAR;
    currentPlane = GL_OBJECT_PLANE;
    //一维纹理，只有S坐标
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);//整形形式
    glTexGenfv(GL_S, currentPlane, currentCoeff);//向量形式
    
    glEnable(GL_TEXTURE_GEN_S);//激活s坐标的纹理坐标生成
    glEnable(GL_TEXTURE_1D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);//激活光照并启动0号光源
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glMaterialf (GL_FRONT, GL_SHININESS, 64.0);
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除一个或一组特定的缓冲区
    /*矩阵堆栈，可以消除上一次的变换对本次变换的影响，一系列像平移，旋转，
     缩放等的矩阵变换操作放在它们之间可以简化操作，不然每次改变都要重新
     设置矩阵模式*/
    glPushMatrix ();
    glRotatef(45.0, 0.0, 0.0, 1.0);//旋转
    glBindTexture(GL_TEXTURE_1D, texName);
    glutSolidTeapot(2.0);
    glPopMatrix ();
    glFlush();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);//设置视口，即显示区域
    glMatrixMode(GL_PROJECTION);//GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈
    glLoadIdentity();
    if (w <= h)
    /*正交投影（Xmin, Xmax, Ymin, Ymax, Zmin, Zmax）,参数指定了投影的长方体，
     这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。*/
        glOrtho (-3.5, 3.5, -3.5*(GLfloat)h/(GLfloat)w,
                 3.5*(GLfloat)h/(GLfloat)w, -3.5, 3.5);
    else
        glOrtho (-3.5*(GLfloat)w/(GLfloat)h,
                 3.5*(GLfloat)w/(GLfloat)h, -3.5, 3.5, -3.5, 3.5);
    glMatrixMode(GL_MODELVIEW);//GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
    glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'e':
        case 'E':
            currentGenMode = GL_EYE_LINEAR;//视觉坐标系中计算纹理坐标
            currentPlane = GL_EYE_PLANE;//向量形式
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);//整形形式
            glTexGenfv(GL_S, currentPlane, currentCoeff);//向量形式
            //重新绘制窗口
            glutPostRedisplay();
            break;
        case 'o':
        case 'O':
            currentGenMode = GL_OBJECT_LINEAR;//模型坐标系中计算纹理坐标
            currentPlane = GL_OBJECT_PLANE;
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);//整数形式
            glTexGenfv(GL_S, currentPlane, currentCoeff);//模型坐标系中计算，计算系数由currentCoeff向量指定
            glutPostRedisplay();//重新绘制窗口
            break;
        case 's':
        case 'S':
            currentCoeff = slanted;
            glTexGenfv(GL_S, currentPlane, currentCoeff);//向量形式
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'x':
        case 'X':
            currentCoeff = xequalzero;
            glTexGenfv(GL_S, currentPlane, currentCoeff);//向量形式
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
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(256, 256);
    glutInitWindowPosition(100, 100);
    glutCreateWindow (argv[0]);
    init ();
     //注册回调函数
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

















