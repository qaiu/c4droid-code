/*c4droid代码手册
 *打飞机
 *风飞飘扬是本人编写
 *TTHHR收集整理
 *转载请说明出处
*感谢 TTHHR 在更新地图方面提供的思路，原来我是
通过计算系统时间来定时更新地图的，这样只能将间隔
控制在秒  */
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#define W 53  //宽
#define H 15  //高
struct Map
{
  char thing;
  char v;
}map[W][H+1];  //定义地图
int Score=0;  //分数
int num=0;  //记步
int X=W/2-3;  //飞机坐标

void load();  //加载。。。玩玩而已
void draw_map();  //绘制地图
void draw_airplane();  //绘制飞机
void airplane(char cho);
void draw_enemy();  //绘制敌机
void enemy(int x);
void gameover();  //判断是否结束
void attack();  //判断敌机是否中弹
void getp();  //发射炮弹
int main()
{
  load();
  while(1)
  {
    if(num%5==0)  //每隔5步绘制一次地图
      draw_map();
    draw_airplane();
    if(num%10==0)  //每隔十步出现一个敌机
      draw_enemy();
    if(num%5==0)  //每隔五步发射一次炮弹
      getp();
    gotoxy(W-2,1);
    printf("%d",Score);  //输出分数
    gotoxy(1,H+2);
    attack();
    gameover();
    usleep(1000*100);
    num++;
  }
  return 0;
}

void getp()
{
  map[X+1][H-4].thing='|';
  map[X+1][H-4].v='2';
}

void airplane(char cho)
{
  int mx;  //判断上一次的位置
  switch(cho)
  {
    case '6':mx=-2;break;
    case '4':mx=2;break;
    default :mx=0;break;
  }
  map[X-1+mx][H-2].thing=' ';
  map[X+mx][H-2].thing=' ';
  map[X+1+mx][H-2].thing=' ';   
  map[X+2+mx][H-2].thing=' ';
  map[X+3+mx][H-2].thing=' '; 
  map[X+mx][H-1].thing=' ';
  map[X+1+mx][H-1].thing=' ';
  map[X+2+mx][H-1].thing=' ';   //清除原来的飞机
  map[X-1][H-2].thing='-';
  map[X][H-2].thing='-';
  map[X+1][H-2].thing='|';
  map[X+2][H-2].thing='-';
  map[X+3][H-2].thing='-'; 
  map[X][H-1].thing='-';
  map[X+1][H-1].thing='-';
  map[X+2][H-1].thing='-';  //绘制新飞机
}

void draw_airplane()
{
  static int n=0;
  char cho;
  if(n==0)
  {
    airplane('5');
    n++;
  }
  else
  {
    setbuf(stdin,NULL);  //清除缓冲区
    if(kbhit())
    {
      cho=getch();
      switch(cho)
      {
        case '4':if(X-2>1)X-=2;break;
        case '6':if(X+2<W-4)X+=2;break;
        default :cho='5';
      }
      n++;
    }
    airplane(cho);
  }
}

void enemy(int x)
{
  map[x-1][0].thing='-';
  map[x][0].thing=']';
  map[x+1][0].thing='+';
  map[x+2][0].thing='[';
  map[x+3][0].thing='-';
  map[x+1][1].thing='+';  //绘制敌机
  map[x-1][0].v='8';
  map[x][0].v='8';
  map[x+1][0].v='8';
  map[x+2][0].v='8';
  map[x+3][0].v='8';
  map[x+1][1].v='8'; 
}

void draw_enemy()
{
  int x;
  srand(time(NULL));
  x=rand()%(W-5-2)+2;  //随机出现敌机
  enemy(x);
}

