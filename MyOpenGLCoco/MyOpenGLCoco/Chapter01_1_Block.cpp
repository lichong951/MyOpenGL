//
//  Chapter01_1_Block.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/7/29.
//  Copyright © 2017年 lichong. All rights reserved.
//

#include <stdio.h>
#include <GLTools.h>	// OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLGeometryTransform.h>

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

/////////////////////////////////////////////////////////////////////////////////
// An assortment of needed classes
//着色器管理器
GLShaderManager		shaderManager;
GLMatrixStack		modelViewMatrix;
GLMatrixStack		projectionMatrix;
GLFrame				cameraFrame;
GLFrustum			viewFrustum;
GLBatch				cubeBatch;
GLBatch				floorBatch;
GLBatch				topBlock;
GLBatch				frontBlock;
GLBatch				leftBlock;

GLGeometryTransform	transformPipeline;
M3DMatrix44f		shadowMatrix;

// Keep track of effects step
int nStep = 0;

// Lighting data
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat lightSpecular[] = { 0.9f, 0.9f, 0.9f };
GLfloat vLightPos[] = { -8.0f, 20.0f, 100.0f, 1.0f };

GLuint textures[4];
////文件读取(将本地shader文件读取为字符串形式加载)
char *textFileRead(char *fn)
{
    FILE *fp;
    char *content = NULL;
    
    long count=0;
    
    if (fn != NULL)
    {
        fp = fopen(fn,"rt");
        
        if (fp != NULL)
        {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            
            if (count > 0)
            {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}
///////////////////////////////////////////////////////////////////////////////
// Make a cube out of a batch of triangles. Texture coordinates and normals
// are also provided.
void MakeCube(GLBatch& cubeBatch)
{
    cubeBatch.Begin(GL_TRIANGLES, 36, 1);//绘制三角形
    
    /////////////////////////////////////////////
    // Top of cube
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    
    ////////////////////////////////////////////
    // Bottom of cube
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    ///////////////////////////////////////////
    // Left side of cube
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    // Right side of cube
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    // Front and Back
    // Front
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    // Back
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    cubeBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    cubeBatch.Vertex3f(-1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    cubeBatch.Vertex3f(1.0f, 1.0f, -1.0f);
    
    cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
    cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    cubeBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    cubeBatch.End();
}

/////////////////////////////////////////////////////////////////////////////
// Make the floor, just the verts and texture coordinates, no normals
void MakeFloor(GLBatch& floorBatch)
{
    GLfloat x = 5.0f;
    GLfloat y = -1.0f;
    
    floorBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
    floorBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    floorBatch.Vertex3f(-x, y, x);
    
    floorBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    floorBatch.Vertex3f(x, y, x);
    
    floorBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    floorBatch.Vertex3f(x, y, -x);
    
    floorBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    floorBatch.Vertex3f(-x, y, -x);
    floorBatch.End();
}

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    GLbyte *pBytes;
    GLint nWidth, nHeight, nComponents;
    GLenum format;
    //初始化着色器管理器
    shaderManager.InitializeStockShaders();
    
    // 为色彩缓冲区指定用于清除的值
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2.5f);
    //设置源因子与目标因子取值
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
    cameraFrame.MoveForward(-15.0f);
    cameraFrame.MoveUp(6.0f);
    cameraFrame.RotateLocalX(float(m3dDegToRad(20.0f)));
    
    MakeCube(cubeBatch);
    MakeFloor(floorBatch);
    
    // Make top
    topBlock.Begin(GL_TRIANGLE_FAN, 4, 1);
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 0.0f, 0.0f);
    topBlock.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 1.0f, 0.0f);
    topBlock.Vertex3f(1.0f, 1.0f, 1.0f);
    
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 1.0f, 1.0f);
    topBlock.Vertex3f(1.0f, 1.0f, -1.0f);
    
    topBlock.Normal3f(0.0f, 1.0f, 0.0f);
    topBlock.MultiTexCoord2f(0, 0.0f, 1.0f);
    topBlock.Vertex3f(-1.0f, 1.0f, -1.0f);
    topBlock.End();
    
    // Make Front
    frontBlock.Begin(GL_TRIANGLE_FAN, 4, 1);
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 0.0f, 0.0f);
    frontBlock.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 1.0f, 0.0f);
    frontBlock.Vertex3f(1.0f, -1.0f, 1.0f);
    
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 1.0f, 1.0f);
    frontBlock.Vertex3f(1.0f, 1.0f, 1.0f);
    
    frontBlock.Normal3f(0.0f, 0.0f, 1.0f);
    frontBlock.MultiTexCoord2f(0, 0.0f, 1.0f);
    frontBlock.Vertex3f(-1.0f, 1.0f, 1.0f);
    frontBlock.End();
    
    // Make left
    leftBlock.Begin(GL_TRIANGLE_FAN, 4, 1);
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 0.0f, 0.0f);
    leftBlock.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 1.0f, 0.0f);
    leftBlock.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 1.0f, 1.0f);
    leftBlock.Vertex3f(-1.0f, 1.0f, 1.0f);
    
    leftBlock.Normal3f(-1.0f, 0.0f, 0.0f);
    leftBlock.MultiTexCoord2f(0, 0.0f, 1.0f);
    leftBlock.Vertex3f(-1.0f, 1.0f, -1.0f);
    leftBlock.End();
    
    // Create shadow projection matrix
    GLfloat floorPlane[] = { 0.0f, 1.0f, 0.0f, 1.0f};
    m3dMakePlanarShadowMatrix(shadowMatrix, floorPlane, vLightPos);
    
    // Load up four textures
    glGenTextures(4, textures);
    char *vs=NULL;
    vs=textFileRead("blog/shader/minimal.vert");
    // Wood floor
    pBytes = gltReadTGABits("tga/floor.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
    
    // One of the block faces
    
    pBytes = gltReadTGABits("tga/Block4.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
    
    // Another block face
    pBytes = gltReadTGABits("tga/block5.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
    
    // Yet another block face
    pBytes = gltReadTGABits("tga/block6.tga", &nWidth, &nHeight, &nComponents, &format);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
                 format, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
}



///////////////////////////////////////////////////////////////////////////////
// Render the block
void RenderBlock(void)
{
    GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat vWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    switch(nStep)
    {
            // Wire frame
        case 0:
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vRed);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            
            // Draw the cube
            cubeBatch.Draw();
            
            break;
            
            // Wire frame, but not the back side... we also want the block to be in the stencil buffer
        case 1:
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vRed);
            
            // Draw solid block in stencil buffer
            // Back face culling prevents the back sides from showing through
            // The stencil pattern is used to mask when we draw the floor under it
            // to keep it from showing through.
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_NEVER, 0, 0);
            glStencilOp(GL_INCR, GL_INCR, GL_INCR);
            cubeBatch.Draw();
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glDisable(GL_STENCIL_TEST);
            
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
            // Draw the front side cube
            cubeBatch.Draw();
            break;
            
            // Solid
        case 2:
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vRed);
            
            // Draw the cube
            cubeBatch.Draw();
            break;
            
            // Lit
        case 3:
            shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                         projectionMatrix.GetMatrix(), vLightPos, vRed);
            
            // Draw the cube
            cubeBatch.Draw();
            break;
            
            // Textured & Lit
        case 4:
        case 5:
        default:
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                         projectionMatrix.GetMatrix(), vLightPos, vWhite, 0);
            
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            topBlock.Draw();
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            frontBlock.Draw();
            glBindTexture(GL_TEXTURE_2D, textures[3]);
            leftBlock.Draw();
            
            break;
    }
    
    // Put everything back
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_STENCIL_TEST);
}
///////////////////////////////////////////////////////////////////////////////
// Render the floor
void RenderFloor(void)
{
    GLfloat vBrown [] = { 0.55f, 0.292f, 0.09f, 1.0f};
    GLfloat vFloor[] = { 1.0f, 1.0f, 1.0f, 0.6f };
    
    switch(nStep)
    {
            // Wire frame
        case 0:
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBrown);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            break;
            
            // Wire frame, but not the back side.. and only where stencil == 0
        case 1:
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, 0, 0xff);
            
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBrown);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
            
            // Solid
        case 2:
        case 3:
            shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBrown);
            break;
            
            // Textured
        case 4:
        case 5:
        default:
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vFloor, 0);
            break;
    }
    
    // Draw the floor
    floorBatch.Draw();
    
    // Put everything back
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_STENCIL_TEST);
}
///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    modelViewMatrix.PushMatrix();
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.MultMatrix(mCamera);
    
    // Reflection step... draw cube upside down, the floor
    // blended on top of it
    if(nStep == 5) {
        glDisable(GL_CULL_FACE);
        modelViewMatrix.PushMatrix();
        modelViewMatrix.Scale(1.0f, -1.0f, 1.0f);
        modelViewMatrix.Translate(0.0f, 2.0f, 0.0f);
        modelViewMatrix.Rotate(35.0f, 0.0f, 1.0f, 0.0f);
        RenderBlock();
        modelViewMatrix.PopMatrix();
        glEnable(GL_BLEND);
        //设置源因子与目标因子取值
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        RenderFloor();
        glDisable(GL_BLEND);
    }
    
    
    modelViewMatrix.PushMatrix();
    
    // Draw normally
    modelViewMatrix.Rotate(35.0f, 0.0f, 1.0f, 0.0f);
    RenderBlock();
    modelViewMatrix.PopMatrix();
    
    
    // If not the reflection pass, draw floor last
    if(nStep != 5)
        RenderFloor();
    
    
    modelViewMatrix.PopMatrix();
    
    
   //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
}


///////////////////////////////////////////////////////////////////////////////
// A normal ASCII key has been pressed.
// In this case, advance the scene when the space bar is pressed
void KeyPressFunc(unsigned char key, int x, int y)
{
    if(key == 32)
    {
        nStep++;
        
        if(nStep > 5)
            nStep = 0;
    }
    
    // Refresh the Window
    glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
    //设置视口，即显示区域
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    modelViewMatrix.LoadIdentity();
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc,char * argv[]) {
    //设置当前工作目录，针对MAC OS X
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Effects Demo");
    
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPressFunc);
    glutDisplayFunc(RenderScene);
    
    SetupRC();
    
    glutMainLoop();
    glDeleteTextures(4,textures);
    return 0;
}
