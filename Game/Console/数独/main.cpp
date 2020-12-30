/*
数独v1.0
2017-7-5
by 千百度
*/
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "solvesoduku.h"
#include "generate.h"

void cue(int x,int y);//提示
void check_error(void);
void putsubject(void);//输入数独矩阵
void drawmap(int map[MMAX][MMAX],int k);//绘制地图
void event(void);//控制移动
int show(void);//主界面

char fixed[512];
int x=0,y=0,aaa=0,cue_k=0,err=0,t;
int check[MMAX][MMAX]={},arr[10]={};

int main (void)
{
	star:
	show();
	cue_k=x=y=t=0;
	while (memcmp(answer,map,sizeof(map)))
	{
		clrscr();
		event();
		cue(x,y);
		check_error();
		drawmap(map,1);
		usleep(100000);
		t++;
	}
	printf("解答成功,任意键继续:\n");
	getch();
	
	clrscr();
	goto star;
    return 0;
}

void cue(int x,int y)
{
	int a[10]={};
	memcpy(arr,a,sizeof (a));
	if (x>=0&&y>=0&&judgemap[y][x]==0)
	{
	for (int i=0;i<MMAX;i++)
	{
		arr[map[i][x]]=1;
		arr[map[y][i]]=1;
	}
	for (int i=y-y%3;i<y-y%3+3;i++)
	for (int j=x-x%3;j<x-x%3+3;j++)
	arr[map[i][j]]=1;
	}
	else
	for (int i=0;i<=MMAX;i++)
	arr[i]=1;
}

void check_error(void)
{
	err=0;
	int a[MMAX][MMAX]={};
	memcpy(check,a,sizeof (a));
	for (int i=0;i<MMAX;i++)
	for (int j=0;j<MMAX;j++)
	{
		if (!judgemap[i][j]&&map[i][j])
		{
			for (int k=0;k<MMAX;k++)
			if ((k!=j&&map[i][j]==map[i][k])||(k!=i&&map[i][j]==map[k][j]))
			check[i][j]=1;
			for (int m=i-i%3;m<i-i%3+3;m++)
			for (int n=j-j%3;n<j-j%3+3;n++)
			if (m!=i&&n!=j&&map[i][j]==map[m][n])
			check[i][j]=1;
		}
		if (check[i][j]) err++;
	}
}

void putsubject(void)
{
	char  str[MMAX][MMAX]={};
	char ch;
	int k=0;
	for (int i=0;i<MMAX;i++)
	{
	for (int j=0;j<MMAX;j++)
	{
		do
		    ch=getch();
		while (ch==32||ch==10);
		str[i][j]=ch;
		judgemap[i][j]=ch-48;
		printf("%c ",str[i][j]);
	}
	printf("\n");
	}
	for (int i=0;i<MMAX;i++)
	for (int j=0;j<MMAX;j++)
	{
		fixed[k++]=str[i][j];
	}
}

void drawmap(int map[MMAX][MMAX],int k)
{
	if (k)
	printf("\033[47;31m2468控制光标移动，5选中输入数字,空格键查看答案\033[0m\n");
	for(int i=0;i<MMAX;i++)
    {
    	if (i==0)
            printf("┏━━━━ ━━━━━━━━━┓\n");
    	if (i==3||i==6)
            printf("┣━━━━ ━━━━━━━━━┫\n");
        for(int j = 0; j<MMAX;j++)
        {
        	if (!(j%3))
        	    printf("┃ "); 
        	if (map[i][j])
        	{
        		if (y==i&&x==j)
        		{
        			if (aaa)
        			    printf("\033[47;31m%d \033[0m", map[i][j]);
        			else
                        printf("\033[45;37m%d \033[0m", map[i][j]);
        		}
                else
                {
                    if (judgemap[i][j])
                        printf("\033[43;30m%d \033[0m", map[i][j]);
                    else if (check[i][j])
                        printf("\033[41;37m%d \033[0m", map[i][j]);
                    else
                        printf("\033[44;37m%d \033[0m", map[i][j]);
                }
        	}
            else
            {
            	if (y==i&&x==j)
            	{
            		if (aaa)
                        printf("\033[47m  \033[0m");
                    else
                        printf("\033[41m  \033[0m");
            	}
                else 
                    printf("\033[46m  \033[0m");
            }
        }
        printf ( "┃\n" );
    }
    printf ( "┗━━━━ ━━━━━━━━━┛\n" );
    if (k)
    {
    	if (cue_k)
    	{
    	    printf("\033[46;31m可选数字:\033[0m ");
    	    for (int i=1;i<10;i++)
    	    { 
    	    	if (arr[i]==0)
    	        printf("\033[43;30m%d \033[0m ",i);
    	    }
    	}
    	else
    	    printf("\033[46;31m按0键开启/关闭提示\033[0m");
    	        printf("\n\033[33m错误:%d\n用时:%0.1fs\033[0m\n",err,(double)t/10);
    }
}

void event(void)
{
	if (kbhit()&&judgemap[y][x]==0&&aaa)
	{
	    aaa=0;
	    int k=getch()-48;
		drawmap(map,1);
	    if (k>=0&&k<=9)
			map[y][x]=k;
		clrscr();
	}
	while (kbhit())//判断输入
	{
		aaa=0;
		int key  =  getch(); //获取按键
		switch(key)
		{
			case '2':
			if (y > 0)
				y--; 
			else
				y = MMAX-1; 
			break; 
			case '4':
			if (x > 0)
				x--; 
			else
				x = MMAX-1; 
			break; 
			case '6':
			if (x < MMAX-1)
				x++; 
			else
				x = 0; 
			break; 
			case '8':
			if (y < MMAX-1)
				y++; 
			else
				y = 0; 
			break; 
			case '5':
			aaa=1;
			break;
			case '0':
			if (cue_k)
			cue_k=0;
			else
			cue_k=1;
			break;
			case 32:
			memcpy(map,answer,sizeof(answer));
			break;
		}
	}
}

int show(void)
{
	int k;
	time_t t;
	printf("\033[1;32m\t\t**********************\n\t\t*      数独v1.0      *\n\t\t**********************\n\033[0m");
	printf("\033[40;33m\t\t请输入难度序号(任意键默认简单):\n\t\t1 入门级\n\t\t2 标准级\n\t\t3 困难级\n\t\t0 骨灰级(题目生成时间较长)\n\t\t\033[36;1m++++++++++++++++++++++++\n\t\t\033[35m4 手动输入 程序解答\n\033[0m"); 
	switch (getch())
	{
		case '1':
		k=41;
		break;
		case '2':
		k=50;
		break;
		case '3':
		k=55;
		break;
		case '0':
		k=60;
		break;
		case '4':	
		clrscr();
		printf("\033[47;35m请输入题目\n未知数用0表示(可以直接粘贴9*9数字矩阵也可以输入81个数)，\n系统会自动过滤空格和换行:\033[0m\n");
			putsubject();
			clrscr();
			printf("计算结果:\n");
		    t=clock();
			solve(fixed);
			if (judge)
			{
				x=y=-1;
	        	drawmap(printmap,0);
	            printf ( "计算次数 = %d\n解的个数 = %d\n", count_num,judge );
	        }
	    	else
	        printf("无解\n");
	        printf("用时 = %ldms\n",(clock()-t)/1000);
	        return 0;
		break;
		default:
		k=40;
		break;
	}
	printf("生成题目中……\n");
	generate_map(k);
	clrscr();
	printf("题目生成完毕\n");
}