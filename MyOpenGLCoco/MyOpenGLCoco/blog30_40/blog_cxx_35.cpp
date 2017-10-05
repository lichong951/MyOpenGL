//
//  blog_cxx_35.cpp
//  blog_cxx_35
//
//  Created by lichong on 2017/10/5.
//  Copyright © 2017年 lichong. All rights reserved.
//OpenGL--粒子系统 雪花粒子
/*
 理论基础
 1，粒子系统的基本思想是用许多形状简单且赋予生命的微小粒子作为基本元素来表示物体(一般由点或很小的多边形通过纹理贴图表示)，侧重于物体的总体形态和特征的动态变化。把物体定义为许多不规则，随机分布的粒子，且每个粒子均有一定的生命周期。随着时间的推移，旧的粒子不断消失(死亡)，新的粒子不断加入(生长)。粒子的这种出生，成长，衰老和死亡的过程，能够较好地反映模糊物体的动态特性。因此它比起传统的帧动画，效果要更加逼真而且节省资源。但其计算量较大，不适合大范围的特效(卡)，一般用于较小场景。
 2，粒子系统实现的一般步骤：一，初始化，即创建很多粒子对象同时赋予初始属性。二，循环运行，渲染粒子<–>更新粒子(如果有死亡就把其初始化为起始属性)。总的原理就是随机改变粒子的属性，然后渲染，这样循环。所谓的粒子编辑器无非就是在操作改变粒子属性罢了。
 http://blog.csdn.net/u010223072/article/details/45888397
 **/

#include <stdio.h>
//说明：就是根据粒子结构给其设置与得到对应属性的值（这个类只是给粒子设置属性）

#include "GLTools.h"
#include "Particle.hpp"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

/** 创建一个粒子类对象 */
CParticle Snow;

/** 用来设置粒子的属性值 */
float x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec;
int r,g,b;

/** 载入纹理 */
GLuint  texName[2];
bool LoadTextures()
{
    GLbyte *pBits[2];
    int nWidth, nHeight;
    
    char* fileName[] = {"/Users/lichong/Documents/openGL/code/MyOpenGLCoco/MyOpenGLCoco/tga/wall.bmp","/Users/lichong/Documents/openGL/code/MyOpenGLCoco/MyOpenGLCoco/tga/snowball.bmp" };
    
    for(int i=0; i<2; i++)
    {
        pBits[i] = gltReadBMPBits(fileName[i], &nWidth, &nHeight );
        if(pBits[i] == NULL)
        {
            printf("bmp load failed");
            exit(-1);
        }
        
        glGenTextures(1,&texName[i]);
        
        /** 创建纹理对象 */
        glBindTexture(GL_TEXTURE_2D, texName[i]);
        
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nWidth, nHeight,
                          GL_RGB, GL_UNSIGNED_BYTE,pBits[i]);
    }
    return true;
}


/** 绘制地面 */
float angle = 0;
void DrawGround()
{
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-6.0f);
    glRotatef(angle,0.0f,1.0f,0.0f);
    
    /** 指定纹理 */
    glBindTexture(GL_TEXTURE_2D,texName[0]);
    glColor4ub(255,255,255,255);
    glNormal3f(0.0f,1.0f,0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f);glVertex3f(-2.5f,-1.0f,2.5f);
    glTexCoord2f(1.0f,0.0f);glVertex3f(-2.5f,-1.0f,-2.5f);
    glTexCoord2f(1.0f,1.0f);glVertex3f(2.5f,-1.0f,-2.5f);
    glTexCoord2f(0.0f,1.0f);glVertex3f(2.5f,-1.0f,2.5f);
    glEnd();
    glPopMatrix();
    
    /** 地面转动的角度 */
    angle += 0.02f;
}

