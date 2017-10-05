//
//  blog_cxx_23.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/23.
//  Copyright © 2017年 lichong. All rights reserved.
//
/**
 OpenGL--纹理组合器函数
 纹理组合器函数(glTexEnvf):简单点讲就是指定纹理贴图和材质混合的方式。固定管线时代，它只是告诉你，纹理的像素读取出来之后，其实还可以经过一些复杂变换，然后才拿去使用，这样可以搞出一些绚丽效果。不过现在shader可以进行更多更自由的变换，所以这东西现在就没什么用了。
 http://blog.csdn.net/u010223072/article/details/45224667
 */
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define imageWidth 8
#define imageHeight 8

static GLubyte image0[imageHeight][imageWidth][4];
static GLubyte image1[imageHeight][imageWidth][4];

static GLuint texName[4];

void makeImages(void)
{
    int i, j, c;
    for (i = 0; i < imageHeight; i++) {
        for (j = 0; j < imageWidth; j++) {
            c = ((i&2)==0)*255;
            image0[i][j][0] = (GLubyte) c;
            image0[i][j][1] = (GLubyte) c;
            image0[i][j][2] = (GLubyte) c;
            image0[i][j][3] = (GLubyte) 255;
            c = ((j&4)!=0)*128;
            image1[i][j][0] = (GLubyte) 0;
            image1[i][j][1] = (GLubyte) c;
            image1[i][j][2] = (GLubyte) c;
            image1[i][j][3] = (GLubyte) 255;
        }
    }
}

