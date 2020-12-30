/*
贪吃蛇
BY：一叶成林\n贴吧：C4droid
游戏实现：
先将界面画出来，并初始化蛇和食物
每个坐标都有类型，里面是食物、蛇体、或墙体还是背景
在蛇体的坐标内有蛇头的方向，这样蛇尾才能根据蛇头的方向移动
 蛇移动是把蛇头前方的坐标改成蛇头，删掉蛇尾，将蛇尾前面的坐标改成蛇尾
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#define MAXH 18
#define MAXW 18  //有bug！长宽必须一样！
#define UP '2'
#define DOWN '8'
#define LEFT '4'
#define RIGHT '6'
#define SNAKELEN 3 //蛇初始长度
#define N 3 //难度
struct{
	char type;
	char*ch;
}
charwall = {
	1,"□"
},//墙体
charbg = {
	2,"■"
},//背景
charfood = {
	3,"●"
},//食物
charsnake = {
	4,"★"
};//蛇体
struct xy{
	int x;
	int y;
}headxy,tailxy;//记录蛇头和蛇尾的坐标
char atlasmap[MAXH][MAXW];//地图所有的坐标
char direction[MAXH][MAXW];//里面存放蛇尾转弯方向
int score;//分数
int main(void);//声明主函数，死了要调用
void drawmap(void);//创建地图
void createfood(void);//随机坐标生成食物
void createsnake(void);//创建蛇
void loading(void);//伪加载
void init(void);//初始化
void die(void);//死亡
void move(char key);//蛇移动
char nextkey(char key,char inpkey);//判断输入的方向是否符合要求
void drawmap(void)//创建地图
{
	int x,y;
	for(y = 0;y<MAXH;y++)
	{
		for(x = 0;x<MAXW;x++)
		{
			if(x==0||y==0||x==MAXW-1||y==MAXH-1)//周围一圈是墙体
			{
				printf("%s",charwall.ch);
				atlasmap[y][x] = charwall.type;
			}
			else
				//创建背景
			{
				atlasmap[y][x] = charbg.type;
				printf("%s",charbg.ch);
			}
		}
		printf("\n");
	}
}
void createfood(void)//随机坐标生成食物
{
	int x,y;
	srand(time(0));
	do
	{
		x = rand()%(MAXW-2)+1;
		y = rand()%(MAXH-2)+1;
	}while(atlasmap[x][y]!=charbg.type)
		;//如果该坐标内不是背景，则重新生成
	gotoxy(x+1,y+1);
	printf("%s",charfood.ch);
	atlasmap[x][y] = charfood.type;//将该坐标存放为食物类型
}
void createsnake(void)//创建蛇
{
	int x_centre,y_centre;
	int len;
	x_centre = MAXW/2+SNAKELEN/2;
	y_centre = MAXH/2;
	for(len = 1;len<=SNAKELEN;len++)
	{
		gotoxy(x_centre-len+1,y_centre+1);
		printf("%s",charsnake.ch);
		//判断蛇头和蛇尾
		if(len==1)//蛇头坐标
		{
			headxy.x = x_centre;
			headxy.y = y_centre+1;
		}
		else if(len==SNAKELEN)//蛇尾坐标
		{
			tailxy.x = x_centre-len+1;
			tailxy.y = y_centre+1;
		}
		atlasmap[x_centre-len][y_centre] = charsnake.type;//坐标类型改成蛇体
		direction[x_centre-len][y_centre] = RIGHT;//初始方向设置为RIGHT
	}
}

void init(void)
{
//	loading();//伪加载
	drawmap();//绘制地图
	createsnake();//初始化贪吃蛇
	createfood();//创建食物
	gotoxy(0,MAXH+3);
	printf("分数：%d\n",score = 0);
	puts("BY：一叶成林\n贴吧：C4droid");
	gotoxy(0,MAXH+1);//将坐标移动到图外进行输入
}
void die(void)
{
	sleep(1);
	clrscr();
	printf("游戏结束！得分：%d\n任意键继续\n",score);
	getch();
	clrscr();
	main();//应该不是很合理……，就这么粗暴的解决吧！
}
void move(char key)//蛇移动
{
	extern struct xy headxy,tailxy;//再次声明
	direction[headxy.x-1][headxy.y-1] = key;//先将蛇头要去的方向放在该坐标内，蛇尾到这里就能判断去哪
	switch(key)
	{
	case RIGHT:
		++headxy.x;
		break;
	case UP:
		--headxy.y;
		break;
	case LEFT:
		--headxy.x;
		break;
	case DOWN:
		++headxy.y;
		break;
	}
	if(atlasmap[headxy.x-1][headxy.y-1]==charwall.type
			||atlasmap[headxy.x-1][headxy.y-1]==charsnake.type)//判断是否撞墙或咬自己
	{
		die();
	}
	gotoxy(headxy.x,headxy.y);
	printf("%s",charsnake.ch);
	//判断是否吃到食物，如果吃到了蛇尾不动，并把该坐标的类型转换成蛇体
	if(atlasmap[headxy.x-1][headxy.y-1]==charfood.type)
	{
		createfood();
		score++;//分数加1
		gotoxy(0,MAXH+3);
		printf("分数：%d",score);
		atlasmap[headxy.x-1][headxy.y-1] = charsnake.type;
	}
	else
		//如果没吃到则蛇尾跟上，这里可用goto语句
	{
		atlasmap[headxy.x-1][headxy.y-1] = charsnake.type;//蛇头前移
		gotoxy(tailxy.x,tailxy.y);//坐标到蛇尾
		atlasmap[tailxy.x-1][tailxy.y-1] = charbg.type;//将蛇尾改成背景，这样就算前进了
		printf("%s",charbg.ch);
		switch(direction[tailxy.x-1][tailxy.y-1])//判断蛇头经过这里的时候拐弯了没有
		{
		case RIGHT:
			direction[tailxy.x-1][tailxy.y-1] = 0;//将该坐标内的方向清空
			++tailxy.x;//进行相应的方向
			break;
		case UP:
			direction[tailxy.x-1][tailxy.y-1] = 0;
			--tailxy.y;
			break;
		case LEFT:
			direction[tailxy.x-1][tailxy.y-1] = 0;
			--tailxy.x;
			break;
		case DOWN:
			direction[tailxy.x-1][tailxy.y-1] = 0;
			++tailxy.y;
			break;
		}
	}
	gotoxy(0,MAXH+1);//到地图外进行方向输入
}
char nextkey(char key,char inpkey)//判断输入的方向是否符合要求
{
	char next;
	if(inpkey==' ')//如果是空格则暂停游戏
	{
		inpkey = getch();
	}
	//判断用户的方向是否相反，如果相反或输入非法则采用上次合法方向
	if((inpkey==UP&&key!=DOWN)||(inpkey==DOWN&&key!=UP)
			||(inpkey==LEFT&&key!=RIGHT)||(inpkey==RIGHT&&key!=LEFT))
		next = inpkey;
	else
		next = key;
	return next;
}
int main(void)
{
	init();//初始化游戏
	char key = RIGHT,inpkey;//定义方向，默认向右
	while(1)//无限循环、、、
	{
		if(kbhit())//判断输入流中是否有数据
		{
			inpkey = getch();//将输入流中的数据保存下来
			key = nextkey(key,inpkey);//返回合法方向
		}
		move(key);//蛇移动
		putchar('\n');//更新数据，输出缓冲区
		usleep(1000000/N);//移动速度
	}
	return 0;
}