/** 初始化雪花粒子 */
bool InitSnow()
{
    for (int i=0; i < Snow.GetNumOfParticle(); ++i)
    {
        ///初始化颜色（白色）
        r = 255;
        g = 255;
        b = 255;
        Snow.SetColor(i,r,g,b);
        
        ///初始化坐标
        x = 0.1f * (rand() % 50) - 2.5f;
        y = 2 + 0.1f * (rand() % 2);
        if((int)x % 2 == 0)
            z = rand()%6;
        else
            x = -rand()%3;
        Snow.SetPosition(i,x,y,z);
        
        ///初始化速度
        vx = 0.00001 * (rand()%100);
        vy = 0.0000002 * (rand()%28000);
        vz = 0;
        Snow.SetVelocity(i,vx,vy,vz);
        
        ///初始化加速度
        ax = 0;
        ay = 0.000005f;
        az = 0;
        Snow.SetAcceleration(i,ax,ay,az);
        
        ///初始化生命周期
        lifetime = 100;
        Snow.SetLifeTime(i,lifetime);
        
        ///消失速度
        dec = 0.005 * (rand()%50);
        Snow.SetDec(i,dec);
        
        ///初始化大小
        Snow.SetSize(i,0.03f);
    }
    return true;
}

/** 更新粒子 */
void UpdateSnow()
{
    /** 更新位置 */
    x += (vx * 5);
    y -= vy;
    
    /** 更新速度 */
    vy += ay;
    
    /** 更新生存时间 */
    lifetime -= dec;
    
    if(x > 3)
        x = -2;
    
    /** 如果粒子消失或生命结束 */
    if (y <= -1 || lifetime <= 0)
    {
        /** 初始化位置 */
        x = 0.1f * (rand()%50) - 2.5f;
        y = 2 + 0.1f * (rand()%2);
        if((int)x%2 == 0)
            z = rand()%6;
        else
            z = -rand()%3;
        
        /** 初始化速度 */
        vx = (float)(0.00001 * (rand()%100));
        vy = (float)(0.0000002 * (rand()%28000));
        vz = 0;
        
        /** 初始化加速度 */
        ax = 0;
        ay = 0.000005f;
        az = 0;
        lifetime = 100;
        dec = 0.005*(rand()%50);
    }
}

/** 绘制粒子 */
void DrawParticle()
{
    /** 绑定纹理 */
    glBindTexture(GL_TEXTURE_2D,texName[1]);
    
    for(int i = 0; i<Snow.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        Snow.GetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
        glLoadIdentity();
        glTranslatef(0.0f,0.0f,-6.0f);
        glColor4ub(r,g,b,255);
        glNormal3f(0.0f,0.0f,1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);glVertex3f(x-size,y-size,z);
        glTexCoord2f(1.0f,0.0f);glVertex3f(x-size,y+size,z);
        glTexCoord2f(1.0f,1.0f);glVertex3f(x+size,y+size,z);
        glTexCoord2f(0.0f,1.0f);glVertex3f(x+size,y-size,z);
        glEnd();
        
        /** 更新粒子属性 */
        UpdateSnow();
        Snow.SetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
    }
    glutPostRedisplay();//发送渲染请求
}

/////////////////////////////////////////////////////////////////
/** 初始化 */
void init(void)
{
    /** 用户自定义的初始化过程 */
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);//指定深度缓冲区的清除值(即将深度缓冲区里的值设置为1)
    glDepthFunc(GL_LEQUAL);//指定用于深度缓冲比较值(即新进像素深度值与原来的1比较，<=则通过，否则丢弃)
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glEnable(GL_TEXTURE_2D);             /**< 开启纹理映射 */
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);    /**< 设置混合因子获得半透明效果 */
    glEnable(GL_BLEND);                  /**< 启用混和 */
    
    /**  载入纹理 */
    if(!LoadTextures())
    {
        printf("bmp load failed");
        exit(-1);
    }
    
    /** 创建500个粒子 */
    Snow.Create(500);
    
    /** 初始化粒子 */
    InitSnow();
}

/** 渲染 */
void display(void)
{
    /** 用户自定义的绘制过程 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    /** 绘制地面 */
    DrawGround();
    
    /** 绘制粒子 */
    DrawParticle();
    
    glFlush(); /**<强制执行所有的OpenGL命令 */
}


void ChangeSize(int width, int height)
{
    glViewport(0, 0, width, height);    /**< 重新设置视口 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1700.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB );
    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("雪花粒子");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