void init(void)
{
    GLint numunits[1];
    
    glClearColor (0.0, 0.0, 0.0, 0.0);// 为色彩缓冲区指定用于清除的值
    glShadeModel(GL_SMOOTH);// GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色
    
    makeImages();
    /*设置像素的储存模式，参数1包括：GL_PACK_ALIGNMENT：打包
     与GL_UNPACK_ALIGNMENT：解包两种。参数2是对齐方式，如这里
     是取字母，一个字母占1字节，所以以1字节对齐*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(4, texName);//生成4个未使用的纹理对象标志
    //创建两个纹理对象
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    /*设置纹理过滤方式*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //指定二维纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image0);
    
    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //指定二维纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
    //绘制四边形作为材质
    glNewList (1, GL_COMPILE);//OpenGL提供类似于绘制图元的结构即类似于glBegin()与glEnd()的形式创建显示列表
    glBegin(GL_QUADS);//四边形
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0, 0.0);//设置多重纹理坐标
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0, 0.0);//设置多重纹理坐标
    glColor3f (0.5, 1.0, 0.25);//设置颜色
    glVertex3f(0.0, 0.0, 0.0);//指定顶点
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0, 2.0);//设置多重纹理坐标
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0, 2.0);
    glColor3f (1.0, 1.0, 1.0);
    glVertex3f(0.0, 1.0, 0.0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 2.0, 2.0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 2.0, 2.0);
    glColor3f (1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 0.0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 2.0, 0.0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 2.0, 0.0);
    glColor3f (1.0, 0.5, 0.25);
    glVertex3f(1.0, 0.0, 0.0);
    glEnd();
    glEndList ();
}

void display(void)
{
    static GLfloat constColor[4] = {0.0, 0.0, 0.0, 0.0};
    
    glClear(GL_COLOR_BUFFER_BIT);//清除一个或一组特定的缓冲区
    
    //没有使用贴图的方形图
    glDisable(GL_TEXTURE_2D);
    /*(1,1)*/
    glPushMatrix();
    glTranslatef(0.0, 5.0, 0.0);
    glCallList(1);/* execute the display list */
    glPopMatrix();
    
    //启用贴图，在原图与现在纹理做调整混合模式下，分别绘制纹理对象0，1
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//调整
    /*(1,2)*/
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glTranslatef(1.0, 5.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(1,3)*/
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glTranslatef(2.0, 5.0, 0.0);
    glCallList(1);
    glPopMatrix();
    
    //启动纹理组合器模式下，用纹理对象0实现的效果
    /*(2,2)*/
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);//启动纹理组合器函数
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
    glPushMatrix();
    glTranslatef(1.0, 4.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(2,3)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
    glPushMatrix();
    glTranslatef(2.0, 4.0, 0.0);//平移
    glCallList(1);
    glPopMatrix();
    /*(2,4)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD);
    glPushMatrix();
    glTranslatef(3.0, 4.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(2,5)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD_SIGNED_ARB);
    glPushMatrix();
    glTranslatef(4.0, 4.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(2,6)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_SUBTRACT_ARB);
    glPushMatrix();
    glTranslatef(5.0, 4.0, 0.0);
    glCallList(1);
    glPopMatrix();
    
    //启动纹理组合器模式下，用纹理对象0在加重颜色蓝色成分下实现的效果
    /*(3,2)*/
    constColor[3] = 0.2;
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_CONSTANT_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);
    glPushMatrix();
    glTranslatef(1.0, 3.0, 0.0);//平移
    glCallList(1);
    glPopMatrix();
    /*(3,3)*/
    constColor[3] = 0.4;
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
    glPushMatrix();
    glTranslatef(2.0, 3.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(3,4)*/
    constColor[3] = 0.6;
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
    glPushMatrix();
    glTranslatef(3.0, 3.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(3,5)*/
    constColor[4] = 0.8;
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
    glPushMatrix();
    glTranslatef(4.0, 3.0, 0.0);
    glCallList(1);
    glPopMatrix();
    
    //多重纹理下使用纹理组合器函数
    glActiveTextureARB (GL_TEXTURE0_ARB);
    glEnable (GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glActiveTextureARB (GL_TEXTURE1_ARB);
    glEnable (GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
    /*(4,2)*/
    glPushMatrix();
    glTranslatef(1.0, 2.0, 0.0);
    glCallList(1);
    glPopMatrix();
    
    /*(4,3)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
    glPushMatrix();
    glTranslatef(2.0, 2.0, 0.0);//平移
    glCallList(1);
    glPopMatrix();
    /*(4,4)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD);
    glPushMatrix();
    glTranslatef(3.0, 2.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(4,5)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD_SIGNED_ARB);
    glPushMatrix();
    glTranslatef(4.0, 2.0, 0.0);//平移
    glCallList(1);
    glPopMatrix();
    /*(4,6)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_SUBTRACT_ARB);
    glPushMatrix();
    glTranslatef(5.0, 2.0, 0.0);
    glCallList(1);
    glPopMatrix();
    
    //使用组合器的一些例子
    /*(5,2)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2.0);
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
    glPushMatrix();
    glTranslatef(1.0, 1.0, 0.0);
    glCallList(1);
    glPopMatrix();
    /*(5,3)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
    glPushMatrix();
    glTranslatef(2.0, 1.0, 0.0);
    glCallList(1);
    glPopMatrix();
    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.0);
    
    /*  using SOURCE0 and SOURCE1, reverse the order of subtraction Arg1-Arg0  */
    /*(5,6)*/
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_SUBTRACT_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
    glPushMatrix();
    glTranslatef(5.0, 1.0, 0.0);
    glCallList(1);
    glPopMatrix();
    
    glActiveTextureARB (GL_TEXTURE1_ARB);//指定第二层纹理关闭贴图
    glDisable (GL_TEXTURE_2D);
    glActiveTextureARB (GL_TEXTURE0_ARB); //恢复为第一层，即就是没有开启多重纹理模式
    
    glFlush();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);//设置视口，即显示区域
    glMatrixMode(GL_PROJECTION);//GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈
    glLoadIdentity();
    gluOrtho2D(0.0, 7.0, 0.0, 7.0);//参数设置的是视景体
    glMatrixMode(GL_MODELVIEW);//GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
    glLoadIdentity();
}
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    //扩展库接口需要获得它的函数的指针后才能使用
    glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC)glutGetProcAddress("glActiveTextureARB");
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)glutGetProcAddress("glMultiTexCoord2fARB");
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    //注册回调函数
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
































