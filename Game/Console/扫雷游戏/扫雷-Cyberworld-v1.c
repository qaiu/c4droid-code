/******************************
**制作者:@Cyberworld
**QQ:3258359040
**版本:1.0
**时间:2017.3.1
*******************************
C4droid在G++编译下能计时
2.4.6.8控制移动
5挖雷
+增加标志
-清除标志
@快速移动到输入的坐标
*******************************
****如有bug，望大神批评指出******
*******************************/
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include <errno.h>
#include <termios.h>
clock_t start,end;
int winsize_x,winsize_y,maxx,maxy;
unsigned int count;
int T,xyx=0;
int f=1;
int arr[200][200];
int X,Y;
int mark;
int dug=0;
int lose=0;
int mine=0;
//定义全局变量
void init();
void zk();
void randmine();
void make_num();
void operate();
int setmaxxy();
void l0(int i,int j);
void print_num(int num);
void is_end();
void startgame();
//函数的声明
int main()
{
	char ch;
	setmaxxy();
	do
	{
	clrscr();
	init();
	startgame();
	zk();
	randmine();
	make_num();
	do
	{
	gotoxy(X,Y);
	operate();
	gotoxy(25,maxy+2);
	printf("@:%d/%d",mark,count);
	if(f)
	{
	start = clock(); 
	f=0;
	}
	}while(!mine&&dug<(maxx*maxy-count));
	end = clock();
	T=(end - start)/CLOCKS_PER_SEC;
	is_end();
	gotoxy(1,maxy+3);
	printf("\033[47;31m新游戏请按@键，结束游戏请按回车键，按其他键无效\033[0m");
	do
	{
		ch=getch();
		if(ch=='@')
		xyx=1;
		if(ch=='\n')
		xyx=0;
	}while(ch!='@'&&ch!='\n');
	}while(xyx);
	return 0;
}

void init()//初始化
{
	
	dug=0;
	lose=0;
	mine=0;
	f=1;
	xyx=0;
	maxx=winsize_x-2;
	maxy=winsize_y;
	count=(winsize_x-2)*(winsize_y)/8+1;
	memset(arr,0,sizeof(arr)/sizeof(int));
	printf("\033[?25h");
}
void startgame()//设置参数
{
	int set=0;
	printf("自定义设置请按0,使用游戏默认参数请按其他键\n");
if(getch()=='0')
{
	set=1;
	count=0;
}
	while(set&&(count>=maxx*maxy||count<=0||maxx>winsize_x-2||maxy>winsize_y||maxx<=0||maxy<=0))
{
printf("请设置行长度:(不要超过%d，也不要太小)\n",winsize_x-2);
scanf("%d",&maxx);
printf("请设置列长度:(不要超过%d，也不要太小)\n",winsize_y);
scanf("%d",&maxy);	printf("请设置雷的数量:(参考数目:%d,)\n",maxx*maxy/8+1);
	scanf("%d",&count);
set=0;	if(count>=maxx*maxy||count<=0||maxx>winsize_x-2||maxy>winsize_y||maxx<=0||maxy<=0)
	{
		printf("\n请认真设置!\n");
		set=1;
	}
}
	X=maxx/2;
	Y=maxy/2;
	mark=count;
	clrscr();
}
	

int setmaxxy()//获取尺寸
{
	struct winsize ws;
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != 0)
	{
		perror("TIOCGWINSZ");
		return -1;
	}
		winsize_y= ws.ws_row*2/5;
    	winsize_x= ws.ws_col;
    	
    	return 0;
}

