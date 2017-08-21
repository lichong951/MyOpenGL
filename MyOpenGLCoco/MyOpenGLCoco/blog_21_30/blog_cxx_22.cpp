//
//  blog_cxx_22.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/21.
//  Copyright © 2017年 lichong. All rights reserved.
//
/**
 多重纹理
 多重纹理是指在同一个模型表面指定两个或两个以上的纹理，这些纹理通过一定的融合方式进行混合形成的效果。其每一层各自执行自己的纹理操作，并把结果传递给下一层，直到全部完全。主要应用在如：光照效果，贴花，合成，细节纹理等
 http://blog.csdn.net/u010223072/article/details/45199717
 
 */

#include <GLTools.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

static GLubyte texels0[32][32][4];
static GLubyte texels1[16][16][4];

//2个纹理数据
void makeCheckImages(void)
{
    int i, j;
    
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            texels0[i][j][0] = (GLubyte) i * 8;
            texels0[i][j][1] = (GLubyte) j * 8;
            texels0[i][j][2] = (GLubyte) (i*j)/255;
            texels0[i][j][3] = (GLubyte) 255;
        }
    }
    
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            texels1[i][j][0] = (GLubyte) 255;
            texels1[i][j][1] = (GLubyte) i * 8;
            texels1[i][j][2] = (GLubyte) j * 8;
            texels1[i][j][3] = (GLubyte) 255;
        }
    }
}

void init(void)
{
    GLuint texNames[2];
    
    glClearColor (0.0, 0.0, 0.0, 0.0);// 为色彩缓冲区指定用于清除的值
    glShadeModel(GL_FLAT);//设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
    glEnable(GL_DEPTH_TEST);//开启深度测试
    
    makeCheckImages();
    /*设置像素的储存模式，参数1包括：GL_PACK_ALIGNMENT：打包
     与GL_UNPACK_ALIGNMENT：解包两种。参数2是对齐方式，如这里
     是取字母，一个字母占1字节，所以以1字节对齐*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(2, texNames);//生成2个未使用的纹理对象标志
    
    //设置第一个纹理对象属性
    glBindTexture(GL_TEXTURE_2D, texNames[0]);
    //指定二维纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texels0);
     /*设置纹理过滤方式*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    //设置第二个纹理对象属性
    glBindTexture(GL_TEXTURE_2D, texNames[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texels1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glActiveTextureARB (GL_TEXTURE0_ARB);//激活第一层，并绑定
    glEnable(GL_TEXTURE_2D);//激活二维纹理贴图
    glBindTexture(GL_TEXTURE_2D, texNames[0]);
    /*设置贴图方式,就是现在颜色与原来颜色怎么操作*/
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//替换
    
    //激活第二层，并绑定
    glActiveTextureARB (GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texNames[1]);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//调整
    
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除一个或一组特定的缓冲区
    glBegin(GL_TRIANGLES);//绘制三角形
    glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 0.0);//设置多重纹理坐标
    glMultiTexCoord2fARB (GL_TEXTURE1_ARB, 1.0, 0.0);
    glVertex2f(0.0, 0.0);
    glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.5, 1.0);
    glMultiTexCoord2fARB (GL_TEXTURE1_ARB, 0.5, 0.0);
    glVertex2f(50.0, 100.0);
    glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 0.0);
    glMultiTexCoord2fARB (GL_TEXTURE1_ARB, 1.0, 1.0);
    glVertex2f(100.0, 0.0);
    glEnd();
    glFlush();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);//设置视口，即显示区域
    glMatrixMode(GL_PROJECTION);//GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈
    glLoadIdentity();
    if (w <= h)
        //参数设置的是视景体
        gluOrtho2D(0.0, 100.0, 0.0, 100.0 * (GLdouble)h/(GLdouble)w);
    else
        gluOrtho2D(0.0, 100.0 * (GLdouble)w/(GLdouble)h, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);//GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
    glLoadIdentity();
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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    //注册回调函数
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}








































