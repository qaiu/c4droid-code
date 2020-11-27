/**************************
* copyright:@cyberworld   *
* qq:3258359040           *
* date:2017.6.10          *
* version:1.1  解决延迟问题 *
***************************
* 2.4.6.8  控制:上.下.左.右 *
* 5键暂停   按任意键继续     *
*     如有bug,望批评指出    *
* 字体10点阵，或者根据实际改  
*千百度整理并简单修改*
*版权归原作者所有*
**************************/
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#define maxy 20
//设置高度
#define maxx 30
//设置宽度
void zk();//制框
void init();//初始化
void printmap();//打印
void move();//操作
int pd(int x,int y);//判断游戏结果
int food();//产生食物
int a[maxy][maxx];//地图
int headx,heady;//蛇头
int m;//是否吃到食物
int len;//蛇长
int fx;//方向
int score;//得分
int flag;//控制游戏结束
int mm;//控制食物的产生
int main()
{
	while (1)
	{
	init();
	zk();
	do
	{
	if (mm)
	{
	food();
	mm=0;
	}
	clrscr();	//清屏
	move();       //移动
	printmap();  //打印图
	usleep(333333);//延迟
	}while (flag);
	printf("按任意键开始新游戏。。");
	getch();
	}
	return 0;
}
void init()   //初始化
{
	headx=maxx/2-5;heady=maxy/2;
	a[heady][headx]=1;
	a[heady][headx-1]=2;
	a[heady][headx-2]=3;
	m=0;
	len=3;
	fx=6;
	score=0;
	flag=1;
	mm=1;
}
void zk()//边界地图
{
	int i,j;
	for(i=0;i<maxy;i++)
	for(j=0;j<maxx;j++)
	if(i==0||i==maxy-1||j==0||j==maxx-1)
	 a[i][j]=-2;//边界
	 else a[i][j]=0;//内部
}
void printmap()//打印
{
	int i,j;
	for (i=0;i<maxy;i++)
	{
	for (j=0;j<maxx;j++)
	{
		if (a[i][j]==0)
	    printf("\033[47m  \033[0m");//空白
	    else if (a[i][j]>1)
		printf("\033[31;42m哈\033[0m");//蛇身
		else if (a[i][j]==1)
	    printf("\033[34;45m头\033[0m");//蛇头
	    else if (a[i][j]==-1)
	    printf("\033[33;47m🍄 \033[0m");//食物
	    else
	    printf("\033[43m  \033[0m");//边界
	}
	printf("\n");
	}
	printf("\033[31;47mlen:%5d    score:%5d\033[0m\n",len,score);
}
int food()
{
   int i,j,count=0,kk;
  srand(time(NULL));//随机产生食物
  kk=rand()%((maxx-2)*(maxy-2)-len-1);
  for (i=1;i<maxy-1;i++)
  for (j=1;j<maxx-1;j++)
    if (a[i][j]==0)//空白处
   {
   count++;
   if (count==kk)
   {
    a[i][j]=-1;
    return 0;
   }
   }
  return 0;
}
void move()
{
	int nn=fx;
	while (kbhit())//判断键盘有无输入
	//修改，if->while
	nn=getch()-48;//若有，由getch()读入
	if (fx+nn!=10)//防止倒退致死
	switch(nn)
	{
		case 2:pd(0,-1);fx=2;break;//蛇头上移
		case 4:pd(-1,0);fx=4;break;//蛇头左移
		case 6:pd(1,0);fx=6;break;//蛇头右移
		case 8:pd(0,1);fx=8;break;//蛇头下移
		case 5:getch();//暂停，按任意键继续
		default:break;//暂停游戏
	}
}
int  pd(int x,int y)
{
	int i,j;
	headx=headx+x;
	heady=heady+y;
	if (a[heady][headx]==-1)
	{
	m=1;
	len++;
	mm=1;
	score=score+5+(len%3);//计分
	}
	else if (a[heady][headx]!=0)	
	{
	printf("GAME OVER!\n");//游戏结束
	flag=0;
	}
	for (i=1;i<maxy-1;i++)
	for (j=1;j<maxx-1;j++)
	if(a[i][j]>=1)
	{
	a[i][j]++;//蛇尾移动
	if (a[i][j]==len+1&&m==0)
	a[i][j]=0;//没吃到食物，尾巴要移动
	}
	m=0;
	a[heady][headx]=1;
	return 0;	
}				