//
//  blog_cxx_19.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/19.
//  Copyright © 2017年 lichong. All rights reserved.
//
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
#include <stdio.h>
/*注释：如果gltReadTGABits读取失败，可以自己定义这个接口，具体的定义前面章节有*/
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
///////////////////////////////////////////读取图片文件end



GLShaderManager     shaderManager;          // Shader Manager
GLMatrixStack       modelViewMatrix;        // Modelview Matrix
GLMatrixStack       projectionMatrix;       // Projection Matrix
GLFrustum           viewFrustum;            // View Frustum
GLGeometryTransform transformPipeline;      // Geometry Transform Pipeline

GLBatch             floorBatch;
GLBatch             ceilingBatch;
GLBatch             leftWallBatch;
GLBatch             rightWallBatch;

GLfloat             viewZ = -65.0f;

// Texture objects
#define TEXTURE_BRICK   0
#define TEXTURE_FLOOR   1
#define TEXTURE_CEILING 2
#define TEXTURE_COUNT   3
GLuint  textures[TEXTURE_COUNT];
const char *szTextureFiles[TEXTURE_COUNT] = {
    "/Users/lichong/Documents/openGL/code/MyOpenGLCoco/MyOpenGLCoco/tga/brick.tga",
    "/Users/lichong/Documents/openGL/code/MyOpenGLCoco/MyOpenGLCoco/tga/floor.tga",
    "/Users/lichong/Documents/openGL/code/MyOpenGLCoco/MyOpenGLCoco/tga/ceiling.tga" };

// 设置纹理过滤方式
void ProcessMenu(int value)
{
    GLfloat fLargest;
    GLint iLoop;
    
    for(iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);//创建纹理对象(存储纹理数据的容器)
        
        switch(value)
        {
                //单纹理，远近调节时有明显闪烁不自然情况
            case 0:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);/*设置纹理过滤方式*/
                break;
                
            case 1:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);/*设置纹理过滤方式*/
                break;
                //设置多级纹理的过滤方式(远模糊近清晰，有效避免闪烁情况)
            case 2:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);/*设置纹理过滤方式*/
                break;
                
            case 3:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);/*设置纹理过滤方式*/
                break;
                
            case 4:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);/*设置纹理过滤方式*/
                break;
                
            case 5:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);/*设置纹理过滤方式*/
                break;
                //开启各向异性过滤(过滤时还考虑了观察的角度，效果最好方式，但也是最耗性能的)
            case 6:
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);//变化量（梯度）
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
                break;
                //恢复为各向同性过滤(过滤时不考虑观察角度，默认垂直方向采样，如果是倾斜观察，这种模式下会丢失一些纹理信息)
            case 7:
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
                break;
        }
    }
    
    glutPostRedisplay();//重新绘制窗口
}

