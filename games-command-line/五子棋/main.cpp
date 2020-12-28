/*
基于博弈树α-β剪枝算法的五子棋AI
必须使用gcc或g++编译器

AI核心原作者 百度贴吧：e自由电子
UI框架by 千百度
*/
#include<math.h>
#include <conio.h>
#include<stdlib.h>
#include<time.h>
#include "AI.h"
#include "VIGL.h"

#define GRID_NUM 15 //棋盘行列数

enum //定义按键:上,下,左,右;5落子
{
	UP  = '2',
	DOWN = '8',
	LEFT = '4',
	RIGHT = '6',
	OK = '5'
};

int xs=0;//先手标志
int dx,dy;//光标

/*基本框架*/
void event(void);
void drawmap(void);
void menu(void);
int main()
{
	srand(time(NULL));//初始化伪随机数生成器 
	int x,y;
	init_score();
	star:;		
	BOARD=ZERO;
	ROUTE=ZERO_ROUTE;
	ROUTE.first=2;
	dy=dx=GRID_NUM/2+1;
	menu();
	if (!xs)
	    initboard(7,7,1);
	drawmap();
	while(1)
	{
		//clrscr();
		//drawmap();
		event();
		if (BOARD.result)
		{
			if (BOARD.result==1)
	    		printf("你输了⊙﹏⊙");
			if (BOARD.result==2)
	    		printf("你赢了(=_=)");
			printf("\n输入“1”再来一局，其他任意键退出游戏");
			if (getch()!='1')
			{
				clrscr();
				printf("\tby 千百度\n\tAI算法作者:贴吧 e自由电子");
				return 0;
			}
			else break;
		}
	}
	goto star;
	return 0;
}

void menu(void)
{
	int ch;
	clrscr();
	do{
	printf ("\t   请选择: \n\n\t 1.先手执黑\n\t 0.后手执白");
	ch=getch();
	clrscr();
	}while (ch<'0'||ch>'1');	
	xs=ch-'0';
	do{
	printf ("   请选择AI难度级别(1~5)，难度越高AI计算时间越长");
	ch=getch();
	clrscr();
	}while (ch<'1'||ch>'5');	
	level_limit=(ch-'0')*2;
}

/*按键事件函数*/
void event(void)
{
	switch(getch())
	{
		case UP:
		if (dx > 1)
			dx--; 
		else
			dx = GRID_NUM; 
		break; 		
		case LEFT:
		if (dy > 1)
			dy--; 
		else
			dy = GRID_NUM; 
		break; 		
		case RIGHT:
		if (dy < GRID_NUM)
			dy++; 
		else
			dy = 1; 
		break;					 
		case DOWN:
		if (dx < GRID_NUM)
			dx++; 
		else
			dx = 1; 
		break;
		case OK:
		if (BOARD.array[dx-1][dy-1]==0)
		{
			initboard(dx-1,dy-1,2);
			clrscr();
			drawmap();
			printf("AI计算中...\n");
			if (ROUTE.step==1&&abs(dx-8)<3&&abs(dy-8)<3)
  	  	{
  	  	    int x=dx,y=dy;
  	  	    do
  	  	    {
  	  	    dx=dx+(double)rand() / RAND_MAX*3-2;
  	  	    dy=dy+(int)rand() / RAND_MAX*3-2;
  	  	    }while (dx+1==x&&dy+1==y);
			initboard(dx++,dy++,1);
	    	}
			else
			{
							analyse(BOARD,0,-1000000,1000000);
			while(kbhit())	getch();//清除多余的输入
			setbuf(stdin,NULL);
			initboard(DECISION.coo_x,DECISION.coo_y,1);
			dx=DECISION.coo_x+1;
			dy=DECISION.coo_y+1;
			}
		}
		break;
		case '=':
		if (ROUTE.step>1)
		{
		    initboard(ROUTE.x[ROUTE.step-1],ROUTE.y[ROUTE.step-1],0);
		    initboard(ROUTE.x[ROUTE.step-1],ROUTE.y[ROUTE.step-1],0);
		}
		break;
	}
	clrscr();
	drawmap();
}

/*绘制整个棋盘*/
void drawmap(void)
{
	int map[GRID_NUM+2][GRID_NUM+2]={};
	for (int i = 0; i < GRID_NUM+2; i++) 
	{
        for (int j = 0; j < GRID_NUM+2; j++) 
        {
        	if (i==0||i==16||j==0||j==16)	
                map[i][j]=4;
            else 
            	map[i][j]=BOARD.array[i-1][j-1];
        }
	}
	for (int i = 0; i < GRID_NUM+2; i++) 
	{
		for (int j = 0; j < GRID_NUM+2; j++) 
        {
            if ((map[i][j] == 1&&xs==1)||(map[i][j] == 2&&xs==0))
            printf("\e[31;4%dm⚪️\e[0m",(dx==i&&dy==j)?5:3);           
            else if ((map[i][j] == 2&&xs==1)||(map[i][j] == 1&&xs==0))
            printf("\e[4%dm⚫️\e[0m",(dx==i&&dy==j)?5:3);
            else if (map[i][j]==4)
            {
            	if (i==16&&j>0&&j<16)
                printf("\e[1;41;37m%-2c\e[0m",j+64);
                else if (j==0&&i>0&&i<16)
                printf("\e[1;41;37m%2d\e[0m",16-i);
                else
            	printf("\e[1;41;36m＊\e[0m");
            }
            else 
            {
            	if ((i==4&&j==4)||(i==4&&j==12)||(i==12&&j==4)||(i==12&&j==12)||(i==8&&j==8))
          	  printf("\e[4%d;30m※ \e[0m",(dx==i&&dy==j)?5:3);     
            	else
                printf("\e[4%d;34m十\e[0m",(dx==i&&dy==j)?5:3);
            }
        }
        printf("\n");
    }
	printf(" 使用数字键:2468移动光标 5落子 0悔棋\n");
	drawbox(19,1,10,19, {5,5,5,0,0,5});
	printf("\e[2;40H\e[47;30m %s 难度 %d\n\e[0m",xs?"玩家先手":"AI先手",level_limit/2);
	printf("\e[3;40H\e[47;31m 当前估值:%6d",BOARD.eva);
	printf("\e[4;40H\e[47;35m>第%02d手 %s(%c,%02d)\n\e[0m",ROUTE.step+1,(ROUTE.step+1)%2?"黑":"白",dy+'A'-1,16-dx);
	for (int i=1;i<=ROUTE.step&&i<16;i++)
	printf("\e[%d;40H\e[47;30m 第%02d手 %s(%c,%02d)\n\e[0m",4+i,ROUTE.step+1-i,(ROUTE.step+1-i)%2?"黑":"白",ROUTE.y[ROUTE.step-i]+'A',15-ROUTE.x[ROUTE.step-i]);
	
}