//
//  blog_cxx_21.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/21.
//  Copyright © 2017年 lichong. All rights reserved.
//
/**
 
 环境映射
 三维场景中的物体不仅受光照影响，而且受周围环境的影响，如金属，水面等材质都可以映射出周围环境的图像。模拟物体光滑表面能够映射出周围环境的技术叫做环境映射(也称反射映射)。
 其原理是通过立方体纹理(cube)实现的，具体是：把摄像机放在反射物体旁边，分别朝6个方向照得6张纹理，组成立方体纹理。然后就是利用这个立方体纹理通过一些复杂的数学计算，把反射纹理映射到我们的光亮物体表面，从而形成物体反射周围环境的效果。其本质还是纹理贴图，只是纹理来源于周围环境
 http://blog.csdn.net/u010223072/article/details/45193283
 
 */


#include <GLTools.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

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


//立方体6个面纹理:其实就是摄像机在光亮物体附件分别朝6个方向照下的纹理，这里直接手动指定。
const char *szCubeFaces[6] = {
    "/Users/lichong/Documents/openGL/code/MyOpenGL/MyOpenGLCoco/MyOpenGLCoco/tga/pos_x.tga",
    "/Users/lichong/Documents/openGL/code/MyOpenGL/MyOpenGLCoco/MyOpenGLCoco/tga/neg_x.tga",
    "/Users/lichong/Documents/openGL/code/MyOpenGL/MyOpenGLCoco/MyOpenGLCoco/tga/pos_y.tga",
    "/Users/lichong/Documents/openGL/code/MyOpenGL/MyOpenGLCoco/MyOpenGLCoco/tga/neg_y.tga",
    "/Users/lichong/Documents/openGL/code/MyOpenGL/MyOpenGLCoco/MyOpenGLCoco/tga/pos_z.tga",
    "/Users/lichong/Documents/openGL/code/MyOpenGL/MyOpenGLCoco/MyOpenGLCoco/tga/neg_z.tga" };

//立方体6个面
GLenum  cube[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
GLuint              cubeTexture;
void init()
{
    GLbyte *pBytes;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
    int i;
    
    glCullFace(GL_BACK);//剔除背面
    glFrontFace(GL_CCW);//设置逆时针方向为正面
    glEnable(GL_DEPTH_TEST);//开启深度测试
    
    glGenTextures(1, &cubeTexture);//生成1个未使用的纹理对象标志
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);//创建立方体纹理对象
    
    //设置立方体纹理过滤方式
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    /*设置像素的储存模式，参数1包括：GL_PACK_ALIGNMENT：打包
     与GL_UNPACK_ALIGNMENT：解包两种。参数2是对齐方式，如这里
     是取字母，一个字母占1字节，所以以1字节对齐*/
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for(i = 0; i < 6; i++)
    {
        pBytes = gltReadTGABits(szCubeFaces[i], &iWidth, &iHeight, &iComponents, &eFormat);
        //指定立方体6个面纹理
        glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);
    }
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);//为立方体每个面生成多级纹理
}

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除一个或一组特定的缓冲区
    
    glEnable(GL_TEXTURE_CUBE_MAP_ARB);//激活立方体纹理
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeTexture);//指定当前立方体纹理为活动纹理
    
    //用立方体纹理绘制场景
    glBegin(GL_QUADS);
    glTexCoord3f(1.0f, 1.0f, -1.0f); glVertex3f(12.0f, 12.0f, -20.0f);
    glTexCoord3f(-1.0f, 1.0f, -1.0f); glVertex3f(-12.0f, 12.0f, -20.0f);
    glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-12.0f, -12.0f, -20.0f);
    glTexCoord3f(1.0f, -1.0f, -1.0f); glVertex3f(12.0f, -12.0f, -20.0f);
    glEnd();
    
    //自动生成立方体纹理坐标
    glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);//反射
    glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
    glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glutSolidSphere (2.0, 30, 30);//绘制球体，立方体纹理贴到球体上
    
    glutSwapBuffers();//后台缓冲区交换到前台缓冲区显示
}

void ChangeSize(int w, int h)
{
    if(h == 0)
        h = 1;
    
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);//设置视口，即显示区域
    glMatrixMode(GL_PROJECTION);// GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈
    glLoadIdentity();
    gluPerspective(35.0, (GLfloat) w/(GLfloat) h, 1.0, 1000.0f);/*透视投影,参数：角度，宽高比，近平面，远平面*/
    glMatrixMode(GL_MODELVIEW);//GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
    glLoadIdentity();
    
    glTranslatef(0.0f, 0.0f, -15.0f);//平移
}
//退出时，删除纹理
void ShutdownRC(void)
{
    glDeleteTextures(1, &cubeTexture);
}

int main(int argc, char* argv[])
{
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(500,500);
    glutCreateWindow("OpenGL Cube Maps");
    //注册回调函数
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
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







































