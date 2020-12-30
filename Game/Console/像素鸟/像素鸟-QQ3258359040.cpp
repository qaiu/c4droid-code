


/*c语言像素鸟v1.1
*在C4droid上亲测可用
*修复了许多v1.0的bug
*优化了许多v1.0的代码
*建议将终端模拟器的字体大小设为9点阵后以下
*本次更新时间2017.1.27
*版本低级，个人水平有限，还存在其他bug还望批评指出
*本人QQ:3258359040
*/
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_LENGTH 63
#define MAX_WIDTH 23
#define DISTENCE 5
#define count 500
/*说明:
*游戏中按任意键(回车键除外)上升(建议按空格键)
*游戏中按回车键暂停
*GOOD LUCK!
*/
 int high=MAX_WIDTH/2;
 /*全局变量high*/
  void startgame();
  int ingame();
  void gameover(int score);
  int jx();
  void make_holes(int holes[]);
  void remora(int x,int hole);
  void fly();
  int isdead1(int high,int hole);
  void dxk();
  /*自定义函数的声明*/
 int main()
{
    do{
    startgame();
    getch();
    gameover(ingame());
    }while(jx());
    return 0;
}

/*自定义函数*/
 void startgame()/*开始函数*/
{
    clrscr();
    printf("\033[31m");
    printf("欢迎进入像素鸟游戏!\n\n");        printf("说明:游戏中按任意键(回车键除外)上升(建议按空格键)\n游戏中按回车键暂停\nGOOD LUCK!\n");
    printf("请按任意键开始。。。\033[?25l");
}    

int ingame()/*游戏中函数*/
{
    int n,k,g=0;
    int holes[count];
    char str[5];
    int rx=MAX_LENGTH-MAX_LENGTH/3;
    int score=0;
    make_holes(holes);
    fflush(NULL);
    usleep(500000);
    while(1)
    {
        clrscr();
        dxk();
        for(k=g;k<count;k++)
       {
          n=rx+20*k;
          remora(n,holes[k]);
          if(n==0)
          {
             score=k;
             g++;
             if(isdead1(high,holes[k])||g==count)
               return score;
          }
      }
      rx--;
      fly();
      high=high+1;
      gotoxy(1,high);
      printf("\033[44;30m@\033[?25l\033[49;37m");
      if(high>MAX_WIDTH||high<1)
           return score;
        fflush(NULL);
        usleep(300000);
    }
}

void gameover(int score)/*游戏结束函数*/
{
    clrscr();
    high=MAX_WIDTH/2;
    printf("\033[31m");
    printf("GAME OVER!\n");
    printf("score: %d\n",score);
    if(score==count)
    printf("恭喜你，通关！");
    printf("按0键退出游戏。。。\n按1键开始游戏。。。");
}

int jx()
{
    char ch;
    do{
        ch=getch();
    }while(ch!=48&&ch!=49);
    if (ch==48)
    return 0;
    else return 1;
}

void make_holes(int holes[])    
{
    /*产生开口位置函数*/
    int i;
    srand(time(0));
    for(i=0;i<count;i++)
    holes[i]=rand()%(MAX_WIDTH-DISTENCE-1)+1;
}

void remora(int x,int hole)/*障碍物函数*/
{
    int i;
    for(i=1;i<MAX_WIDTH+1;i++)
    {
        if(x>0&&x<=MAX_LENGTH-2)
        {
            if(i<hole||i>hole+DISTENCE)
            {
                gotoxy(x+1,i);
                printf("\033[43;37m1#1\033[49;37m");
            }
            if(i==hole||i==hole+DISTENCE)
            {
                gotoxy(x,i);
                printf("\033[41;41m*^ω^*\033[49;39m");
            }
        }
    }
}

void fly()/*向上移动,控制游戏暂停*/
{
    char ch;
    while(kbhit())
    {
        ch=getch();
        if(ch!='\n')
        high=high-2;
        else 
        {
            clrscr();
            printf("\033[31m请按任意键继续。。。");
            getch();
        }    
    }
}

int isdead1(int high,int hole)/*判断死亡函数*/
{
    if(high<=hole||high>=hole+DISTENCE)
    return 1;
    else return 0;
}

void dxk()/*打印底线框*/
{
    int i;
    gotoxy(1,MAX_WIDTH);
    for(i=0;i<MAX_LENGTH;i++)
    printf("\033[%dm_",i%7+31);
}