void draw_map()
{
  static int n=0;
  int x,y;
  char s[4];
  printf("\033[?25l");  //隐藏光标
  if(n==0)  //初始化地图
  {
    for(y=1;y<=H;y++)
      for(x=1;x<=W;x++)
        if(x==1||x==W)
        { if(!(x==W&&y==1))  //防止分数闪烁
          {
            map[x-1][y-1].thing='#';
            map[x-1][y-1].v='5';
          }
        }
        else
        {
          map[x-1][y-1].thing=' ';
          map[x-1][y-1].v='5';
        }
    n++;
  }
  else
  {
    for(y=H;y>=1;y--)
      for(x=W;x>=1;x--)
      { 
        if(map[x-1][y-1].v=='8'&&map[x-1][y-1].thing!=' '&&map[x-1][y].thing==' ')
        {
          map[x-1][y]=map[x-1][y-1];
          map[x-1][y-1].thing=' ';
          map[x-1][y-1].v='5';  //.v为'8'的向下移，用于移动敌机
        }
      }
    for(y=1;y<=H;y++)
      for(x=1;x<=W;x++)
      {
        if(map[x-1][y-1].v=='2'&&map[x-1][y-1].thing!=' '&&map[x-1][y-2].thing==' ')
        {         
          map[x-1][y-2]=map[x-1][y-1];
          map[x-1][y-1].thing=' ';
          map[x-1][y-1].v='5';   //.v为'2'的向上移，用于移动炮弹
        } 
      }
    for(x=1;x<=W;x++)
    {
      map[x-1][H].thing=' ';
      map[x-1][H].v='5';  //清除最下面的一行，不然会出现一排敌机的情况
    }
    for(y=1;y<=H;y++)
      for(x=1;x<=W;x++)
      {
        if(map[x-1][y-1].thing==' ')
          map[x-1][y-1].v=='5';
        if(!((x==W-1||x==W||x==W-2)&&y==1))  //防止分数闪烁
        {
          gotoxy(x,y);
          printf("%c\n",map[x-1][y-1].thing);  //输出地图
        }
      }
    n++;
  }
}

void attack()
{
  int x,y;
  for(y=1;y<=H-2;y++)
    for(x=3;x<=W-4;x++)
      if(map[x-1][y-1].thing==']')
        if(map[x-2][y].thing=='|'||map[x-1][y].thing=='|'||map[x][y+1].thing=='|'||map[x+1][y].thing=='|'||map[x+2][y].thing=='|')  //判断敌机下面是否有炮弹
        {
          map[x-2][y-1].thing=' ';
          map[x-1][y-1].thing=' ';
          map[x][y-1].thing=' ';
          map[x+1][y-1].thing=' ';
          map[x+2][y-1].thing=' ';
          map[x-2][y].thing=' ';
          map[x-1][y].thing=' ';
          map[x][y].thing=' ';
          map[x+1][y].thing=' ';
          map[x+2][y].thing=' ';
          map[x+1][y+1].thing=' ';   //清除敌机
          Score++;
        }
}

void gameover()
{
  int n=0;
  if(map[X-1][H-3].thing==' ')
    if(map[X][H-3].thing==' ')
      if(map[X+1][H-3].thing==' ')
        if(map[X+2][H-3].thing==' ')
          if(map[X+3][H-3].thing==' ')
            if(map[X-3][H-1].thing!='+'&&map[X+3][H-1].thing!='+')  //判断是否撞到敌机
              n=1;
  if(n==0)
  {
    printf("\nGame Over!!!\n\nScore:%d\n",Score);
    exit(0);
  }
}

void load()
{
  int i,j=0;
  char s[4][30]={"正在加载地图","正在加载敌机","正在加载本机","正在加载炮弹"};
  printf("\033[?25l");
  for(i=0;i<=100;i++)
  {
    gotoxy(1,1);
    printf("加载中……%d%%\n",i);
    if(i==0||(i-25)%25==0)
    {
      gotoxy(1,2);
      printf("%s\n",s[j++]);
    }
    usleep(1000*50);
  }
  clrscr();
  printf("加载完成！\n");
  usleep(1000*1000);
  clrscr();
}