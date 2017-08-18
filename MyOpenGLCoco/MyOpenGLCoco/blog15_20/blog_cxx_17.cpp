//
//  blog_cxx_17.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/18.
//  Copyright © 2017年 lichong. All rights reserved.
//
////////////////////////////////纹理贴图基础
//http://blog.csdn.net/u010223072/article/details/45114685
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

/*tga图片头信息结构*/
#pragma pack(1)//结构体字节对齐
typedef struct
{
    GLbyte  identsize;              // Size of ID field that follows header (0)
    GLbyte  colorMapType;           // 0 = None, 1 = paletted
    GLbyte  imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short  colorMapStart;          // First colour map entry
    unsigned short  colorMapLength;         // Number of colors
    unsigned char   colorMapBits;   // bits per palette entry
    unsigned short  xstart;                 // image x origin
    unsigned short  ystart;                 // image y origin
    unsigned short  width;                  // width in pixels
    unsigned short  height;                 // height in pixels
    GLbyte  bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte  descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)

/*tga图片读取*/
GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
    FILE *pFile;            // File pointer
    TGAHEADER tgaHeader;        // TGA file header
    unsigned long lImageSize;       // Size in bytes of image
    short sDepth;           // Pixel depth;
    GLbyte  *pBits = NULL;          // Pointer to bits
    
    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_RGB;
    *iComponents = GL_RGB;
    
    // Attempt to open the file
    pFile = fopen(szFileName, "rb");
    if(pFile == NULL)
        return NULL;
    
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
    
    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif
    
    // Get width, height, and depth of texture
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
    
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    // Allocate memory and check for success
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if(pBits == NULL)
        return NULL;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other
    // weird formats that I don't want to recognize
    if(fread(pBits, lImageSize, 1, pFile) != 1)
    {
        free(pBits);
        return NULL;
    }
    
    // Set OpenGL format expected
    switch(sDepth)
    {
#ifndef OPENGL_ES
        case 3:     // Most likely case
            *eFormat = GL_BGR;
            *iComponents = GL_RGB;
            break;
#endif
        case 4:
            *eFormat = GL_BGRA;
            *iComponents = GL_RGBA;
            break;
        case 1:
            *eFormat = GL_LUMINANCE;
            *iComponents = GL_LUMINANCE;
            break;
        default:        // RGB
            // If on the iPhone, TGA's are BGR, and the iPhone does not
            // support BGR without alpha, but it does support RGB,
            // so a simple swizzle of the red and blue bytes will suffice.
            // For faster iPhone loads however, save your TGA's with an Alpha!
#ifdef OPENGL_ES
            for(int i = 0; i < lImageSize; i+=3)
            {
                GLbyte temp = pBits[i];
                pBits[i] = pBits[i+2];
                pBits[i+2] = temp;
            }
#endif
            break;
    }
    
    // Done with File
    fclose(pFile);
    
    // Return pointer to image data
    return pBits;
}





GLuint  texName;
GLbyte *pBits;
int nWidth, nHeight, nComponents;
GLenum eFormat;

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);// 为色彩缓冲区指定用于清除的值
    /*设置着色模式：GL_FLAT：恒定着色，用某个顶点颜色来渲染整个图元
     GL_SMOOTH：光滑着色，独立的处理图元中各个顶点的颜色。*/
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);//开启深度测试
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//设置像素存储模式
    
    
    //读取tga图像数据
    pBits =
    gltReadTGABits("tga/floor.tga", &nWidth, &nHeight, &nComponents, &eFormat);
    
    if(pBits == NULL)
        printf("tga load failed");
    
    glGenTextures(1, &texName);//生成1个未使用的纹理对象标志
    glBindTexture(GL_TEXTURE_2D, texName);//创建纹理对象(存储纹理数据的容器)
    
    /*设置纹理过滤方式*/
    //指当纹理坐标的第一维坐标值大于1.0或小于0.0时，应该如何处理。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    
    //指当纹理坐标的第二维坐标值大于1.0或小于0.0时，应该如何处理。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //纹理图象被使用到一个小于它的形状上，应该如何处理。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    /*设置如果纹理图象被使用到一个大于它的形状上时，像素如何填充，可选择的设置有GL_NEAREST和GL_LINEAR，前者表示“使用纹理中坐标最接近的一个像素的颜色作为需要绘制的像素颜色”，后者表示“使用纹理中坐标最接近的若干个颜色，通过加权平均算法得到需要绘制的像素颜色",后者效果要比前者好。*/
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //指定二维纹理
    glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

    free(pBits);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除一个或一组特定的缓冲区
    glEnable(GL_TEXTURE_2D);//激活二维纹理贴图
    //设置纹理贴图方式(直接覆盖原来颜色或与原来颜色混合)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//直接使用纹理覆盖模式
    
    glBindTexture(GL_TEXTURE_2D, texName);//指定当前使用的纹理(第一次使用和第二次使用含义不同)
    
    //为每个顶点指定纹理坐标，类似指定法线一样glNormal()
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
    
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
    glEnd();
    
    glFlush();
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);//平移
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
    glutMainLoop();
    return 0;
    
}
























