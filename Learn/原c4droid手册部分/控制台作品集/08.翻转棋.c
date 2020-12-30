/*c4droid代码手册
*翻转棋
*作者 火可天
*转载请注明出处
*/
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define N 8
int x=-1;
int flag0=1;
int flag1=2;
int Temp;
int Ls=0;//结束判断计数
char aBody[N+2][N+6];
char jBody[250][N+2][N+6];//新加
/*************自定义函数****************/
void draw();//构画
void Start();//初始
void Luo(int i,int j);//落子
void JL();//记录
void SC();//输出
void HY();//还原
void HYSC();//还原输出
void FZ(int i,int j,int flag0,int flag1);//反转
void XS(int flag0,int flag1);//显示
void PD(int i,int j);//换人,结束判断
void End(int Qz0,int Qz1);
/****************************************/
/****************主函数*****************/
main()
{
	int i=3,j=5;
	Start();
	Luo(i,j);
}
/****************************************/
/**************构画*****************/
void draw()
{
	int i,j;
	for (i=1;i<=N+2;i++)
	{
		for (j=1;j<=N+6;j++)
		{
		if (j<=3||j>N+3)
		{
			printf("\033[47;45m  ");
			}
		else if (i==1||i==N+2)
		{
			printf("\033[47;45m  ");
			}
		else
		{
			if (aBody[i][j]==3)
			{
		if (j==N+3) {printf("\033[47;40m+");
				printf("\033[36;47m ");}
	    else {printf("\033[47;40m+");
				printf("\033[36;47m─");}
			}
/**************棋盘构建*****************/
			else if (aBody[i][j]==0)
			{
//当棋盘身体为0时,输出构建棋盘
				if (i==2&&j==4)
				{
					printf("\033[36;47m┌─");
					}
				else if (i==2&&j==N+3)
				{
					printf("\033[36;47m┐ ");
					}
				else if (i==2)
				{
					printf("\033[36;47m┬─");
					}
				else if (i==9&&j==4)
				{
					printf("\033[36;47m└─");
					}
				else if (i==9&&j==N+3)
				{
					printf("\033[36;47m┘ ");
					}
				else if (i==9)
				{
					printf("\033[36;47m┴─");
					}
				else if (j==4)
				{
					printf("\033[36;47m├─");
					}
				else if (j==N+3)
				{
					printf("\033[36;47m┤ ");
					}
				else
				{
				printf("\033[36;47m┼─");
				}
				}
/****************************************/
			else if (aBody[i][j]==1)
			{
//当棋盘身体为1时,输出蓝色棋子
			 if (j==N+3)
			{printf("\033[47;44m+");
			 printf("\033[36;47m ");}
			 else 
			 {printf("\033[47;44m+");
			 printf("\033[36;47m─");}
				}
			else if (aBody[i][j]==2)
			{
	//当棋盘身体为2时,输出黄色棋子
	         if (j==N+3)
			{printf("\033[47;43m+");
			 printf("\033[36;47m ");}
			 else 
			 {printf("\033[47;43m+");
			 printf("\033[36;47m─");}
				}
			else if (aBody[i][j]==4)
			{
	//当棋盘身体为4时,输出应落子的地方
		if (j==N+3) {printf("\033[36;47m○ ");}
		else {printf("\033[36;47m○─");}
				}
		}
		}
				printf("\n");
		}
	}
