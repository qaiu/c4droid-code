/*c4droid代码手册
 *五子棋
 *火可天编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/****************************************/
//自定义函数区，作用嘛，后边有解释
//这里大概解释一下....
void Pan_draw(int x,int y,int sum);
//构化精美(额,还算了)界面
void Init();//初使化界面
int Start_Menu(int Game_State);
//开始界面
int Run(int Game_State,int x,int y);
//运行
void Record();//记录
void SC();//输出
int End_Judge(int x,int y);//结束判断
int getSum(int i,int j,int x,int y);
//统计棋子数
void End(int Game_State);//结束
/****************************************/
/*****这里不解释，主要代码是抄的**/
/***我只是将其融入我的代码中*******/
  int count(int qz_x, int qz_y, int ex, int ey,int bwf) ;
 int makesense(int qz_x, int qz_y, int ex, int ey);
 int getType(int qz_x,int qz_y,int bwf);
 int getMark(int k,int bwf);
/****************************************/
//自定义变量
int Body[20][20];//棋盘组成
int jBody[226][20][20];
//记录每局棋谱，很重要
int game_state=3;
//游戏状态	
//	1	游戏结束，白方胜
//	2	游戏结束，黑方胜
//	3	游戏开始，进入开始菜单
//	4	游戏运行，进入运行菜单
int X[226];
int Y[226];
//记录每局落子的坐标
int M=-1;
//是int jBody[226][16][16]中的第一个变量
//记录棋局数
int flag1=2;
int flag2=1;
int temp=0;
//在双方落子转换，重要变量
int Game=1;//游戏类型 1 双人 2 单人
	int sum=2;
/****************************************/



/***************************************/
int AI();
int gStyle(int i,int j,int zuo_x,int zuo_y);
int AI_Judge(int zi,int i,int j,int ai_x,int ai_y);
struct BEST
{
	int BestJie;
	int x[225];
	int y[225];
};
struct BEST Best[230];
struct BEST JIE_BEST;
int best;
int Sort[5][300000];
/***************************************/

