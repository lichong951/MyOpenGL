//
//  blog_cxx_18.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/19.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////替换纹理图像的全部或一部分
//http://blog.csdn.net/u010223072/article/details/45150933
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define checkImageWidth 64
#define checkImageHeight 64
#define subImageWidth 45
#define subImageHeight 45
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte subImage[subImageHeight][subImageWidth][4];

static GLuint texName;

//纹理数据(黑白相间的棋盘和蓝色区域)
void makeCheckImages(void)
{
    int i, j, c;
    
    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i&0x8)==0)^((j&0x8))==0))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
            checkImage[i][j][3] = (GLubyte) 255;
        }
    }
    for (i = 0; i < subImageHeight; i++) {
        for (j = 0; j < subImageWidth; j++) {
            subImage[i][j][0] = (GLubyte) 0;
            subImage[i][j][1] = (GLubyte) 0;
            subImage[i][j][2] = (GLubyte) 255;
            subImage[i][j][3] = (GLubyte) 255;
        }
    }
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);// 为色彩缓冲区指定用于清除的值
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);//开启深度测试
    
    makeCheckImages();
    /*设置像素的储存模式，参数1包括：GL_PACK_ALIGNMENT：打包
     与GL_UNPACK_ALIGNMENT：解包两种。参数2是对齐方式，如这里
     是取字母，一个字母占1字节，所以以1字节对齐*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &texName);//生成1个未使用的纹理对象标志
    glBindTexture(GL_TEXTURE_2D, texName);
    /*设置纹理过滤方式*/
    //指当纹理坐标的第一维坐标值大于1.0或小于0.0时，应该如何处理。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //指定二维纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}
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
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除一个或一组特定的缓冲区
    glEnable(GL_TEXTURE_2D);//激活二维纹理贴图
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//直接使用纹理覆盖模式
    glBindTexture(GL_TEXTURE_2D, texName);//创建纹理对象(存储纹理数据的容器)
    glBegin(GL_QUADS);//四边形
    //为每个顶点指定纹理坐标，类似指定法线一样glNormal()
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
    
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
    glEnd();
    glFlush();//让缓冲区中的图像最终显示，而不是一直放在缓冲区等待
    glDisable(GL_TEXTURE_2D);//关闭二维纹理贴图
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
    glMatrixMode(GL_PROJECTION);//GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);/*透视投影,参数：角度，宽高比，近平面，远平面*/
    glMatrixMode(GL_MODELVIEW);//把其后的矩阵操作施加于造型视图矩阵栈。（默认）
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);//平移
}


void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 's':
        case 'S':
            //指定当前活动的纹理
            glBindTexture(GL_TEXTURE_2D, texName);
            /*用subImage纹理替换原纹理的(12,12)至
             (subImageWidth,subImageHeight)区域*/
            glTexSubImage2D(GL_TEXTURE_2D, 0, 12, 12,
                            subImageWidth, subImageHeight,
                            GL_RGBA,GL_UNSIGNED_BYTE, subImage);
            
            glutPostRedisplay();//重新绘制窗口
            break;
        case 'r':
        case 'R':
            glBindTexture(GL_TEXTURE_2D, texName);//创建纹理对象(存储纹理数据的容器)
            //指定二维纹理
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         checkImageWidth, checkImageHeight, 0,
                         GL_RGBA,GL_UNSIGNED_BYTE, checkImage);
            
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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(250, 250);
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