/****************开始*******************/
void Start()//初始
{
	int i,j;
	printf("\033[43;36m***************************\n");
printf("\033[43;37m********  ");
printf("\033[43;37翻转棋");
printf("\033[43;37m  *********\n");
printf("\033[43;36m***************************\n");
printf("\033[36;47m                           \n");
printf("*********游戏开始**********\n");
printf("\033[36;47m                           \n");
printf("\033[43;36m         制作  by  火可天  \n");
printf("\033[43;36m***************************\n");
printf("\033[36;47m                           \n");
printf("Loading.......             \n");
printf("\033[36;47m                     3.0版 \n");
/*以上是初使界面,版权所有翻版必究*/
/*呵呵，这是开玩笑的，还请各位多指*/
/*出改正.*/
	usleep(5000000);
/*此处暂停,接下来棋盘初使构建*/
    clrscr();
		for (i=1;i<=N+2;i++)
	{
		for (j=1;j<=N+6;j++)
		{
			if ((i==5&&j==7)||(i==6&&j==8))
			{
				aBody[i][j]=1;
				}
		else if ((i==6&&j==7)||(i==5&&j==8))
			{
				aBody[i][j]=2;
				}
			else
			{
			aBody[i][j]=0;
			}
			}
		}
/*************界面初使化*************/
	XS(flag0,flag1);
	JL();
	SC();
	aBody[3][5]=3;//光标初始化
	draw();
	printf("\033[47;41m输入(%d,%d)",3,5);
printf("\033[47;41m 2.上 8.下 4.左 6.右");
printf("\033[47;45m \n");
if (flag0==1)
{
	printf("\033[47;44m蓝方执子  5.落子 0.悔棋      ");
	}
else
{
	printf("\033[47;44m黄方执子  5.落子 0.悔棋      ");
}
}
/****************************************/
/****************落子******************/
/****与换人,结束判断功能相互调用***/
/*************实现循环****************/
void Luo(int i,int j)//落子
{
	int G;
	G=getch();
	switch (G)
	{
		case '2':if (i-1<2) {i=2;}
		              else {i--;}break;
		case '4':if (j-1<4) {j=4;}
					   else {j--;}break;
		case '8':if (i+1>9) {i=9;}
		              else {i++;}break;
		case '6':if (j+1>N+3) {j=N+3;}
		              else {j++;}break;
/************新加悔棋功能**************/
		case '0':if (x-1>=0)
		             {
		               x--;
		              Temp=flag0;
                 	  flag0=flag1;
                       flag1=Temp;
		             }
		             break;
/**************************************/ 
		case '5': HY();
	               	SC();
	               	XS(flag0,flag1);
		               if (aBody[i][j]==4)
		              {
		                HY();
					   HYSC();//重要
		              aBody[i][j]=flag0;
/**************棋子转换****************/
		              Temp=flag0;
                 	  flag0=flag1;
                       flag1=Temp;
/****************************************/
/**************      反转      **************/
  				    FZ(i,j,flag0,flag1);
/****************************************/
/**********落子地方电脑判断***********/
					  XS(flag0,flag1);
/***************************************/
/************新加记录功能*************/
		              JL();
/***************************************/
						}
                      break;
         default:break;
		}
		HY();
		SC();
		aBody[i][j]=3;
		clrscr();
		draw();
printf("\033[47;41m输入(%d,%d)",i-1,j-3);
printf("\033[47;41m 2.上 8.下 4.左 6.右");
printf("\033[47;45m \n");
if (flag0==1)
{
	printf("\033[47;44m蓝方执子");
}
else
{
	printf("\033[47;43m黄方执子");
}
/************换人,结束判断************/
     Ls=0;
     PD(i,j);
/***************************************/
     Luo(i,j);
}
/***************************************/
/****************记录*******************/
void JL()//记录
{
	int i,j;
	   x++;
		for (i=1;i<=N+2;i++)
	{
		for (j=1;j<=N+6;j++)
		{
			jBody[x][i][j]=aBody[i][j];
			}
		}
}
/****************************************/
/****************输出********************/
void SC()//输出
{
	int i,j;
		for (i=1;i<=N+2;i++)
	{
		for (j=1;j<=N+6;j++)
		{
			aBody[i][j]=jBody[x][i][j];
			}
		}
}
/****************************************/
/******************还原*****************/
/******将棋盘的各部分身体清0********/
void HY()//还原,清空数据aBody[][]的记录
{
		int i,j;
		for (i=1;i<=N+2;i++)
	{
		for (j=1;j<=N+6;j++)
		{
			aBody[i][j]=0;
			}
		}
	}
/****************************************/
/*************还原输出*****************/
/*****将落子判断产生的圈清0,保留***/
/*****棋盘棋子,然后读取数据*********/
//很重要
void HYSC()
{
		int i,j;
		for (i=1;i<=N+2;i++)
	{
		for (j=1;j<=N+6;j++)
		{
			if (jBody[x][i][j]==4)
			{aBody[i][j]=0;}
			else
			{aBody[i][j]=jBody[x][i][j];}
			}
		}
	}
