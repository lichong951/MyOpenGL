//
//  blog_cxx_34.cpp
//  blog_cxx_34
//
//  Created by lichong on 2017/10/5.
//  Copyright © 2017年 lichong. All rights reserved.
//选择和反馈
/*
 理论基础
 选择和反馈：整的来说就是鼠标点击，然后算出到底点击的是哪个物体。在选择和反馈模式下，绘制信息返回给应用程序而不是像在渲染模式中那样送往帧缓冲，在这种模式下，屏幕将被冻结，没有图形出现。实现的一般步骤是：1，创建选择缓存，用于记录返回的点中物体信息 2，调用glRenderMode(GL_SELECT)进入选择模式 3，用glInitName()，glPushName()等来初始化名字栈 4，定义视景体 5，绘图同时设置对应名字 6，退出选择模式并处理返回的点中物体数据(glRenderMode(GL_RENDER))。
 注释：
 1，本节内容在OpenGL 3.1以后已经废除了，当初OpenGL其实也就是想让硬件做，达到加速目的。但现在其实都是软件做，而且性能更高。因为软件做的话，有八叉树之类的，可以先剔除一大部分物体。
 2，OpenGL中的选择和反馈是与普通渲染方式不同的一种特殊的渲染方式。我们使用时一般是先在帧缓存中渲染普通场景,然后进入选择模式重绘场景,此时帧缓存的内容并无变化。也就是说,为了选择某些物体,我们需要在一帧中使用不同的渲染方式将其渲染两遍。我们知道对对象进行渲染是比较耗时的操作,当场景中需要选择的对象多而杂的时候,采用这个机制是非常影响速度的。
 
 http://blog.csdn.net/u010223072/article/details/45369075
 **/

#include <stdio.h>
#include "GLTools.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

int board[3][3]; //记录方块对应颜色值

/* 清除每个方块颜色值 */
void init(void)
{
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j ++)
            board[i][j] = 0;
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

/*绘制方块，用2个名字(row, col) 标志方块，这时堆栈深度就只有2，
 如果只用一个名字标志物体，那么对应堆栈大小就是1，即每次替换栈顶数据*/
void drawSquares(GLenum mode)
{
    GLuint i, j;
    for (i = 0; i < 3; i++) {
        if (mode == GL_SELECT)
            glLoadName (i);//替换栈顶数据，作为第一个名字
        for (j = 0; j < 3; j ++) {
            if (mode == GL_SELECT)
                glPushName (j);//压入第二个名字
            glColor3f ((GLfloat) i/3.0, (GLfloat) j/3.0,
                       (GLfloat) board[i][j]/3.0);
            glRecti (i, j, i+1, j+1);
            if (mode == GL_SELECT)
                glPopName ();//弹出栈顶名字，堆栈大小恢复为1
        }
    }
}

/*打印选择信息*/
void processHits (GLint hits, GLuint buffer[])
{
    unsigned int i, j;
    GLuint ii, jj, names, *ptr;
    
    printf ("hits = %d\n", hits);
    ptr = (GLuint *) buffer;
    for (i = 0; i < hits; i++) {    /*  for each hit  */
        names = *ptr;
        printf (" number of names for this hit = %d\n", names); ptr++;
        printf("  z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
        printf(" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
        printf ("   names are ");
        for (j = 0; j < names; j++) { /*  for each name */
            printf ("%d ", *ptr);
            if (j == 0)  /*  set row and column  */
                ii = *ptr;
            else if (j == 1)
                jj = *ptr;
            ptr++;
        }
        printf ("\n");
        board[ii][jj] = (board[ii][jj] + 1) % 3;
    }
}


#define BUFSIZE 512

void pickSquares(int button, int state, int x, int y)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;
    
    glGetIntegerv (GL_VIEWPORT, viewport);
    //创建选择缓冲，用于返回点击记录的数组
    glSelectBuffer (BUFSIZE, selectBuf);
    //设置状态为选择模式
    (void) glRenderMode (GL_SELECT);
    
    //初始化名字堆栈，即清除堆栈
    glInitNames();
    //压入一个初始数据到栈顶，保证堆栈不为空
    glPushName(0);
    
    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    //挑选矩阵即点击区域
    gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);
    gluOrtho2D (0.0, 3.0, 0.0, 3.0);
    drawSquares (GL_SELECT);
    
    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glFlush ();
    
    hits = glRenderMode (GL_RENDER);
    processHits (hits, selectBuf);//打印选择缓冲的数据
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSquares (GL_RENDER);
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, 3.0, 0.0, 3.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (100, 100);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutMouseFunc (pickSquares);
    glutKeyboardFunc (keyboard);
    glutMainLoop();
    return 0;
}

