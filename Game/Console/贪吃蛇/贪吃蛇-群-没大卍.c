/*
按2546或wasd
控制上下左右移动
*/
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int f,l,//蛇的初始长度
a[60][30]={0};//地图大小
struct{
	int x,y;
}food,snake[26*26];//蛇的最大节点数

void console(),//控制方法
draw(),//显示地图,食物和蛇
move(),//移动方法
getfood(),//获取食物
init();//初始化
int over();//判断游戏状态

int main()
{
	init();
	srand(time(NULL));
	printf("\e[?25l");
	while(over())
	{
		gotoxy(0,0);
		printf("\e[3J");
		usleep(1000000/8.4f);//移动速度
		console();
		move();
		getfood();
		draw();
	}
	printf("\e[18;28f游戏结束\n\e[20;24f按任意键重新开始\n");
	getch();
	main();
}
//实现方法
void init()//初始化
{
	l=3;
	f=6;
	for(int i=0;i<30;i++)
	for(int j=0;j<30;j++)
	a[i][j]=0;
	for(int n=l-1;n>0;n--)
	snake[n].x=2,
	snake[n].y=2;
	snake[0].x=2;
	snake[0].y=2;
	srand(0);
	food.y=rand()%27;
	food.x=rand()%27;
	if(food.x>1&food.y>1)
	a[food.x][food.y]=2;
	else
	init();
}
void draw()
{
	for(int i=0;i<30;i++)
	{
		for(int j=0;j<30;j++)
		printf(a[i][j]==1?
		"\e[0;42m  ":
		a[i][j]==3?//判断蛇身
		"\e[5;42m  ":
		a[i][j]==2?//判断食物
		"\e[0;30m🍎 ":
		i<1^i>28|
		j<1^j>28?//描绘边框
		"\e[5;40m  ":
		"\e[0;40m  ");
		printf("\n");
	}
}
void move()
{
	for(int n=l+1;n>0;n--)
	{
		snake[n]=snake[n-1];
		if(n==l+1)
		a[snake[n].x][snake[n].y]=0;
		else if(n<=1)
		a[snake[n].x][snake[n].y]=1;
		else
		a[snake[n].x][snake[n].y]=3;
	}
}
void getfood()
{
	if(a[food.x][food.y]==1||
	a[food.x][food.y]==3)
	{
		++l;
		food.y=rand()%27;
		food.x=rand()%27;
	}
	while(food.x<1|food.y<1||
		a[food.x][food.y]==3)
		{
			srand(time(NULL));
			food.y=rand()%27;
			food.x=rand()%27;
		}
		a[food.x][food.y]=2;
}
void console()
{
	int i;
	if(kbhit())
	while(kbhit())
	i=getch();
		//控制条件
		i=='2'|i=='w'
		&&f!=5?
			f=2:
		i=='5'|i=='s'
		&&f!=2?
			f=5:
		i=='6'|i=='d'
		&&f!=4?
			f=6:
		i=='4'|i=='a'
		&&f!=6?
			f=4:i;
	switch(f)
	{
		case 5:
			snake[0].x++;
			break;
		case 2:
			snake[0].x--;
			break;
		case 4:
			snake[0].y--;
			break;
		case 6:
			snake[0].y++;
			break;
	}
}
int over()
{
	for(int n=l+1;n>0;n--)
	{
		if(n!=1)
		if(a[snake[n].x]
		[snake[n].y]==1)
		return 0;
	}
	if(snake[0].x<1||snake[0].x>28|
	snake[0].y<1||snake[0].y>28)
	return 0;
}