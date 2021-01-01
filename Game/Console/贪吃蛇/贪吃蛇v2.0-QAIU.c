#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
/*地图范围*/
#define H 19
#define W 29

#define N 300000 //步时
enum //DIRECTION键:上,下,左,右;5暂停
{
	UP  = '2',
	DOWN = '8',
	LEFT = '4',
	RIGHT = '6',
	PUSS = '5'
};
typedef struct
{
	int y[100];
	int x[100];
	int len;
	int *head;
}SNAKE;//蛇结构体：坐标，长度，蛇头指针
typedef struct
{
	int x;
	int y;
}FOOD;//食物结构体：坐标

SNAKE snake;
FOOD food;
FOOD initfood(); //食物函数，返回食物坐标
void movesnake(); //初始化蛇
void initmap(); //初始化地图
void drawmap();//绘制地图
void gameover();//游戏结束
void event();//事件函数，判断输入
int map[1+H][1+W];//地图数组
char key,direction=RIGHT;//初始化方向
int k=1;//k=1前进-1后退
int main()
{
	srand(clock());//初始化伪随机数生成器 
	snake.x[0]=W/2;//初始化蛇位置居中
	snake.y[0]=H/2;
	snake.len=3;//初始化长度
	snake.head=&snake.x[0];//要移动的蛇头坐标
	food=initfood();//生成食物
	printf("\033[?25l"); //隐藏光标
	while (1)
	{
		movesnake();
		clrscr();
		printf("\033[43;35m分数：%d   身长：%d\n",(snake.len-3)*snake.len,snake.len);
		drawmap();
		usleep(N);//步时刷新
	}
	return 0;
}
void initmap(void)
{
	for (int i=0;i<=H;++i)//定义地图
	for (int j=0;j<=W;++j)
	{
		if ((i==0)||(i==H)||(j==0)||(j==W))
		map[i][j]=1;
		else
		map[i][j]=0;
	}
	map[food.y][food.x]=4;//定义食物
	for(int i=0;i<snake.len;i++)
	{
		if (i==0)
		map[snake.y[i]][snake.x[i]]=2;//定义蛇头
		else
		map[snake.y[i]][snake.x[i]]=3;//定义蛇身
	}
}

void movesnake()
{
	event();
	for(int i=1;i<snake.len;i++)//蛇身移动
	{
		snake.x[snake.len-i]=snake.x[snake.len-i-1];
	//身体及尾巴跟上
		snake.y[snake.len-i]=snake.y[snake.len-i-1];
	}
	*snake.head+=k;//蛇头移动
	if (map[snake.y[0]][snake.x[0]]==4)
	//如果蛇头碰到食物
	{
		snake.len++;//吃掉后蛇长度+1
		snake.x[snake.len-1]=snake.x[snake.len-2];
		//新增的蛇身成为尾巴
		snake.y[snake.len-1]=snake.y[snake.len-2];
		do
		food=initfood();//刷新食物
		while (map[food.y][food.x]==3);
		//食物坐标不和蛇身体坐标重叠
	}
	if(snake.x[0]==W||snake.x[0]==0||snake.y[0]==0||snake.y[0]==H||map[snake.y[0]][snake.x[0]]==3)
	//如果蛇头撞墙或者撞自己则游戏结束	
		gameover();
}

void event()
{
	if (kbhit())//判断输入，有则循环
	{
		key = getch();//获取按键
		switch(key)
		{
			case UP://上
			if (direction==RIGHT||direction==LEFT)
			{//如果原来方向为左右
				snake.head=&snake.y[0];
				//则要移动的蛇头坐标为纵坐标
				k=-1;//方向为负方向
				direction=key;//保存当前方向
			}
			break;//下同，注释略
			case DOWN:
			if (direction==RIGHT||direction==LEFT)
			{
				snake.head=&snake.y[0];
				k=1;
				direction=key;
			}
			break;
			case LEFT:
			if (direction==UP||direction==DOWN)
			{
				snake.head=&snake.x[0];
				k=-1;
				direction=key;
			}
			break;
			case RIGHT:
			if (direction==UP||direction==DOWN)
			{
				snake.head=&snake.x[0];
				k=1;
				direction=key;
			}
			break;
			case PUSS:
			printf("游戏暂停！任意键继续游戏");
			getch();
			break;
		}
	}
	setbuf(stdin,NULL);
}
void drawmap()
{
	initmap();
	for (int i=0;i<=H;i++)//画地图
	{
	for (int j=0;j<=W;j++)
	{
		switch(map[i][j])
		{
			case 0:
			printf("  \33[40;37m");
			break;
			case 1:
			printf("\033[46;35m**\33[40;37m");
			break;
			case 2:
			printf("\033[41;33m@@"
			       "\33[40;37m");
			break;
			case 3:
			printf("\033[42;33m**"
			       "\33[40;37m");
			break;
			case 4:
			printf("\033[33m翔");
			break;
		}
	}
	printf("\n");
	}
}
FOOD initfood()
{
	FOOD food;
    food.y = (double)rand()/RAND_MAX*(H-1)+1;
    food.x = (double)rand()/RAND_MAX*(W-1)+1;
 //   n=rand()%28;
//    m=rand()%28;
   return food;
}
void gameover()
{
	printf("  💔游戏结束：您的分数为%d,任意键继续……",(snake.len-3)*snake.len);
	getch();//等待任意键继续，再次初始化
	snake.head=&snake.x[0];
	if (direction==UP||direction==DOWN)
	direction=LEFT;
	k=1;
	snake.x[0]=W/2;
	snake.y[0]=H/2;
	snake.len=3;
	food=initfood();//刷新食物
}