/****************************************/
/***************反转********************/
/********两大主要功能之一************/
void FZ(int i,int j,int flag0,int flag1)
{
	int X,Y,xmin,xmax,ymin,ymax;
	X=i;
	Y=j;
//向上
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
         X--;
		if (aBody[X][Y]==flag1)
		{
			xmin=X;
			X=i-1;
		  while (X>=xmin)
		  {
		  	aBody[X][Y]=flag1;
		  	X--;
		  }
		  goto ZY1;//转移
		}
/*向上查找当有aBody[X][Y]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY1:
X=i;
Y=j;
//向下
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
         X++;
		if (aBody[X][Y]==flag1)
		{
			xmax=X;
			X=i+1;
		  while (X<=xmax)
		  {
		  	aBody[X][Y]=flag1;
		  	X++;
		  }
		  goto ZY2;//转移
		}
/*向上查找当有aBody[X][Y]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY2:
X=i;
Y=j;
//向左
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
         Y--;
		if (aBody[X][Y]==flag1)
		{
			ymin=Y;
			Y=j-1;
		  while (Y>=ymin)
		  {
		  	aBody[X][Y]=flag1;
		  	Y--;
		  }
		  goto ZY3;//转移
		}
/*向上查找当有aBody[X][Y]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY3:
X=i;
Y=j;
//向右
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
         Y++;
		if (aBody[X][Y]==flag1)
		{
			ymax=Y;
			Y=j+1;
		  while (Y<=ymax)
		  {
		  	aBody[X][Y]=flag1;
		  	Y++;
		  }
		  goto ZY4;//转移
		}
/*向上查找当有aBody[X][Y]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY4:
X=i;
Y=j;
//向左上
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
	     X--;
         Y--;
  //此后判断用X或Y的范围都一样,我用X
		if (aBody[X][Y]==flag1)
		{
			xmin=X;
			X=i-1;
			Y=j-1;
		  while (X>=xmin)
		  {
		  	aBody[X][Y]=flag1;
		  	X--;
		  	Y--;
		  }
		  goto ZY5;//转移
		}
/*向上查找当有aBody[X][Y]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY5:
X=i;
Y=j;
//向右下
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
	     X++;
         Y++;
  //此后判断用X或Y的范围都一样,我用X
		if (aBody[X][Y]==flag1)
		{
			xmax=X;
			X=i+1;
			Y=j+1;
		  while (X<=xmax)
		  {
		  	aBody[X][Y]=flag1;
		  	X++;
		  	Y++;
		  }
		  goto ZY6;//转移
		}
/*向上查找当有aBody[X][Y]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY6:
X=i;
Y=j;
//向右上
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
	     X--;
         Y++;
  //此后判断用X或Y的范围都一样,我用X
		if (aBody[X][Y]==flag1)
		{
			xmin=X;
			X=i-1;
			Y=j+1;
		  while (X>=xmin)
		  {
		  	aBody[X][Y]=flag1;
		  	X--;
		  	Y++;
		  }
		  goto ZY7;//转移
		}
/*向上查找当有aBody[X][X]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY7:
X=i;
Y=j;
//向左下
while ((X>=2&&X<=9)&&(Y>=4&&Y<=N+3))
{
	     X++;
         Y--;
  //此后判断用X或Y的范围都一样,我用X
		if (aBody[X][Y]==flag1)
		{
			xmax=X;
			X=i+1;
			Y=j-1;
		  while (X<=xmax)
		  {
		  	aBody[X][Y]=flag1;
		  	X++;
		  	Y--;
		  }
		  goto ZY8;//转移
		}
/*向上查找当有aBody[X][X]=flag0时翻转开始到结束之间的*/
	   else if (aBody[X][Y]==flag0){continue;}
	    else {break;}
}
ZY8:
flag0=flag0;
}
/*****************************************/
/*************落子地方判断*************/
/********两大主要功能之一************/
void XS(int flag0,int flag1)
{
	int X,Y,xm,ym;
for (X=2;X<=9;X++)
{
	for (Y=4;Y<=3+N;Y++)
	{
	if (aBody[X][Y]==flag0)
	{
//向上
	xm=X;
	ym=Y;
if (aBody[xm-1][ym]==flag1)
{
	xm=xm-1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
		xm--;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS1;}
	}}
XS1:
//向下
	xm=X;
	ym=Y;
if (aBody[xm+1][ym]==flag1)
{
	xm=xm+1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
		xm++;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS2;}
	}}
XS2:
//向左
	xm=X;
	ym=Y;
if (aBody[xm][ym-1]==flag1)
{
	ym=ym-1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
		ym--;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS3;}
	}}
XS3:
//向右
	xm=X;
	ym=Y;
if (aBody[xm][ym+1]==flag1)
{
	ym=ym+1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
		ym++;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS4;}
	}}
XS4:
//向左上
	xm=X;
	ym=Y;