void init()
{
    GLbyte *pBytes;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
    GLint iLoop;
    
    glClearColor(0.0f, 0.0f, 0.0f,1.0f);// 为色彩缓冲区指定用于清除的值
    
    shaderManager.InitializeStockShaders();//初始化着色器管理器
    
    glGenTextures(TEXTURE_COUNT, textures);//生成TEXTURE_COUNT个未使用的纹理对象标志
    for(iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);//创建纹理对象(存储纹理数据的容器)
        
        pBytes = gltReadTGABits(szTextureFiles[iLoop],&iWidth, &iHeight,
                                &iComponents, &eFormat);
        if(pBytes == NULL)
            printf("tga load failed");
        /*设置纹理过滤方式*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //指定二维纹理
        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        
        glGenerateMipmap(GL_TEXTURE_2D);//为纹理生成一组完整的mipmap，即多级纹理(分辨率递减的同一纹理)
        
        free(pBytes);
    }
    
    //绘制地板几何图形
    GLfloat z;
    floorBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);//三角形条带
    for(z = 60.0f; z >= 0.0f; z -=10.0f)
    {
        // Top
        floorBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        floorBatch.Vertex3f(-10.0f, -10.0f, z);
        
        floorBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        floorBatch.Vertex3f(10.0f, -10.0f, z);
        
        floorBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        floorBatch.Vertex3f(-10.0f, -10.0f, z - 10.0f);
        
        floorBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        floorBatch.Vertex3f(10.0f, -10.0f, z - 10.0f);
    }
    floorBatch.End();
    //绘制天花板几何图形
    ceilingBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);//三角形条带
    for(z = 60.0f; z >= 0.0f; z -=10.0f)
    {
        ceilingBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        ceilingBatch.Vertex3f(-10.0f, 10.0f, z - 10.0f);
        
        ceilingBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        ceilingBatch.Vertex3f(10.0f, 10.0f, z - 10.0f);
        
        ceilingBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        ceilingBatch.Vertex3f(-10.0f, 10.0f, z);
        
        ceilingBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        ceilingBatch.Vertex3f(10.0f, 10.0f, z);
    }
    ceilingBatch.End();
    //绘制左面墙几何图形
    leftWallBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);//三角形条带
    for(z = 60.0f; z >= 0.0f; z -=10.0f)
    {
        leftWallBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        leftWallBatch.Vertex3f(-10.0f, -10.0f, z);
        
        leftWallBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        leftWallBatch.Vertex3f(-10.0f, 10.0f, z);
        
        leftWallBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        leftWallBatch.Vertex3f(-10.0f, -10.0f, z - 10.0f);
        
        leftWallBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        leftWallBatch.Vertex3f(-10.0f, 10.0f, z - 10.0f);
    }
    leftWallBatch.End();
    
    //绘制右面墙几何图形
    rightWallBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);//三角形条带
    for(z = 60.0f; z >= 0.0f; z -=10.0f)
    {
        rightWallBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        rightWallBatch.Vertex3f(10.0f, -10.0f, z);
        
        rightWallBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        rightWallBatch.Vertex3f(10.0f, 10.0f, z);
        
        rightWallBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        rightWallBatch.Vertex3f(10.0f, -10.0f, z - 10.0f);
        
        rightWallBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        rightWallBatch.Vertex3f(10.0f, 10.0f, z - 10.0f);
    }
    rightWallBatch.End();
}

//退出，删除纹理对象
void ShutdownRC(void)
{
    glDeleteTextures(TEXTURE_COUNT, textures);//退出，删除纹理对象
}
//前后移动视口
void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
        viewZ += 0.5f;
    
    if(key == GLUT_KEY_DOWN)
        viewZ -= 0.5f;
    
    glutPostRedisplay();//重新绘制窗口
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;
    
    if(h == 0)
        h = 1;
    
    glViewport(0, 0, w, h);//设置视口，即显示区域
    
    fAspect = (GLfloat)w/(GLfloat)h;
    
    viewFrustum.SetPerspective(80.0f,fAspect,1.0,120.0); // Set the View Frustum
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

//绘制场景
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//清除一个或一组特定的缓冲区
    
    modelViewMatrix.PushMatrix();
    modelViewMatrix.Translate(0.0f, 0.0f, viewZ);
    
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_FLOOR]);
    floorBatch.Draw();
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_CEILING]);
    ceilingBatch.Draw();
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BRICK]);
    leftWallBatch.Draw();
    rightWallBatch.Draw();
    
    modelViewMatrix.PopMatrix();
    
    glutSwapBuffers();//后台缓冲区交换到前台缓冲区显示
}

int main(int argc, char *argv[])
{
    gltSetWorkingDirectory(argv[0]);
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(800, 600);
    glutCreateWindow("Anisotropic Tunnel");
     //注册回调函数
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    
    //右击鼠标弹出菜单
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("GL_NEAREST",0);
    glutAddMenuEntry("GL_LINEAR",1);
    glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST",2);
    glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", 3);
    glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", 4);
    glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", 5);
    glutAddMenuEntry("Anisotropic Filter", 6);
    glutAddMenuEntry("Anisotropic Off", 7);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    init();
    glutMainLoop();
    ShutdownRC();
    
    return 0;
}






