//主函数
main()
{
  	Init();
 	Record();
	game_state=		Start_Menu(game_state);
	game_state=Run(game_state,8,8);
	End(game_state);
}
/****************************************/
//下面是自定义函数具体内容
/****************************************/
/*函数名称:Pan_draw(int x,int y,int sum)
函数介绍:
构化界面(包括棋盘和右边功能栏)
输入参数:
最后落子的坐标(x,y),落子次数sum
输出参数:无
返回值:无
*/
void Pan_draw(int x,int y,int sum)
{
		int i,j;
/***************************************/
//棋盘构化
/*
Body[i][j]=
	0	输出初始界面
	1	输出白子
	2	输出黑子
	3	输出光标	
	4	输出定位的'■'
*/
		printf("\033[46;42;5m a b c d e f g h i j k l m n o  \033[0m\n");
	for (i=1;i<=15;i++)
	{
		for (j=0;j<=16;j++)
		{
		if (j==0)
		{
 printf("\033[46;42;5m%c\033[0m",96+i);
		}
		else if (j==16)
		{
 printf("\033[46;42;5m \033[0m");		
		}
		else
		{
				if (Body[i][j]==0)
				{
					if (j==15)
					{
printf("\033[36;40;7m┼\033[0m");
					}
					else
					{ printf("\033[36;40;7m┼─\033[0m");
					}
				}
				else if (Body[i][j]==1)
				{
					if (j==15)
					{
printf("\033[31;36;7m●\033[0m");	
					}
					else
					{
printf("\033[31;36;7m●─\033[0m");
					}
				}
				else if (Body[i][j]==2)
				{
					if (j==15)
					{
printf("\033[47;36;7m●\033[0m");
					}
					else 
					{
printf("\033[47;36;7m●\033[0m");	
printf("\033[31;36;7m─\033[0m");
					}	
				}
				else if (Body[i][j]==3)
				{
printf("\033[46;30;5m■\033[0m");			printf("\033[31;36;7m─\033[0m");
				}
				else if (Body[i][j]==4)
				{
					if (j==15)
					{
printf("\033[41;36;7m■\033[0m");
					}
					else 
					{
printf("\033[41;36;7m■\033[0m");	
printf("\033[31;36;7m─\033[0m");
					}						
				}
		}
	}
		printf("\n");
}
		printf("\033[43;42;5m                                 \033[0m\n");
/***************************************/
//右边框栏构化
/*
1.上边部分显示当前棋局黑方或白方走动情况
2.中间部分显示包括当前棋局，前几局棋的落子情况
3.下边部分显示操作功能界面
*/
	gotoxy(32,1);
	printf("\033[40;31;7m==================\033[0m");
/***************************************/
//上部分
	X[sum]=x;
	Y[sum]=y;
	gotoxy(32,2);
	printf("\033[47;32;7m第%3d局\033[0m",sum/2);
	if (sum%2==0)
	{
	printf("\033[47;32;7m黑方走(%c,%c)\033[0m",X[sum]+96,Y[sum]+96);
	gotoxy(32,3);
	printf("\033[47;32;7m                  \033[0m");
	}
	else if (sum%2==1)
	{
	printf("\033[47;32;7m黑方走(%c,%c)\033[0m",X[sum-1]+96,Y[sum-1]+96);
	gotoxy(32,3);
	printf("\033[47;32;7m       白方走(%c,%c)\033[0m",X[sum]+96,Y[sum]+96);
	}
	gotoxy(32,4);
	printf("\033[40;31;7m==================\033[0m");
/***************************************/
//中间部分
gotoxy(32,5);
printf("\033[40;33;7m%3d.\033[0m",sum/2);
	if (sum%2==0)
	{
	printf("\033[40;33;7m黑(%c,%c)\033[0m",X[sum]+96,Y[sum]+96);
printf("\033[40;33;7m       \033[0m");
	for (i=10;i>=2;i=i-2)
	{
		if (sum-i>=2)
		{
	gotoxy(32,5+i/2);	printf("\033[40;33;7m%3d.\033[0m",(sum-i)/2);
	printf("\033[40;33;7m黑(%c,%c)\033[0m",X[sum-i]+96,Y[sum-i]+96);
	gotoxy(43,5+i/2);
	printf("\033[40;33;7m白(%c,%c)\033[0m",X[sum-i+1]+96,Y[sum-i+1]+96);
		}
		else
		{
	gotoxy(32,5+i/2);
	printf("\033[40;33;7m                  \033[0m");			
		}
	}
	}
	else if (sum%2==1)
	{
	printf("\033[40;33;7m黑(%c,%c)\033[0m",X[sum-1]+96,Y[sum-1]+96);
	printf("\033[40;33;7m白(%c,%c)\033[0m",X[sum]+96,Y[sum]+96);
	for (i=10;i>=2;i=i-2)
	{
		if (sum-i-1>=2)
		{
	gotoxy(32,5+i/2);	printf("\033[40;33;7m%3d.\033[0m",(sum-i)/2);
	printf("\033[40;33;7m黑(%c,%c)\033[0m",X[sum-i-1]+96,Y[sum-i-1]+96);
	gotoxy(43,5+i/2);
	printf("\033[40;33;7m白(%c,%c)\033[0m",X[sum-i]+96,Y[sum-i]+96);
		}
		else
		{
	gotoxy(32,5+i/2);
	printf("\033[40;33;7m                  \033[0m");			
		}
	}
	}
	gotoxy(32,11);
	printf("\033[40;31;7m==================\033[0m");
/***************************************/
//下边部分
	gotoxy(32,12);
	printf("\033[46;41;7m2.上8.下4.左6.右  \033[0m");
	gotoxy(32,13);
	printf("\033[46;41;7m5.确定0.悔棋      \033[0m");
	gotoxy(32,14);
	printf("\033[46;41;7m                  \033[0m");
	gotoxy(32,15);
	printf("\033[46;41;7m                  \033[0m");
	gotoxy(32,16);
	printf("\033[40;31;7m==================\033[0m");
	gotoxy(32,17);
	printf("\033[40;31;7m==================\033[0m");
}
/****************************************/
/*函数名称:Init()
函数介绍:初始化界面
输入参数:无
输出参数:无
返回值:无
*/
void Init()
{
	int i,j;
	for (i=1;i<=15;i++)
	{
		for (j=1;j<=15;j++)
		{
			Body[i][j]=0;
		}
	}
	Body[4][4]=3;
	Body[4][12]=3;
	Body[12][4]=3;
	Body[12][12]=3;
	Body[8][8]=3;
}
/****************************************/
/*函数名称:Start_Menu(int Game_State)
函数介绍:游戏初始化选择界面
		2	上	8	下	5	确定
输入参数:游戏状态Game_State
输出参数:无
返回值:游戏状态4(运行状态)
*/
int Start_Menu(int Game_State)
{
	//游戏方式 1 双人模式 
	//2 单人模式 3 退出游戏
	while (Game_State==3)
	{
			gotoxy(1,0);
	printf("\033[41;44;5m                                                \033[0m");
			gotoxy(1,2);
	printf("\033[41;45;5m                                                \033[0m");
			gotoxy(1,3);
	printf("\033[41;46;5m                                                \033[0m");
	          gotoxy(1,4);
		printf("\033[44;43;5m                                                \033[0m");
	gotoxy(20,6);
	if (Game==1){printf("\033[41;46;7m双人游戏\033[0m");}
	else {printf("双人游戏");}
	gotoxy(20,8);
	if (Game==2){printf("\033[41;46;7m单人游戏\033[0m");}
	else {printf("单人游戏");}
	gotoxy(20,10);
	if (Game==3){printf("\033[41;46;7m退出游戏\033[0m");}
	else {printf("退出游戏");}
			gotoxy(1,13);
	printf("\033[41;43;5m                                                \033[0m");
			gotoxy(1,14);
	printf("\033[41;46;5m                                                \033[0m");
			gotoxy(1,15);
	printf("\033[41;45;5m                                                \033[0m");
	          gotoxy(1,16);
		printf("\033[44;44;5m                                                \033[0m");
	char ch=getch();
	switch(ch)
	{
		case '2':
				if (Game-1<1){Game=3;}
				else {Game--;}
				break;
		case '8':
				if (Game+1>3){Game=1;}
				else {Game++;}
				break;
		case '5':
				if (Game==1){return 4;}
				else if (Game==2) 			
				{return 4;}
				else {exit(0);}
				break;
		default:break;
	}
}//while
return 0;
}//Start_Menu
/****************************************/
/*
函数名称:Run(int Game_State,int x,int y)
函数介绍:
内部运行函数，控制光标运动，落子类型
	2 	光标向上运动一格
	8 	光标向下运动一格
	4 	光标向左运动一格
	6 	光标向右运动一格
	5	 落子
	0	 悔棋
输入参数:游戏状态Gamec_State
				开始游戏是光标的坐标(x,y)
输出参数:无
返回值:游戏状态1(白胜)或2(黑胜)
*/
int Run(int Game_State,int x,int y)
{
	sum=2;
	clrscr();
	Init();
	Body[x][y]=4;
	Pan_draw(x,y,sum);
while (Game_State==4)
{
/***************************************/
	if (Game==2&&flag1==1)
	{
			Init();
		    SC();
/****************************************/
		temp=flag1;
		flag1=flag2;
		flag2=temp;
/*****************************************/
		int ai=AI();
			
		x=JIE_BEST.x[ai];
		y=JIE_BEST.y[ai];
		Body[x][y]=flag1;
		if (End_Judge(x,y)) 
		{
	  	return flag1;
		}
		x=JIE_BEST.x[ai];
		y=JIE_BEST.y[ai];
		Record();
		sum++;
	/*	if (sum==8)
		{
			printf("%d %d %d",x,y,ai);
			exit(0);
		}*/
/**************************************/
	}
else
{
	char ch=getch();
	switch(ch)
	{
		case '2':
			if (x-1<1){x=15;}
			else {x--;}
			break;
		case '4':
			if (y-1<1){y=15;}
			else {y--;}
		    break;
		case '6':
			if (y+1>15){y=1;}
			else {y++;}
			break;
		case '8':
			if (x+1>15){x=1;}
			else {x++;}
			break;
		case '0':
			if (M>=0&&sum>2)
		   {
		   	M--;sum--;
		   	temp=flag1;
		   	flag1=flag2;
		   	flag2=temp;
		   }
		   break;
		case '5':
			Init();
		    SC();
		if (Body[x][y]!=1&&Body[x][y]!=2)
		{	    

/****************************************/
			temp=flag1;
			flag1=flag2;
			flag2=temp;
/****************************************/
			Body[x][y]=flag1;
	   	if (End_Judge(x,y)) 
			{
			return flag1;
			}
			Record();
			sum++;
		}
	
			break;
		default:break;
	}//switch
}
	clrscr();
	Init();
	SC();
	Body[x][y]=4;
	Pan_draw(x,y,sum);
}//while
	return 0;
}//Run
/****************************************/
/*函数名称:Record()
函数介绍:记录第M局的棋谱
输入参数:无
输出参数:无
返回值:无
*/
void Record()
{
	int i,j;
	M++;
	for (i=1;i<=15;i++)
	{
		for (j=1;j<=15;j++)
		{
			jBody[M][i][j]=Body[i][j];
		}
	}
}//Record
/****************************************/
/*函数名称:SC()
函数介绍:读取输出的M局的棋谱
输入参数:无
输出参数:无
返回值:无
*/
void SC()
{
	int i,j;
	for (i=1;i<=15;i++)
	{
		for (j=1;j<=15;j++)
		{
		Body[i][j]=jBody[M][i][j];
		}
	}
}//SC
/****************************************/
/*函数名称:End_Judge(int x,int y)
函数介绍:
判断最后一次落的子是否构成五连
输入参数:最后一次落子坐标(x,y)
输出参数:无
返回值:true 或false
*/
int End_Judge(int x,int y)
{
	int i,j;
	int n=0;
	for (i=-1;i<=0;i++)
	{
		for (j=-1;j<=1;j++)
		{
			if ((i!=0||j!=0)&&(i!=0||j!=1))
			{
			n=getSum(i,j,x,y)+getSum(-i,-j,x,y)+1;
			}
			if (n==5)
            {
	         	return 1;
			}
		}
	}
	return 0;
}//End_Judge
/****************************************/
/*函数名称:getSum(int i,int j,int x,int y)
函数介绍:
计算(i,j)方向上与(x,y)相邻且与Body[x][y]相等的部分的数量
输入参数:
方向坐标(i,j)最后一次落子坐标(x,y)
输出参数:无
返回值:
(i,j)方向上与(x,y)相邻且与Body[x][y]相等的部分的数量t
*/
int getSum(int i,int j,int x,int y)
{
	int X,Y;
	int t=0;
	X=x;
	Y=y;
	X=X+i;
	Y=Y+j;
	while (X>=1&&X<=15&&Y>=1&&Y<=15)
	{
	if (Body[X][Y]==Body[x][y])
	{
		t++;
	}
	else 
	{
		return t;
	}
	X=X+i;
	Y=Y+j;
	}//while
	return 0;
}//getSum
/****************************************/
/*函数名称:void End(int Game_State)
函数介绍:结束界面构化
输入参数:无
输出参数:无
返回值:无
*/
void End(int Game_State)
{
	clrscr();
			gotoxy(1,0);
	printf("\033[41;44;5m                                                \033[0m");
			gotoxy(1,2);
	printf("\033[41;45;5m                                                \033[0m");
			gotoxy(1,3);
	printf("\033[41;46;5m                                                \033[0m");
	          gotoxy(1,4);
		printf("\033[44;43;5m                                                \033[0m");
			gotoxy(20,6);
if (Game_State==2)
{
	printf("\033[44;41;5m白方胜！\033[0m");
	gotoxy(20,8);
printf("\033[44;41;5m黑方负！\033[0m");
}
else if (Game_State==1)
{
	printf("\033[44;41;5m黑方胜！\033[0m");
	gotoxy(20,8);	printf("\033[44;41;5m白方负！\033[0m");
}
			gotoxy(15,10);
printf("\033[44;41;5m游戏结束loading....\033[0m");
			gotoxy(1,13);
	printf("\033[41;43;5m                                                \033[0m");
			gotoxy(1,14);
	printf("\033[41;46;5m                                                \033[0m");
			gotoxy(1,15);
	printf("\033[41;45;5m                                                \033[0m");
	         gotoxy(1,16);
		printf("\033[44;44;5m                                                \033[0m");
}
/****************************************/
/*函数名称:int AI()
函数介绍:电脑判断落子地方
输入参数:无
输出参数:无
返回值:最佳坐标best_jie
*/
int AI()
{
	int i,j;
	int m,n;
	int Jie[9],jie=0;
	int style=0;
	int max_jie=0;
	int best_jie=0;
	int best_x=0;
	int best_y=0;
	best=0;
	for (i=0;i<=230;i++)
	{
		JIE_BEST.x[i]=0;
		JIE_BEST.y[i]=0;
	}
	for (m=1;m<=15;m++)
	{
		for (n=1;n<=15;n++)
		{
			if (Body[m][n]!=1&&Body[m][n]!=2)
			{
				
				for (i=1;i<=8;i++)
				{
					Jie[i]=0;
				}
				jie=0;
				int k1=getType(m,n,flag1);
				int k2=getType(m,n,flag2);
				max_jie=getMark(k1,flag1)>getMark(k2,flag2)?getMark(k1,flag1):getMark(k2,flag2);
	
				best++;
				Best[best].BestJie=max_jie;
				Best[best].x[0]=m;
				Best[best].y[0]=n;
			}
		}
	}
	best_x=1;
	best_y=1;
	JIE_BEST.BestJie=Best[1].BestJie;
	JIE_BEST.x[best_x]=Best[1].x[0];
	JIE_BEST.y[best_y]=Best[1].y[0];
	for (i=2;i<=best;i++)
	{
		if (Best[i].BestJie>JIE_BEST.BestJie)
		{
			best_x=1;
			best_y=1;
			JIE_BEST.BestJie=Best[i].BestJie;
			JIE_BEST.x[best_x]=Best[i].x[0];
			JIE_BEST.y[best_y]=Best[i].y[0];
		}
		else if (Best[i].BestJie==JIE_BEST.BestJie)
		{
			best_x++;
			best_y++;
			
			JIE_BEST.x[best_x]=Best[i].x[0];
			JIE_BEST.y[best_y]=Best[i].y[0];
		}
	}
	srand(time(0));
	best_jie=rand()%best_x+1;
	clrscr();	Pan_draw(JIE_BEST.x[best_jie],JIE_BEST.y[best_jie],sum);
	return best_jie;
}