if (aBody[xm-1][ym-1]==flag1)
{
	xm=xm-1;
	ym=ym-1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
	xm--;
	ym--;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS5;}
	}}
XS5:
//向右下
	xm=X;
	ym=Y;
if (aBody[xm+1][ym+1]==flag1)
{
	xm=xm+1;
	ym=ym+1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
	xm++;
	ym++;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS6;}
	}}
XS6:
//向右上
	xm=X;
	ym=Y;
if (aBody[xm-1][ym+1]==flag1)
{
	xm=xm-1;
	ym=ym+1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
	xm--;
	ym++;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS7;}
	}}
XS7:
//向左下
	xm=X;
	ym=Y;
if (aBody[xm+1][ym-1]==flag1)
{
	xm=xm+1;
	ym=ym-1;
	while( (xm>=2&&xm<=9)&&(ym>=4&&ym<=N+3))	
{
	xm++;
	ym--;
if (aBody[xm][ym]==flag0){break;}
else if (aBody[xm][ym]==flag1){continue;}
else if (aBody[xm][ym]!=1&&aBody[xm][ym]!=2)
{
	aBody[xm][ym]=4;
	goto XS8;}
	}}
XS8:
flag0=flag0;
	}
	}
}	
}
/*****************************************/
/**************换人,结束判断************/
/***与落子功能相互调用实现循环******/
void PD(int i,int j)
{
	int X,Y,Js=0,Qz0=0,Qz1=1;
	SC();
	for (X=2;X<=9;X++)
	{
		for (Y=4;Y<=N+3;Y++)
		{
			if (aBody[X][Y]==4)
			{
				Js++;
			}
			else if (aBody[X][Y]==1)
			{
				Qz0++;	
			}
			else if (aBody[X][Y]==2)
			{
				Qz1++;
			}
		}
	}
	if (Js<=0) 
	{
		if (flag0==1)
{
	printf("\033[47;44m,但无路可走由对手开始");
		printf("\033[47;45m ");
/****************************************/
         Ls++;//结束判断的计数
/****************************************/
         HY();
	     HYSC();//重要
/**************棋子转换****************/
		              Temp=flag0;
                 	  flag0=flag1;
                       flag1=Temp;
 /**********落子地方电脑判断***********/
					  XS(flag0,flag1);
/************新加记录功能*************/
                      x--;
		              JL();
         PD(i,j);
	}
else
{
	printf("\033[47;43m,但无路可走由对手开始");
		printf("\033[47;45m ");
/****************************************/
         Ls++;//结束判断的计数
/****************************************/
       HY();
       HYSC();//重要
 /**************棋子转换****************/
		              Temp=flag0;
                 	  flag0=flag1;
                       flag1=Temp;
/**********落子地方电脑判断***********/
					  XS(flag0,flag1);
/************新加记录功能*************/
                      x--;
		              JL();
}
	}
	else
	{
		if (flag0==1)
		{
	     printf("\033[47;44m  5.落子 0.悔棋      ");
	     printf("\033[47;45m ");
		}
		else
		{
	     printf("\033[47;43m  5.落子 0.悔棋      ");
	     printf("\033[47;45m ");
		}
	}
/***********结束判断的标志************/
if (Ls==2||Qz0+Qz1==64)
{
	clrscr();
	End(Qz0,Qz1);
}
/****************************************/
}
/****************************************/
/***************结束*******************/
void End(int Qz0,int Qz1)
{
	printf("***************************\n");
		printf("          游戏结束\n");
		printf("***************************\n");
	if (Qz0>Qz1)
	{printf("先手%d个子\n",Qz0);
	printf("后手%d个子\n",Qz1);
	printf("先手胜！！\n");
	printf("Loading.......             \n");}
	if (Qz0<Qz1)
	{printf("先手%d个子\n",Qz0);
	printf("后手%d个子\n",Qz1);
	printf("先手败！！\n");
	printf("Loading.......             \n");}
	usleep(5000000);
	clrscr();	printf("\033[43;36m***************************\n");
printf("\033[43;37m********   ");
printf("\033[43;37翻转棋");
printf("\033[43;37m  ********\n");
printf("\033[43;36m***************************\n");
printf("\033[36;47m                           \n");
printf("*********谢谢使用**********\n");
printf("\033[36;47m                           \n");
printf("\033[43;36m         制作  by  火可天  \n");
printf("\033[36;47m                     3.0版 \n");
	exit(0);
}
/****************************************/