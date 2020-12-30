/*
解救人质游戏
只要可以走到下面的小人那里就赢
by QAIU
*/
#define WIN if(x==56&&y==20) break; /*胜利条件*/
#define UP '2'
#define DOWN '5'
#define LEFT '4'
#define RIGHT '6'

#define MAXTIME 120 /*解救人质的最长时间*/
#include <conio.h>
#include <time.h>
#include <stdlib.h>
long t=1;
int x,y;
void quitgame(int);
void initscreen();
void JudgeKey();
void littlewin(int);
int main()
{
	JudgeKey();	
}

void littlewin(int WinGam/*1--Win game, 0--Lost game */)
{
	printf("\e[31;1m");
	clrscr();
	gotoxy(1,6);
	printf("-----------------------------------------");
	gotoxy(1,1);
	if(WinGam)
	{
		printf("=============== Win Game ================");
		gotoxy(12,2);
		printf("Congratulations!");
		gotoxy(7,3);
		printf("You have Save the Hostage!");
		gotoxy(8,4);
		printf("Press any key to quit...");
		gotoxy(20,5);
		printf("%c",1);
	}
	else
	{
		printf("============== Lost Game ================");
		gotoxy(14,2);
		printf("Very Sorry!");
		gotoxy(2,3);
		printf("You have no time to Save the Hostage!");
		gotoxy(6,4);
		printf("Press any key to restart...");
	}
	getch();
	
}
void initscreen()
{
	int i,j;
	char ch;
	clrscr();
	printf("\e[35m");
	for(i=16;i<=56;i++)
	{
		for(j=4;j<=18;j+=2)
		{
			gotoxy(i,j);
			printf("%c",'#');
		}
		gotoxy(i,21);
		printf("%c",'#');
	}
	for(i=4;i<=21;i++)
	{
		gotoxy(15,i);
		printf("%c",'#');
		gotoxy(57,i);
		printf("%c",'#');
		gotoxy(58,i);
		printf("%c",'#');
		gotoxy(14,i);
		printf("%c",'#');
	}
	printf("\e[33m");
	gotoxy(16,5);
	printf("@");;
	gotoxy(56,20);
	printf("\e[33;1m>\e[0m");
	gotoxy(16,5);
	printf("\e[37m");
	gotoxy(10,22);
	printf("Welcome to this little Saving Hostage Game!");
	gotoxy(6,23);
	printf("All you have to do is finding a way to the Red Person.");
	gotoxy(18,24);
	printf("Press any key to start, Q to quit...");
	ch=getch();
	if(ch=='Q'||ch=='q')
		quitgame(0);
	else
	{
		gotoxy(18,24);
		printf("                                    ");
	}
	printf("\e[36m");
	gotoxy(10,1);
	printf("'2'(UP) '5'(DOWN) '4'(LEFT) '6'(RIGHT) 'q'(QUIT)\n");
	printf("\e[33m");
	gotoxy(18,2);
	printf("You have only %d seconds!",MAXTIME);
	gotoxy(46,2);
	printf("TIME:"); /*图象初始化结束*/
	x=16;
	y=5;
	t=0;
	return;	
}
void quitgame(int Conf/*1--Confirm, 0--no confirm*/)
{
	char ch;
	if(Conf)
	{
		printf("\e[31;1m");
		clrscr();
		gotoxy(1,6);
		printf("-----------------------------------------");
		gotoxy(1,1);
		printf("-------------- Exit Game ----------------");
	    printf("\e[35m");
		gotoxy(16,2);
		printf("Warning!");
		gotoxy(2,3);
		printf("Do you really want to quit this game?");
		printf("\e[36m");
		while(1)
		{
			gotoxy(15,4);
			printf("OK? [Y/N] ");
			ch=getch();
			if(ch=='Y'||ch=='y')
				break;
			else if(ch=='N'||ch=='n')
			{
				JudgeKey();
				return;
			}
		}
	}
	clrscr();
	exit(0);
}
void JudgeKey()
{
		printf("\033[?25l"); //隐藏光标
	int key,a,b,n=0;
	int m;
	initscreen();
	for(;;)
	{
		gotoxy(51,2);
		printf("        ");
		for(;!kbhit();) /*计时器*/
		{
			gotoxy(51,2);
			if(t%1000==0)
				m=t/1000;
				setbuf(stdout,NULL);
			printf("%d",m);
			gotoxy(1,1);
			setbuf(stdout,NULL);
			t++;
			if(m==MAXTIME)
			{
				littlewin(0);
				JudgeKey();
			}
			usleep(1000);
		} /*计时器结束*/
		++n;
		a=x;
		b=y;
		{
		key=getch();
		if(key==UP) /*判断按键*/
		{
			if(y==5)
			continue;
			gotoxy(x,--y);
			WIN;
		}
		else if(key==DOWN)
		{
			if(y==20)
				continue;
			else if(y==5&&x!=56)
				continue;
			else if(y==7&&x!=46)
				continue;
			else if(y==9&&x!=36)
				continue;
			else if(y==11&&x!=26)
				continue;
			else if(y==13&&x!=16)
				continue;
			else if(y==15&&x!=21)
				continue;
			else if(y==17&&x!=51)
				continue;
			gotoxy(x,++y);
			WIN;
		}
		else if(key==LEFT)
		{
			if(x==16)
			continue;
			gotoxy(--x,y);
			WIN;
		}
		else if(key==RIGHT)
		{
			if(x==56)
			continue;
			gotoxy(++x,y);
			WIN;
		}
		else if(key=='q')
			quitgame(1);
		else continue; /*判断结束*/
		printf("\e[1;34;47m@\e[0m"); /*输出图形*/
		gotoxy(a,b);
		printf(" ");
		gotoxy(x,y);		
	}
	}
	/*输出结果*/
	littlewin(1);
	quitgame(0);
}