void zk()//打印边界
{
	int i,j;
	for(j=1;j<=maxy;j++)
	{
	for(i=1;i<=maxx+1;i++)
	{
		if(i<=maxx)
	    printf("\033[40;32m%c\033[0m",32);
	if(i==maxx+1)
        printf("#");
	}
	printf("\n");
	}
	for(j=1;j<=maxx+1;j++)
	printf("#");
}
void randmine()//随机产生雷
{
	int i,t,j,randxy[count];
	srand(time(NULL));
	for(i=0;i<count;i++)
	{
		do
		{
		 t=0;
	 	randxy[i]=rand()%(maxx*maxy)+1;
	 	for(j=i-1;j>=0;j--)
	 	{
	   	if (randxy[i]==randxy[j])
	   	{
	     	t=1;
  	   	break;
	   	}
	 	}
		}while (t);
	}
	for(i=1;i<=maxx*maxy;i++)
	for(j=0;j<count;j++)
	if (i==randxy[j])
	arr[(i-1)/maxx+1][(i-1)%maxx+1]=9;
}
void make_num()
{
	int i,j;
	for(i=1;i<=maxy;i++)
	for(j=1;j<=maxx;j++)
	if(arr[i][j]==9)
  {
	if(i>1)
	{
		if(j>1&&arr[i-1][j-1]!=9)
	    arr[i-1][j-1]++;
    	if(j<maxx&&arr[i-1][j+1]!=9)
	     arr[i-1][j+1]++;
	     if(arr[i-1][j]!=9)
	     arr[i-1][j]++;
	}
	if(j>1&&arr[i][j-1]!=9)
	arr[i][j-1]++;
	if(j<maxx&&arr[i][j+1]!=9)
	arr[i][j+1]++;
	if(i<maxy)
	{
		if(j>1&&arr[i+1][j-1]!=9)
	    arr[i+1][j-1]++;
    	if(j<maxx&&arr[i+1][j+1]!=9)
	     arr[i+1][j+1]++;
	     if(arr[i+1][j]!=9)
	     arr[i+1][j]++;
	}
  }
}
void operate()//游戏操作
{
	char ch,c;
	int k,g[2];
	ch=getch();
	switch (ch)
	{
		case '2':if(Y>1&&Y<=maxy) Y--;break;
		case '8':if(Y<maxy&&Y>=1)Y++;break;
		case '4':if(X>1&&X<=maxx)X--;break;
		case '6':if(X<maxx&&X>=1)X++;break;
		case '@':for(k=0;k<2;k++)
                 {
                  g[k]=0;
                     	while(c=getch(),isdigit(c))
                     		g[k]=g[k]*10+c-'0';
                  }
                 if(g[0]>0&&g[0]<=maxx&&g[1]>0&&g[1]<=maxy)
                 {
                 	X=g[0];
                 	Y=g[1];
                 }
                 break;
        case '+':if(arr[Y][X]<10&&mark>0)
       print_num('+');
	         	break;//标志
	    case '-':if (arr[Y][X]>=10&&arr[Y][X]<20&&mark<count)
	             print_num('-');
	                  break;//去除标志
	   case '5': if (arr[Y][X]<9)//挖
	                     	l0(Y,X);
                  	else 
                  	if (arr[Y][X]==9)
		                 mine=1;
	                  break;
       default:break;
	}
	gotoxy(X,Y);
}
void l0(int i,int j)
{
	gotoxy(j,i);
	if(arr[i][j]==0)
	{ 
	   dug++;
	   arr[i][j]+=20;
	   print_num(0);
		if(i>1&&arr[i-1][j]<9)
		l0(i-1,j);
		if(i<maxy&&arr[i+1][j]<9)
		l0(i+1,j);
		if(j>1&&arr[i][j-1]<9)
		l0(i,j-1);
		if(j<maxx&&arr[i][j+1]<9)
		l0(i,j+1);
		if(i>1&&j>1&&arr[i-1][j-1]<9)
		l0(i-1,j-1);		
		if(i<maxy&&j>1&&arr[i+1][j-1]<9)
		l0(i+1,j-1);
		if(j<maxx&&i<maxy&&arr[i+1][j+1]<9)
		l0(i+1,j+1);
		if(j<maxx&&i>1&&arr[i-1][j+1]<9)
		l0(i-1,j+1);
	}
	else 
	if (arr[i][j]<9)
	{
		dug++;
		print_num(arr[i][j]);
			arr[i][j]=arr[i][j]+20;
	}	
}
void print_num(int num)
{
	switch(num)
	{
case 0:printf("\033[47;35m%c\033[0m\033[1D",32);break;
case 1:printf("\033[47;30m1\033[0m\033[1D");break;
case 2:printf("\033[47;34m2\033[0m\033[1D");break;
case 3:printf("\033[47;32m3\033[0m\033[1D");break;
case 4:printf("\033[47;33m4\033[0m\033[1D");break;
case 5:printf("\033[47;35m5\033[0m\033[1D");break;
case 6:printf("\033[47;36m6\033[0m\033[1D");break;
case 7:printf("\033[47;5;31m7\033[0m\033[1D");break;
case 8:printf("\033[47;31m8\033[0m\033[1D");break;
case 9:printf("\033[41m*\033[0m\033[1D");break;
case '+': printf("\033[47;31m@\033[0m\033[1D");
		            arr[Y][X]+=10;
		            mark--;
		            break;
case '-':arr[Y][X]-=10;
	           mark++;           printf("\033[40;30m%c\033[0m\033[1D",32);	   
         break;
		default :break;
	}
}
void is_end()
{
	int i,j;
	for(i=1;i<=maxy;i++)
	for(j=1;j<=maxx;j++)
	if(arr[i][j]%10==9)
	{
	   gotoxy(j,i);
	   if(!mine)
	   arr[i][j]=19;
	   if(arr[i][j]==9)
	   printf("\033[41m*\033[0m\033[?25l");
	   if(arr[i][j]==19)
	   printf("\033[42m*\033[0m\033[?25l");
	   fflush(NULL);
	   //usleep(100000);
	}
		gotoxy(1,maxy+2);
		if(mine)
		printf("\033[47;31mLOSE\033[0m");
		else
		printf("\033[47;31mWIN\033[0m");
		gotoxy(10,maxy+2);
		printf("\033[47;31m总用时%d秒\033[0m",T);

}