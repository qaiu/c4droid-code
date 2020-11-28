/*c4droid代码手册
 *消灭星星
 *wangtianyu00编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include<stdio.h>
#include<time.h>
#include<stdlib.h> 
#include<conio.h>

int m;//颜色寄存变量
int ans;//深度搜索标记
int score=0,sm=0,st=0,sp=0;//分数
int x=14,y=1;//坐标
int a[9][9];//方块矩阵


int game();//游戏主体
int start();//初始化，生成随机阵列
int mark();//计分
int draw();//绘图
int board();//检测键盘，控制光标
int elim(int ,int );//清除选定方块及周围同色块
int simp();//重置清除后的方块阵列
int gover();//检测游戏结束


int main()
{
	while(1)
	{
	srand(time(NULL));//随机数种子
	start();//初始化，生成随机阵列
	if(gover()==1)
	{
	game();//游戏主体
	}
	}
}
//游戏主体
int game()
{
	score=sp=st=sm=0;//分数初始化
	while(gover())
	{
	clrscr();    //清屏命令
	draw();      //绘图函数
	gotoxy(x,y); //重新定位光标命令
	board();     //采集键盘输入值函数
	simp();      //重置图形函数
	mark();      //计分函数
	}
	clrscr();    //清屏命令
	draw();      //绘图函数
	gotoxy(x,y); //重新定位光标命令
	board();     //采集键盘输入值函数
}
//初始化，生成随机阵列
int start()
{
	int i,j,rd;
	for(i=1;i<=8;i++)
	{
	for(j=1;j<=8;j++)
	{
	rd=1+(int)(4.0*rand()/(RAND_MAX+1.0));
	a[i][j]=rd;//给矩阵随机赋值
	}
	}
}

//绘图
int draw()
{
	int i,j;
/****************屏幕扫描******************/
	for(i=1;i<=8;i++)
	{
/***************1屏幕左侧******************/
	if(i==1)//第一行的输出
	{
	printf("\033[40;36m消灭星星    \033[0m");
	printf("◆");
	}
	else if(i==2)//第二行的输出
	{
		if(gover()==0)
		{
		printf("            ◆");
		}
		else
		{
		printf("分数：      ◆");
		}
	}
	else if(i==3)//第三行的输出
	{
		if(gover()==0)
		{
		printf("Game over!  ◆",score);
		}
		else
		{
		printf(" %-10d ◆",score);
		}
	}
	else if(i==4)//第四行的输出
	{
		if(gover()==0)
		{
		printf("总分:%-7d◆",score);
		}
		else
		{
		if(sp==4)
		{
		printf("消灭四颗星  ◆");
		}
		else if(sp==5)
		{
		printf("消灭五颗星  ◆");
		}
		else if(sp==6)
		{
		printf("消灭六颗星  ◆");
		}
		else if(sp==7)
		{
		printf("消灭七颗星  ◆");
		}
		else if(sp==8)
		{
		printf("消灭八颗星  ◆");
		}
		else if(sp==9)
		{
		printf("消灭九颗星  ◆");
		}
		else if(sp>=10)
		{
		printf("超神!!!!!!!!◆");
		}
		else
		{
		printf("            ◆");
		}
	}
	}
	else if(i==5)//第五行的输出
	{
		if(gover()==0)
		{
		printf("            ◆",score);
		}
		else
		{
		if(sp==4)
		{
		printf("得到六分    ◆");
		}
		else if(sp==5)
		{
		printf("得到十分    ◆");
		}
		else if(sp==6)
		{
		printf("得到十五分  ◆");
		}
		else if(sp==7)
		{
		printf("得到二十一分◆");
		}
		else if(sp==8)
		{
		printf("得到二十八分◆");
		}
		else if(sp==9)
		{
		printf("得到三十六分◆");
		}
		else if(sp>=10)
		{
		printf("超越神的杀戮◆");
		}
		else
		{
		printf("            ◆");
		}
	}
	}
	else if(i==6)//第六行的输出
	{
		if(gover()==0)
		{
		printf("回车重新开始◆",score);
		}
		else
		{
		printf("            ◆");
		}
	}
	else 
	{
	printf("            ◆");
	}
	
	
/***************2屏幕中间******************/
	for(j=1;j<=8;j++)
	{
	switch(a[i][j])
	{
	case 0:printf(" ");
	break;
	case 1:printf("\033[43;33m \033[0m");
	break;
	case 2:printf("\033[47;37m \033[0m");
	break;
	case 3:printf("\033[41;31m \033[0m");
	break;
	case 4:printf("\033[44;34m \033[0m");
	break;
	}
	}
	
/***************3屏幕右侧******************/
	if(i==1)//第一行的输出
	{
	printf("◆ * 操作提示 *\n");
	}
	else if(i==2)//第二行的输出
	{
	printf("◆  使用2,4,6,\n");
	}
	else if(i==3)//第三行的输出
	{
	printf("◆  8控制光标。\n");
	}
	else if(i==4)//第四行的输出
	{
	printf("◆  使用5消除\n");
	}
	else if(i==5)//第五行的输出
	{
	printf("◆  方块。\n");
	}
	else 
	{
	printf("◆\n");
	}
	}
	sp=0;
}

