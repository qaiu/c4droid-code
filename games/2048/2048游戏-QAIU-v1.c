/*
**2048游戏v1.1
**by 千百度
**for C4droid
**2017/6/17  23:59
*/
#include <stdio.h>    //输出printf()
#include <string.h>    //内存拷贝memcpy()
#include <conio.h>    //清屏clrscr()
#include <time.h>      //随机数种子time();
#include <stdlib.h>    //随机数rand，初始化种子srand
#include <math.h>     //对数函数log()
int map[4][4],newmap[4][4]={0};//地图和空地图
int score=0,best=0;//当前分数和最高分

int logt(int n)//目标块的2的指数，用于控制颜色
{
	return log(n)/log(2);//对数函数换底公式
}

void initmap()//初始化地图，移动后随机位置生成数字2
{
	int kk,k=0;//kk随机生成2或4的几率，k随机空格的位置
	int *p[16];//指针数组储存二维数组的所有元素地址
	for (int i=0;i<4;i++)
	for (int j=0;j<4;j++)
	if (map[i][j]==0)
	{
		p[k]=&map[i][j];
		k++;//地图数组为0的元素为空格，k空格的最大数
	}
	k=rand()%k;//在k范围生成一个随机数
	kk=rand()%10;
	*p[k]=kk<9?2:4;//有90%几率生成2，10%生成4
}
void drawmap()//画地图
{

	printf("\t\t\033[40;37;1m2048游戏v1.1\033[0m\n\n\t\033[40;36m 当前分数:%5d\n\t\033[33m 最高纪录:%5d\n\t\033[40;32m┏━━━━━━━━━━━━━━━━━━━━\033[1m┓\n\t│",score,best);
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		if (map[i][j])
		printf(" \033[4%d;3%d;1m%4d\033[0m",(logt(map[i][j])+1)%7+1,logt(map[i][j])%7,map[i][j]);//加入了颜色代码long2N控制颜色
		else
		printf(" \033[40;37;5m    \033[0m");
		if (i<3)
		printf("\033[32;1m│\n\n\t│");
		else
			printf("\033[32m┃ \t");
	}
	printf("\033[40;32m\n\t┗━━━━━━━━━━━━━━━━━━━━\033[1m┛\033[0m  \n");
	printf("\t \033[41;37m2468 键控制方向;0重置\n\033[0m");
}

void init()
{
	clrscr();
	drawmap();
	int a[4][4]={0},b[4][4]={0};
	int key = getch();//获取按键
	switch (key)
	{
		case '2'://向上移动
    	for (int i=0;i<4;i++)//去除空格
    	{
    		for (int j=0,k=0;j<4;j++)
    		if (map[j][i])
    		a[k++][i]=map[j][i];
    	}
    	for (int i=0;i<4;i++)//合并相同数并相加
    	{
    		for (int j=0;j<3;j++)
    		if (a[j][i]&&a[j][i]==a[j+1][i])
    		{
    			a[j][i]+=a[j+1][i];
    			score+=a[j][i];
    			a[++j][i]=0;
    		}
    	}
    	for (int i=0;i<4;i++)//再次去除空格，下同不解释
    	{
    		for (int j=0,k=0;j<4;j++)
    		if (a[j][i])
    		b[k++][i]=a[j][i];
    	}
    	if(memcmp(map,b,sizeof(b)))//判断是否可移动
    	{
    		memcpy(map,b,sizeof(b));
			initmap();
		}
		break;
		case '4':
		for (int i=0;i<4;i++)
    	{
    		for (int j=0,k=0;j<4;j++)
    		if (map[i][j])
    		a[i][k++]=map[i][j];
    	}
    	for (int i=0;i<4;i++)
    	{
    		for (int j=0;j<3;j++)
    		if (a[i][j]&&a[i][j]==a[i][j+1])
    		{
    			a[i][j]+=a[i][j+1];
    			score+=a[i][j];
    			a[i][++j]=0;
    		}
    	}
    	for (int i=0;i<4;i++)
    	{
    		for (int j=0,k=0;j<4;j++)
    		if (a[i][j])
    		b[i][k++]=a[i][j];
    	}
    	if(memcmp(map,b,sizeof(b)))
    	{
    		memcpy(map,b,sizeof(b));
			initmap();
		}
		break;
		case '6':
		for (int i=0;i<4;i++)
    	{
    		for (int j=3,k=3;j>=0;j--)
    		if (map[i][j])
    		a[i][k--]=map[i][j];
    	}
    	for (int i=0;i<4;i++)
    	{
    		for (int j=3;j>0;j--)
    		if (a[i][j]&&a[i][j]==a[i][j-1])
    		{
    			a[i][j]+=a[i][j-1];
    			score+=a[i][j];
    			a[i][--j]=0;
    		}
    	}
    	for (int i=0;i<4;i++)
    	{
    		for (int j=3,k=3;j>=0;j--)
    		if (a[i][j])
    		b[i][k--]=a[i][j];
    	}
    	if(memcmp(map,b,sizeof(b)))
    	{
    		memcpy(map,b,sizeof(b));
			initmap();
		}
		break;
		case '8':
		for (int i=0;i<4;i++)
    	{
    		for (int j=3,k=3;j>=0;j--)
    		if (map[j][i])
    		a[k--][i]=map[j][i];
    	}
    	for (int i=0;i<4;i++)
    	{
    		for (int j=3;j>0;j--)
    		if (a[j][i]&&a[j][i]==a[j-1][i])
    		{
    			a[j][i]+=a[j-1][i];
    			score+=a[j][i];
    			a[--j][i]=0;
    		}
    	}
    	for (int i=0;i<4;i++)
    	{
    		for (int j=3,k=3;j>=0;j--)
    		if (a[j][i])
    		b[k--][i]=a[j][i];
    	}
    	if(memcmp(map,b,sizeof(b)))
    	{
    		memcpy(map,b,sizeof(a));
			initmap();
		}
    	break;
    	case '0':
    	memcpy(map,newmap,sizeof(newmap));
    	initmap();
    	initmap();
    	score=0;
    	break;
	}
}

int judge()//游戏结束判定
{
	for (int i=0;i<4;i++)
	for (int j=0;j<4;j++)//存在空格则游戏继续
		if(!map[i][j])
		return 1;
	for (int i=0;i<4;i++)//横向存在相邻的相同数字游戏继续
	for (int j=0;j<3;j++)
		if(map[i][j]==map[i][j+1])
		return 1;
	for (int i=0;i<4;i++)//纵向判断
	for (int j=0;j<3;j++)
		if(map[j][i]==map[j+1][i])
		return 1;
	clrscr();	
	drawmap();
	return 0;//游戏结束
}

int main(void)//主函数
{
	FILE *fp;
	char readline[256] = {'\0'};
	const char *filename = "2048best.dat";
	if ((fp = fopen(filename, "r")) != NULL)
    {
    	fgets(readline, 64, fp);
        best=atoi(readline);//读取最高纪录
        fclose(fp);
    }
    else
    fopen(filename, "w");
    printf("\033[?25l"); //隐藏光标
	star:
	srand(time(NULL));
	initmap();
	initmap();
	while (judge())
	{
		init();
		if (score>best)
    	{
		best=score;
		FILE *fp;
        char str[256];
        sprintf(str,"%d",best);
        if ((fp = fopen(filename, "w")) != NULL)//打开文件
        fputs(str,fp);//写最高纪录
        fclose(fp);
	}
	}
	printf("\t 游戏结束，任意键重新开始\n");
	getch();
	memcpy(map,newmap,sizeof(newmap));
	score=0;
	goto star;
}