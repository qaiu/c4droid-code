/*
路边吃数游戏
宫殿中找出几个数之和是7的数并把它们吃掉，
再走到7的位置就获胜了。
by QAIU
*/
#define UP '2'
#define DOWN '5'
#define LEFT '4'
#define RIGHT '6'
#include <conio.h>
#include <time.h>
#include <stdlib.h>
int k[17][24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,
0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
0,3,1,1,1,1,0,1,1,1,4,1,0,1,0,1,1,1,1,1,1,1,0,0,
0,1,0,0,1,0,1,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,
0,2,1,1,1,1,6,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,
0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,5,1,1,1,0,1,0,0,1,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,4,0,0,1,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,1,0,
0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,
0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,3,1,1,1,1,1,0,0,
0,2,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
int en(int *b,int n)
{int i;
for(i=0;i<20;i++)
if(b[i]==n)return 0;
return 1;
}
void fu(int *b)
{int i;
for(i=0;i<20;i++)
b[i]=0;
}
void lostgame(int num)
{
	printf("\e[37m");
	gotoxy(11,22);
	printf("你吃掉的数字总和是: %d而不是7",num);
	gotoxy(11,23);
	printf("很抱歉，游戏失败！任意键开始新游戏...");
	getch();
	gotoxy(11,22);
	printf("                                                        ");
	gotoxy(11,23);
	printf("                                                         ");
}
void wingame()
{
	printf("\e[37m");
	gotoxy(11,22);
	printf(" 棒棒哒！恭喜你赢得了胜利");
	gotoxy(11,23);
	printf(" 任意键退出...");
	getch();
	clrscr();
	exit(0);
}
main()
{
	int i,j,key,num,b[20],p;
 	char ch;
aa: 
	clrscr();
	fu(b);
	num=0;
	p=0;
	gotoxy(16,1);
	printf("数字键2(上) 5(下) 4(左) 6(右)");
	gotoxy(16,2);
	printf("吃地图上的数字使其总和为7");
	gotoxy(16,3);
	printf("然后最后吃掉7 你就赢了！");
	printf("\e[32m");
	for(i=4;i<21;i++)
		for(j=20;j<44;j++)
			if(k[i-4][j-20]==0)
			{
				gotoxy(j,i);
				printf("%c",'#');
			}
			else if(k[i-4][j-20]!=1)
			{
				gotoxy(j,i);
				printf("\e[3%d;1m%d\e[0;32m",k[i-4][j-20]-1,k[i-4][j-20]);
			}
	printf("\e[33m");
	i=5;
	j=21;
	gotoxy(j,i);
	printf("\e[1m@\e[0m");
	gotoxy(33,5);
	printf("7");
	gotoxy(7,7);
	printf("总和:%d",num);;
	gotoxy(11,22);
	printf("\e[37m请输入任意键开始游戏，Q键退出游戏...");
	ch=getch();
	if(ch=='Q'||ch=='q')
	{
		clrscr();
		exit(0);
	}
	else
	{
		gotoxy(11,22);
		printf("                                             ");
	}
	do
	{
		setbuf(stdin,NULL);
		key=getch();
		switch(key)
		{
			case DOWN:
			{
				if(k[i-4+1][j-20]==0)
					continue;
				if(k[i-4+1][j-20]!=1&&k[i-4+1][j-20]!=7&&en(b,i+j+1))
				{
					num+=k[i-4+1][j-20];
					b[p++]=i+j+1;
					gotoxy(7,7);
					printf("总和:%d",num);
				}
	    		printf("\e[33m");
				gotoxy(j,i++);
				printf(" ");
				gotoxy(j,i);
				printf("\e[1m@\e[0m");
				gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			case UP:
			{
				if(k[i-4-1][j-20]==0)
					continue;
				if(k[i-4-1][j-20]!=1&&k[i-4-1][j-20]!=7&&en(b,i+j-1))
				{
					num+=k[i-4-1][j-20];
					b[p++]=i+j-1;
					gotoxy(7,7);
					printf("总和:%d",num);
				}
				printf("\e[33m");
				gotoxy(j,i--);
				printf(" ");
				gotoxy(j,i);
				printf("\e[1m@\e[0m");
				gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			case LEFT:
			{
				if(k[i-4][j-20-1]==0)
					continue;
				if(k[i-4][j-20-1]!=1&&k[i-4][j-20-1]!=7&&en(b,i+j-1))
				{
					num+=k[i-4][j-20-1];
					b[p++]=i+j-1;
					gotoxy(7,7);
					printf("总和:%d",num);
				}
				printf("\e[33m");
				gotoxy(j--,i);
				printf(" ");
				gotoxy(j,i);
				printf("\e[1m@\e[0m");
				gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			case RIGHT:
			{
				if(k[i-4][j-20+1]==0)
					continue;
				if(k[i-4][j-20+1]!=1&&k[i-4][j-20+1]!=7&&en(b,i+j+1))
				{
					num+=k[i-4][j-20+1];
					b[p++]=i+j+1;
					gotoxy(7,7);
					printf("总和:%d",num);
				}
				printf("\e[33m");
				gotoxy(j++,i);
				printf(" ");
				gotoxy(j,i);
				printf("\e[1m@\e[0m");gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			default:
				continue;
		}
	}while(1);
}