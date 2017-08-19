//
//  Chapter3_8_Primitives.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/30.
//  Copyright © 2017年 lichong. All rights reserved.
//

// Main entry point for GLUT based programs

/**
 实例程序：三角形
 http://blog.csdn.net/u010223072/article/details/44309113
 */

#include <GLTools.h>
#include "GLShaderManager.h"

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
/////////////////////////////////////////////////////////////////////////////////
// An assortment of needed classes
GLBatch triangleBatch;
GLShaderManager		shaderManager;

void ChangeSize(int w,int h)
{
    
    glViewport(0, 0, w, h);//设置视口，即显示区域
}
void SetupRC()
{
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);// 为色彩缓冲区指定用于清除的值
    shaderManager.InitializeStockShaders();
    GLfloat vVerts[] = {
        -0.5f,0.0f,0.0f,
        0.5f,0.0f,0.0f,
        0.0f,0.5f,0.0f,
    };
    triangleBatch.Begin(GL_TRIANGLES,3);//绘制三角形
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}
void RenderScene(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//清除一个或一组特定的缓冲区
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    triangleBatch.Draw();
    //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    //设置当前工作目录，针对MAC OS X
    gltSetWorkingDirectory(argv[0]);
    //初始化GLUT库
    glutInit(&argc, argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle");
    //注册回调函数 窗口改变
    glutReshapeFunc(ChangeSize);
    //注册回调函数 渲染
    glutDisplayFunc(RenderScene);
    //渲染环境检测
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
//    
//    
    SetupRC();
//消息循环
    glutMainLoop();
    return 0;
}
