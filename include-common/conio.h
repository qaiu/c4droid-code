/*
 * conio.h 
 * C4droid自带这个头，所以使用C4droid运行可以忽略
 * 这个头文件主要提供终端常用的getch和kbhit函数
 * 如果使用其他Linux系统，需要这个头文件将它放在合适位置来引用
 * 
 * @Author: QAIU
 */

#ifndef CONIO_H
#define CONIO_H
 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>

//读取单字符 https://my.oschina.net/yougui/blog/111345
static char getch() 
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char c=getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

//判断输入 https://www.cnblogs.com/xiayong123/archive/2011/07/19/3717262.html
static int kbhit(void)
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return (ch != EOF)? ungetc(ch, stdin),1:0;
}
 
static void gotoxy(int x, int y) { printf("\033[%d;%df", y, x); }

static void _gotoxy(int x,int y) {gotoxy(x,y);}
	 
static void clrscr() { printf("\033[2J\033[0;0f"); }

static void _clrscr() {clrscr();}

static int _getch() {return getch();}

static int _kbhit() {return kbhit();}
#endif