//----------------------------计算指定方位上的棋型-------------------//
   // x,y 方向线基准一点。
   //ex,ey 指定方向步进向量。
   // k 棋子颜色，0：黑色，1：白色
   // 该方向上的棋子数目 以及 活度
 int count(int qz_x, int qz_y, int ex, int ey,int bwf)
  {
  	// 该方向没意义,返回0
      if( !makesense(qz_x, qz_y, ex, ey))
      {
          return 0;
      }
      
      // 正方向 以及 反方向棋子个数
  	int rt_1 = 1,rt_2 = 1;
  	// 总棋子个数
  	int rt = 1;
  	// 正方向 以及 反方向连子的活度
      int ok_1 = 0,ok_2 =0;
      // 总活度
      int ok = 0;
      // 连子中间有无空格
      int flag_mid1 =0,flag_mid2 = 0;
      // 连子中间空格的位置
      int flag_i1 = 1,flag_i2 = 1;
      
      int i0;
      // 往正方向搜索
      for (i0 = 1; qz_x + i0 * ex <=15 && qz_x + i0 * ex >= 1 && qz_y + i0 * ey <=15 && qz_y + i0 * ey >= 1; i0++)
       {
          if (Body[qz_x + i0 * ex][qz_y + i0 * ey] == bwf)
          {
 			 rt_1++;
          }
      	// 位置为空,若中空标志为0,则记为中空并继续搜索  否则,break
          else if (Body[qz_x + i0 * ex][qz_y + i0 * ey] == 3||Body[qz_x + i0 * ex][qz_y + i0 * ey] == 0) 
          {
          		if (flag_mid1==0)
          		 {
          			flag_mid1 = 1;
          			flag_i1 = i0;
          		}
          		else 
          		{
          			break;
          		}
          	}
          // 位置为对方棋子
          else    
          {
          	break;
          }
      }
      // 计算正方向活度,,
      // 最后一个位置不超过边界
      for (i0 = 1; qz_x + i0 * ex <=15 && qz_x + i0 * ex >= 1 && qz_y + i0 * ey <= 15 && qz_y + i0 * ey >= 1; i0++)  
      {
      	// 最后一个位置为空位 +1活
 if ((Body[qz_x + i0 * ex][qz_y + i0*ey] == 3)||(Body[qz_x + i0 * ex][qz_y + i0 * ey] == 0))
 {
      		ok_1++;
      		// 若是在尾部检测到连续的空格而退出搜索,则不算有中空
              if (rt_1 == flag_i1)
              {
      			flag_mid1 = 0;
              }
              // 若中空的位置在4以下 且 棋子数>=4,则这一边的4非活
              if (flag_mid1 && rt_1 > 3 && flag_i1 < 4) {
              	ok_1--;
              }
  	}
      	// 最后一个位置不是空格,且搜索了2步以上,若前一个是空格,  则不算中空,且为活的边
      	else if ( Body[qz_x + i0 * ex][qz_y + i0 * ey] != bwf && i0 >= 2) 
      	{
          	if (Body[qz_x + (i0-1) * ex][qz_y + (i0-1) * ey] == 3||Body[qz_x + (i0-1) * ex][qz_y + (i0-1) * ey] == 0)
          	 {
          		ok_1++;
          		flag_mid1 = 0;
          	}
      }
      // 最后一个位置是边界  搜索了2步以上,且前一个是空格,  则不算中空,且为活的边
      else if (i0 >= 2 && Body[qz_x + (i0-1) * ex][qz_y + (i0-1) * ey] == 3||Body[qz_x + (i0-1) * ex][qz_y + (i0-1) * ey] == 0) 
      {
      	ok_1++;
      	flag_mid1 = 0;
      }
      
      // 往反方向搜索        
      for (i0 = 1; qz_x - i0 * ex >= 1 && qz_x - i0 * ex <=15 && qz_y - i0 * ey >= 1 && qz_y - i0 * ey <= 15; i0++) {
          if (Body[qz_x - i0 * ex][qz_y - i0 * ey] == bwf)
              rt_2++;
          else if (Body[qz_x - i0 * ex][qz_y - i0 * ey] == 3||Body[qz_x - i0 * ex][qz_y - i0 * ey] == 0) {
          		if(!flag_mid2) {
          			flag_mid2 = 1;
          			flag_i2 = i0;
          		}
          		else
          			break;
          	}
          else
              break;
      }
      // 计算反方向活度
      for (i0 = 1; qz_x + i0 * ex <=15 && qz_x + i0 * ex >= 1 && qz_y + i0 * ey <= 15 && qz_y + i0 * ey >= 1; i0++)  {
      	 if (Body[qz_x + i0 * ex][qz_y + i0 * ey] == 3||Body[qz_x + i0 * ex][qz_y + i0 * ey] == 0) {
      		ok_2++;
      		if(rt_2 == flag_i2)
      			flag_mid2 = 0;
      	    if(flag_mid2 && rt_2 > 3 && flag_i2 < 4) {
              	ok_2--;
              }
      	}
      	else if( Body[qz_x - i0 * ex][qz_y - i0 * ey] != bwf && i0 >= 2 ) 
      	{
      		if (Body[qz_x - (i0-1) * ex][qz_y - (i0-1) * ey] == 3||Body[qz_x - (i0-1) * ex][qz_y - (i0-1) * ey] == 0) {
      			ok_2++;
      			flag_mid2 = 0;
      		}
      }
      else if (i0 >= 2 && (Body[qz_x - (i0-1) * ex][qz_y - (i0-1) * ey] == 3||Body[qz_x - (i0-1) * ex][qz_y - (i0-1) * ey] == 0)) {
      	ok_2++;
  		flag_mid2 = 0;
      }
      
      //------------------分析棋子类型
      // 两边都没中空,直接合成
      if ( flag_mid1==0 && flag_mid2==0 )
       {
      	rt = rt_1 + rt_2 - 1;
      	ok = ok_1 + ok_2;
      	return rt*10+ok;
      }
      // 两边都有中空
      else if ( flag_mid1 && flag_mid2 )
      {
      	int temp = flag_i1 + flag_i2 - 1;
      	// 判断中间的纯连子数,在5以上,直接返回;  为4,返回活4;  
      	if (temp >= 5)
      	{
      		return temp*10+2;
      	}
      	else if (temp == 4) 
      	{
      		return temp*10+2;
      	}
      	// 先看有没死4,再看有没活3,剩下只能是死3
      	else if (rt_1 + flag_i2 - 1 >= 4 || rt_2 + flag_i1 - 1 >= 4) 
      	{
      		return 4*10+1;
      	}
      	else if (rt_1+flag_i2-1 == 3 && ok_1 > 0 || rt_2+flag_i1-1 == 3 && ok_2 > 0)
      	{
      		return 3*10+2;
      	}
      	return 3*10+1;
      }
      // 有一边有中空
      else {
      	// 总棋子数少于5,直接合成
      	if ( rt_1 + rt_2 - 1 < 5 )
      		return (rt_1 + rt_2 - 1)*10+(ok_1 + ok_2);
      	// 多于5,先找成5,再找活4,剩下的只能是死4
      	else {
      		if (flag_mid1 && rt_2 + flag_i1 - 1 >= 5) 
      		{
      			return (rt_2 + flag_i1 - 1)*10+(ok_2 + 1);
      		}
      		else if (flag_mid2 && rt_1 + flag_i2 - 1 >= 5) 
      		{
      			return (rt_1 + flag_i2 - 1)*10+(ok_1 + 1);
      		}
      		else if (flag_mid1 && (rt_2 + flag_i1 - 1 == 4 && ok_2 == 1 || flag_i1 == 4) )
      		{
      			return 4*10+2;
      		}
      		else if (flag_mid2 && (rt_1 + flag_i2 - 1 == 4 && ok_1 == 1 || flag_i2 == 4) )
      	{
      			return 4*10+2;
      	}
      		
      		return 4*10+2;
      	}
      }
      }
 }
  }
  //----------------------------判断指定方向下棋是否有意义,即最大可能的棋子数是否 >=5-------------------------------//
   // x,y 评估的基准点
   // ex,ey 方向向量
   // k 棋色
   // true:有意义 false:没意义
    int makesense(int qz_x, int qz_y, int ex, int ey)
  {
      int rt = 1;
      for (int i0 = 1; qz_x + i0 * ex <= 15 && qz_x + i0 * ex >= 1 && qz_y + i0 * ey <= 15 && qz_y + i0 * ey >= 1&& rt < 5; i0++)
      {
          if (Body[qz_x + i0 * ex][qz_y + i0 * ey] != flag2)
          {
              rt++;
          }
          else
          {
              break;
          }
      }
      for (int i0 = 1; qz_x - i0 * ex >= 1 && qz_x - i0 * ex <= 15 && qz_y - i0 * ey >= 1 && qz_y - i0 * ey <= 15 && rt < 5; i0++)
      {
          if (Body[qz_x - i0 * ex][qz_y - i0 * ey] != flag2)
          {
              rt++;
          }
          else
          {
              break;
          }
      }
      return (rt >= 5);
  }
  
  
  //------------------------------------ 棋型判别-------------------------------------//
   // x,y 落子位置
   // bwf 棋色  0：黑子，1：白子
   // 对应的棋型： 棋型代码对应如下：
   //             1：成5
   //             2：成活4或者是双死4或者是死4活3
   //             3：成双活3
   //             4：成死3活3
   //             5：成死4
   //             6：单活3
   //             7：成双活2
  //             8：成死3
   //            9：成死2活2
   //            10：成活2
   //             11：成死2
   //             12: 其他
   //             20: 长连禁手
   //             21: 双四禁手
   //            22: 双活三禁手

  int getType(int qz_x, int qz_y,int bwf) {
  	if (Body[qz_x][qz_y] == 1||Body[qz_x][qz_y] == 2)
  	{
          return -1;
  	}
  	int types[4];
  	types[0] = count(qz_x, qz_y, 0, 1,bwf);   // 竖直
      types[1] = count(qz_x, qz_y, 1, 0,bwf);   // 横向
      types[2] = count(qz_x, qz_y, -1, 1,bwf);  // 斜上
      types[3] = count(qz_x, qz_y, 1, 1,bwf);   // 斜下
      // 各种棋型的方向的数目
      int longfive = 0;
      int five_OR_more = 0;
      int four_died = 0, four_live = 0;
      int three_died = 0, three_live = 0;
      int two_died  = 0, two_live = 0;
      // 各方向上棋型的判别
      for (int k = 0; k < 4; k++) {
      	if (types[k]/10 > 5) {  
      		longfive++;              // 长连
      		five_OR_more++;
      	}
      	else if (types[k]/10 == 5)
      		five_OR_more++;          // 成5
          else if (types[k]/10 == 4 && types[k]%10 == 2)
          	four_live++;             // 活4
          else if (types[k]/10 == 4 && types[k]%10 != 2)
          	four_died++;             // 死4
          else if (types[k]/10 == 3 && types[k]%10 == 2)
          	three_live ++;           // 活3
          else if (types[k]/10 == 3 && types[k]%10 != 2)
          	three_died++;            // 死3
          else if (types[k]/10 == 2 && types[k]%10 == 2)
          	two_live++;              // 活2
          else if (types[k]/10 == 2 && types[k]%10 != 2)
          	two_died++;              // 死2
          else
              ;
      }
      // 总棋型的判别
      if(bwf== 1 /*&& able_flag*/) {  		// 黑棋且选择有禁手
      	if (longfive != 0)        		// 长连禁手
      		return 20;
      	if (four_live + four_died >=2)  // 双4禁手
      		return 21;
      	if (three_live  >=2)        	// 双活三禁手
      		return 22;
      }
      if (five_OR_more != 0)
          return 1;   // 成5
      if (four_live != 0 || four_died >= 2 || four_died != 0 && three_live  != 0)
          return 2;   // 成活4或者是双死4或者是死4活3
      if (three_live  >= 2)
          return 3;   // 成双活3
      if (three_died != 0 && three_live  != 0)
          return 4;   // 成死3活3
      if (four_died != 0)
          return 5;   // 成死4
      if (three_live  != 0)
          return 6;   // 单活3
      if (two_live >= 2)
          return 7;   // 成双活2
      if (three_died != 0)
          return 8;   // 成死3
      if (two_live != 0 && two_died != 0)
          return 9;   // 成死2活2
      if (two_live != 0)
          return 10;  // 成活2
      if (two_died != 0)
          return 11;  // 成死2
      return 12;
  }
  
  
   //------------------------------------- 不同棋型对应分数---------------------------------
   // k 棋型代号
   //return 对应分数
  /**************此处还需改进************/
  int getMark(int k,int bwf) {
  	if (bwf==2)
  	{
      switch (k) {
      case 1:                   
          return 100000;
      case 2:                   
          return 15000;
      case 3:
          return 2500;
      case 4:
          return 500;
      case 5:
          return 250;
      case 6:
          return 100;
      case 7:
          return 50;
      case 8:
          return 25;
      case 9:
          return 5;
      case 10:
          return 3;
      case 11:
          return 2;
      case 12:
       	  return 1;
      default:                     //禁手棋型
          return 0;
      }
  	}
      	if (bwf==1)
  	{
      switch (k) {
      case 1:                   
          return 90000;
      case 2:                   
          return 30000;
      case 3:
          return 5000;
      case 4:
          return 1000;
      case 5:
          return 500;
      case 6:
          return 200;
      case 7:
          return 100;
      case 8:
          return 50;
      case 9:
          return 10;
      case 10:
          return 5;
      case 11:
          return 3;
      case 12:
       	  return 2;
      default:                     //禁手棋型
          return 0;
      }
      }
  	}
  