//
//  blog_cxx_07.cpp
//  MyOpenGLCoco
//
//  Created by lichong on 2017/8/6.
//  Copyright © 2017年 lichong. All rights reserved.
//
/*
 OpenGL--shader入门
 http://blog.csdn.net/u010223072/article/details/44782049
 
 参考：http://blog.csdn.net/u010053344/article/details/51379222
 */

#include <stdio.h>
#include "GLTools.h"
#include "GLShaderManager.h"
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#endif

GLuint v,f,f2,p;
GLuint loc;

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
void changeSize(int w,int h){
    if (h==0) {
        h=1;
    }
    float ratio=1.0*w/h;
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_PROJECTION);
    //重置当前指定的矩阵为单位矩阵,类似于一个复位操作
    glLoadIdentity();
    //设置视口，即显示区域
    glViewport(0, 0, w, h);
    /*透视投影,参数：角度，宽高比，近平面，远平面*/
    gluPerspective(45, ratio, 1, 1000);
    /**
     GL_MODELVIEW: 把其后的矩阵操作施加于造型视图矩阵栈。（默认）
     GL_PROJECTION: 把其后的矩阵操作施加于投影矩阵栈。
     GL_TEXTURE： 把其后的矩阵操作施加于纹理矩阵栈。
     http://blog.csdn.net/u013720169/article/details/19675181
     */
    glMatrixMode(GL_MODELVIEW);
    
}

float time=0;
void renderScene(void){
     //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //初始化了gl_Color值，可给着色器使用
    glColor4f(1.0, 0.0, 0.0, 1.0);
    //重置当前指定的矩阵为单位矩阵,类似于一个复位操作
    glLoadIdentity();
    //视点转换 参考http://blog.csdn.net/augusdi/article/details/20470813
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //给uniform属性loc所指地址赋值
    glUniform1f(loc,time);
    time+=0.01;
    //绘制茶壶
    glutSolidTeapot(1);
    //后台缓冲区交换到前台缓冲区显示
    glutSwapBuffers();
}

//着色器：自己处理渲染管线中的顶点着色器和片段着色器阶段
void setShaders(){
    char *vs=NULL,*fs=NULL,*fs2=NULL;
    /*第一阶段*/
    //创建着色器对象(类似C中的函数，可以复用)
    //顶点着色器
    v=glCreateShader(GL_VERTEX_SHADER);
    //片段着色器
    f=glCreateShader(GL_FRAGMENT_SHADER);
    
    vs=textFileRead("blog/shader/minimal.vert");
    fs=textFileRead("blog/shader/minimal.frag");
    
    const char *vv=vs;
    const char *ff=fs;
    
    //将着色器代码添加到创建的着色器容器中
    glShaderSource(v,1,&vv,NULL);
    glShaderSource(f,1,&ff,NULL);
    
    free(vs);
    free(fs);
    //编译着色器源代码
    glCompileShader(v);
    glCompileShader(f);
    
    /*第二阶段*/
    //创建着色器程序
    p=glCreateProgram();
    //将着色器对象关联到这个着色器程序
    glAttachShader(p,v);
    glAttachShader(p,f);
    //链接着色器程序
    glLinkProgram(p);
    //使用着色器
    glUseProgram(p);
    //得到着色器程序中unifrom属性的time变量地址，以便外面修改它
    loc=glGetUniformLocation(p,"time");
}

int main(int argc,char **argv){
    //初始化GLUT库
    glutInit(&argc,argv);
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    //窗口位置
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("OpenGL Shader 入门");
    //注册回调函数 渲染
    glutDisplayFunc(renderScene);
    //消息循环处于空闲的时候响应
    glutIdleFunc(renderScene);
    //注册回调函数 窗口改变
    glutReshapeFunc(changeSize);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    // 为色彩缓冲区指定用于清除的值
    glClearColor(1.0,1.0,1.0,1.0);
    //glEnable(GL_CULL_FACE);
    //渲染环境检测
    glewInit();
    setShaders();
    //循环
    glutMainLoop();
    return 0;
}






