//检测键盘，控制光标
int board()
{
	int temp;
	temp=getch();
	if(gover()==0)//游戏结束判定
	{
	while(1)//循环检测回车
	{
	if(temp=='\n')
	return 1;
	temp=getch();
	}
	}
	else//游戏过程种按键检测
	{
	switch(temp)
	{
	case '5'://按五消除
	if(a[y][x-13]!=0)
	{
	m=a[y][x-13];
	ans=0;
	elim(y,x);
	}
	return 0;
	break;
	
	case '2'://按二向上
	y--;
	if(y<1)
	{
	y=1;
	}
	return 0;
	break;
	
	case '4'://按四向左
	x--;
	if(x<14)
	{
	x=14;
	}
	return 0;
	break;
	
	case '6'://按六向右
	x++;
	if(x>21)
	{
	x=21;
	}
	return 0;
	break;
	
	case '8'://按八向下
	y++;
	if(y>8)
	{
	y=8;
	}
	return 0;
	break;
	}
	}
}

//清除选定方块及周围同色块
int elim(int y,int x)
{
	ans++;
	if(a[y][x-13]==m&&ans>1)
	a[y][x-13]=0;
	if(y!=1&&a[y-1][x-13]==m)
	{
	elim(y-1,x);
	}
	if(x!=21&&a[y][x-13+1]==m)
	{
	elim(y,x+1);
	}
	if(y!=8&&a[y+1][x-13]==m)
	{
	elim(y+1,x);
	}
	if(x!=14&&a[y][x-13-1]==m)
	{
	elim(y,x-1);
	}
	return 0;
	
}

//重置清除后的方块阵列
int simp()
{
	int i,j;
//实现消除后的方块纵向合并
	for(i=8;i>=1;i--)
	{
	for(j=1;j<=8;j++)
	{
while((i!=1)&&(a[i][j]==0)&&(a[i-1][j]!=0))
	{
	a[i][j]=a[i-1][j];
	a[i-1][j]=0;
	i=8;
	}
	}
	}
//实现消除后的方块横向合并
	for(j=1;j<=8;j++)
	{
while((j!=8)&&(a[8][j]==0)&&(a[8][j+1]!=0))
	{
	for(i=1;i<=8;i++)
	{
	a[i][j]=a[i][j+1];
	a[i][j+1]=0;
	}
	j=1;
	}
	}
}


//计分函数
int mark()
{
	int i,j;
	for(i=1;i<=8;i++)
	{
	for(j=1;j<=8;j++)
	{
	if(a[i][j]==0)
	st++;
	}
	}
	score+=((st-sm)*(st-sm-1))/2;
	//这里是计算分数的核心
	sp=st-sm;
	sm=st;
	st=0;
}

//检测游戏结束
int gover()
{
	int i,j;
	int p,q;
	int n;
	int win=0;
	for(i=1;i<=8;i++)
	{
	for(j=1;j<=8;j++)
	{
	//循环内部
	if(a[i][j]!=0)
	{
	m=a[i][j];
	p=i;
	q=j;
	n=0;
	    //检测内部
	    while(n!=4||i!=p||j!=q)
		{
			n+=1;
			if(n>4)
			n=1;
			switch (n)
			{
			case 1:
			if(j!=1&&a[i][j-1]==m)
			{
				win+=1;
				j-=1;
				n=3;
			}
			continue;
			case 2:
			if(i!=8&&a[i+1][j]==m)
			{
				win+=1;
				i+=1;
				n=4;
			}
			continue;
			case 3:
			if(j!=8&&a[i][j+1]==m)
			{
				win+=1;
				j+=1;
				n=1;
			}
			continue;
			case 4:
			if(i!=1&&a[i-1][j]==m)
			{
				win+=1;
				i-=1;
				n=2;
			}
			continue;
			}
		}
		//退出检测
	i=p;
	j=q;
	}
	}
	//退出循环
	}
	if(win==0)
	return 0;
	else
	return 1;
}