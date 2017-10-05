//
//  blog_cxx_36.cpp
//  blog_cxx_36
//
//  Created by lichong on 2017/10/6.
//  Copyright © 2017年 lichong. All rights reserved.
//

#include <stdio.h>
#include "GLTools.h"
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include "Bone.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#define MAX_BONES                2   /**骨骼数*/
#define MAX_MESHES              3   /**网格数，即基本图元数(纹理贴图后的皮肤)*/
#define MAX_MESH_POINTS    4   /**一个网格顶点数，即四边形*/

CVector4         trans;                       /**< 用于平移骨骼模型 */
CBone             bones[MAX_BONES];            /**< 保存骨骼结构 */
CMatrix4x4     displayBones[MAX_BONES];     /**< 用于绘制骨骼 */
CBoneVertex   modelPoints[MAX_MESHES * MAX_MESH_POINTS]; /**< 模型顶点数据，即皮肤顶点 */
void init(void)
{
    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    /** 构造骨骼 */
    bones[0].SetBone(-1, 4.5f);/**根关节(骨骼索引与长度)*/
    bones[1].SetBone(0, 4.5f);/**子关节*/
    
    /** 绑定到骨骼上的皮肤顶点数据 */
    modelPoints[0].SetVertex(CVector4(-1.0f, 0.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1);
    modelPoints[1].SetVertex(CVector4(1.0f, 0.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1);
    modelPoints[2].SetVertex(CVector4(1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2);
    modelPoints[3].SetVertex(CVector4(-1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2);
    
    modelPoints[4].SetVertex(CVector4(-1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);
    modelPoints[5].SetVertex(CVector4(1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);
    modelPoints[6].SetVertex(CVector4(1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);
    modelPoints[7].SetVertex(CVector4(-1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);
    
    modelPoints[8].SetVertex(CVector4(-1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 2);
    modelPoints[9].SetVertex(CVector4(1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 2);
    modelPoints[10].SetVertex(CVector4(1.0f, 9.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                              1, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1);
    modelPoints[11].SetVertex(CVector4(-1.0f, 9.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                              1, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0f, 0.0f, 1.0f, 1);
    
}

GLint xRotation = 0;
GLint yRotation = 0;
/** 骨骼更新，即计算出每个骨骼的新变换矩阵，用displayBones数组保存 */
void updateBones()
{
    /** 用于保存旋转和平移矩阵 */
    CMatrix4x4 transpose, rotTemp1, rotTemp2, tempRelative;
    
    /** 循环更新骨骼 */
    for(int i = 0; i < MAX_BONES; i++)
    {
        ///检查是否是根骨骼
        if(bones[i].parent == -1)
        {
            /** 设置变换矩阵 */
            bones[i].relative.Translate(trans.x, trans.y, trans.z);
            bones[i].absolute = bones[i].relative;
            
            /** 赋值，为了显示骨骼 */
            displayBones[i] = bones[i].relative;
        }
        else
        {
            bones[i].relative.Clear();
            
            /** 移动该骨骼到父结点骨骼(绕父骨骼转) */
            bones[i].relative.Translate(0, bones[bones[i].parent].length * i, 0);
            
            /** 根据鼠标旋转 */
            rotTemp1.Rotate(xRotation, 0, 1, 0);
            rotTemp2.Rotate(yRotation, 0, 0, 1);
            
            /** 保存相对变换矩阵，并反转 */
            tempRelative = bones[i].relative;
            tempRelative.inverseTranslate();
            
            /** 保存变换，为了显示骨骼 */
            displayBones[i] = bones[bones[i].parent].absolute * bones[i].relative *
            (rotTemp1 * rotTemp2);
            
            /** 计算相对变换矩阵 */
            bones[i].relative = bones[i].relative * (rotTemp1 * rotTemp2) * tempRelative;
            
            /** 计算绝对变换矩阵 */
            bones[i].absolute = bones[bones[i].parent].absolute * bones[i].relative;
        }
    }
}

void display(void)
{
    /** 用户自定义的绘制过程 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    
    glTranslatef(0.0f, -4.0f, -15.0f);
    
    /** 更新骨骼列表 */
    updateBones();
    
    CMatrix4x4 mat;
    
    /** 渲染网格中的顶点，即绘制皮肤 */
    for(int j = 0; j < MAX_MESHES; j++)
    {
        glBegin(GL_QUADS);
        
        for(int i = 0; i < MAX_MESH_POINTS; i++)
        {
            ///获得顶点指针
            CBoneVertex *thisVertex = &modelPoints[i + j * MAX_MESH_POINTS];
            
            ///根据权值计算顶点
            CVector4 pos = CVector4(0.0, 0.0, 0.0, 0.0);
            CVector4 normal = CVector4(0.0, 0.0, 0.0);
            
            for(int i2 = 0; i2 < thisVertex->numBones; i2++)
            {
                mat = bones[thisVertex->boneIndex[i2]].absolute;
                
                ///加权骨骼对顶点位置和法线的影响
                pos += (mat.VectorMatrixMultiply(thisVertex->pos) * thisVertex->weights[i2]);
                normal += (mat.VectorMatrixMultiply3x3(thisVertex->normal) *
                           thisVertex->weights[i2]);
            }
            
            ///渲染该顶点
            glColor4f(thisVertex->r, thisVertex->g, thisVertex->b, thisVertex->a);
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(pos.x, pos.y, pos.z);
        }
        
        glEnd();
    }
    
    //绘制骨骼
    for(int i = 0; i < MAX_BONES; i++)
    {
        glPushMatrix();
        
        glMultMatrixf(displayBones[i].matrix);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        
        glBegin(GL_LINES);
        
        ///绘制线段
        glVertex3f(-0.4f, 0.0f, -0.4f);
        glVertex3f(0.4f, 0.0f, -0.4f);
        glVertex3f(0.4f, 0.0f, -0.4f);
        glVertex3f(0.4f, 0.0f, 0.4f);
        glVertex3f(0.4f, 0.0f, 0.4f);
        glVertex3f(-0.4f, 0.0f, 0.4f);
        glVertex3f(-0.4f, 0.0f, 0.4f);
        glVertex3f(-0.4f, 0.0f, -0.4f);
        
        glVertex3f(-0.4f, 0.0f, -0.4f);
        glVertex3f(0.0f, bones[i].length, 0.0f);
        glVertex3f(0.4f, 0.0f, -0.4f);
        glVertex3f(0.0f, bones[i].length, 0.0f);
        glVertex3f(0.4f, 0.0f, 0.4f);
        glVertex3f(0.0f, bones[i].length, 0.0f);
        glVertex3f(-0.4f, 0.0f, 0.4f);
        glVertex3f(0.0f, bones[i].length, 0.0f);
        
        glEnd();
        
        glPopMatrix();
    }
    
    /** 强制执行所有的OpenGL命令 */
    glFlush();
}

void ChangeSize(int width, int height)
{
    glViewport(0, 0, width, height);                                    /**< 重新设置视口 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1700.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int oldx = 0;
int oldy = 0;
void motion(int x, int y)
{
    xRotation = xRotation - (x - oldx);
    yRotation = yRotation - (y - oldy);
    
    oldx = x;
    oldy = y;
    
    glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB );
    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("骨骼动画");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
