//
//  Move.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/29.
//  Copyright © 2017年 lichong. All rights reserved.
//

#include <stdio.h>
#include <GLTools.h>
#include <GLShaderManager.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

GLBatch squareBatch;
GLShaderManager shaderManager;

GLfloat blockSize=0.1f;
//顶点坐标
GLfloat vVerts[] = { -blockSize - 0.5f, -blockSize, 0.0f,
    blockSize - 0.5f, -blockSize, 0.0f,
    blockSize - 0.5f,  blockSize, 0.0f,
    -blockSize - 0.5f,  blockSize, 0.0f};

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC(){
    //设置背影颜色
    glClearColor(0.0f,0.0f,1.0f,1.0f);
    //初始化着色管理器
    shaderManager.InitializeStockShaders();
    //批次处理 参数4代表顶点个数
    squareBatch.Begin(GL_TRIANGLE_FAN, 4);
    squareBatch.CopyVertexData3f(vVerts);
    squareBatch.End();
    
}
// Respond to arrow keys by moving the camera frame of reference
void BounceFunction(void)
{
    static GLfloat xDir = 1.0f;
    static GLfloat yDir = 1.0f;
    
    GLfloat stepSize = 0.005f;
    
    GLfloat blockX = vVerts[0];   // Upper left X
    GLfloat blockY = vVerts[7];  // Upper left Y
    
    blockY += stepSize * yDir;
    blockX += stepSize * xDir;
    
    // Collision detection
    if(blockX < -1.0f) { blockX = -1.0f; xDir *= -1.0f; }
    if(blockX > (1.0f - blockSize * 2)) { blockX = 1.0f - blockSize * 2; xDir *= -1.0f; }
    if(blockY < -1.0f + blockSize * 2)  { blockY = -1.0f + blockSize * 2; yDir *= -1.0f; }
    if(blockY > 1.0f) { blockY = 1.0f; yDir *= -1.0f; }
    
    // Recalculate vertex positions
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize*2;
    
    vVerts[3] = blockX + blockSize*2;
    vVerts[4] = blockY - blockSize*2;
    
    vVerts[6] = blockX + blockSize*2;
    vVerts[7] = blockY;
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    
    squareBatch.CopyVertexData3f(vVerts);
}
///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //设置一组浮点数来表示红色
    GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    //提交着色器
    squareBatch.Draw();
    //将在后台缓冲区进行渲染，然后在结束时交换到前台
    glutSwapBuffers();
    
    BounceFunction();
    glutPostRedisplay(); // Redraw
}
///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc,char * argv[]) {
    //设置当前工作目录，针对MAC OS X
    gltSetWorkingDirectory(argv[0]);
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bouncing Block");
    //驱动程序的初始化中没有出现任何问题。
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    //注册回调函数
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    //调用SetupRC
    SetupRC();
    
    glutMainLoop();
    return 0;
}
