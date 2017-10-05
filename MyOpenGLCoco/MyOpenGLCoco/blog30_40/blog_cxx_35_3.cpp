//
//  blog_cxx_35_3.cpp
//  blog_cxx_35_3
//
//  Created by lichong on 2017/10/5.
//  Copyright © 2017年 lichong. All rights reserved.
//

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
CParticle Waterfall;
/** 用来设置粒子的属性值 */
float x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec;
int r,g,b;

/** 载入纹理 */
GLuint  texName[2];
bool LoadTextures()
{
    GLbyte *pBits[2];
    int nWidth, nHeight;
    
    char* fileName[] = {"/Users/lichong/Documents/openGL/code/MyOpenGLCoco/MyOpenGLCoco/tga/wall.bmp","/Users/lichong/Documents/openGL/code/MyOpenGLCoco/MyOpenGLCoco/tga/flare.bmp" };
    
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


/** 初始化粒子 */
bool InitParticle()
{
    for (int i=0; i < Waterfall.GetNumOfParticle(); ++i)
    {
        ///初始化颜色
        //r = rand()%255;
        //g = rand()%255;
        //b = rand()%255;
        r = 50;
        g = 50;
        b = 100;
        
        Waterfall.SetColor(i,r,g,b);
        
        ///初始化坐标
        x = 0.005f * (rand()%9) - 1;
        y = 0.005f * (rand()%9) + 1.6;
        z = 0.0001f * (rand()%15000);
        
        Waterfall.SetPosition(i,x,y,z);
        
        ///初始化速度
        vz = 0;
        vx = 0.000001f*(rand()%9000);
        vy = -rand()%5400 * vx * vx;
        
        Waterfall.SetVelocity(i,vx,vy,vz);
        
        ///初始化加速度
        ax = 0;
        ay = -0.0001;
        az = 0;
        
        Waterfall.SetAcceleration(i,ax,ay,az);
        
        ///初始化生命周期
        lifetime = 100;
        Waterfall.SetLifeTime(i,lifetime);
        
        ///消失速度
        dec = 0.05 * (rand()%50);
        Waterfall.SetDec(i,dec);
        
        ///初始化大小
        Waterfall.SetSize(i,0.02f);
    }
    return true;
}
/** 更新粒子 */
void UpdateParticle()
{
    /** 更新位置 */
    x += vx;
    y += vy;
    
    /** 更新速度 */
    vy += ay;
    
    /** 更新生存时间 */
    lifetime -= dec;
    
    /** 如果粒子消失或生命结束 */
    if (y <= -1 || lifetime <= 0)
    {
        /** 初始化位置 */
        x = 0.005f * (rand()%9) - 1;
        y = 0.005f * (rand()%9) + 1.6;
        z = 0.0001f * (rand()%15000);
        
        /** 初始化速度 */
        vz = 0;
        vx = 0.000001f * (rand()%9000);
        vy = -rand()%500 * vx * vx;
        
        lifetime = 100;
        dec = 0.05 * (rand()%50);
    }
}
/** 绘制粒子 */
void DrawParticle()
{
    /** 绑定纹理 */
    glBindTexture(GL_TEXTURE_2D,texName[1]);
    
    for(int i = 0; i<Waterfall.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        //        Snow.GetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
        Waterfall.GetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
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
        //        UpdateSnow();
        UpdateParticle();
        //        Snow.SetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
        Waterfall.SetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
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
    Waterfall.Create(500);
    
    /** 初始化粒子 */
    //    InitSnow();
    InitParticle();
    
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
    glutCreateWindow("瀑布粒子");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
