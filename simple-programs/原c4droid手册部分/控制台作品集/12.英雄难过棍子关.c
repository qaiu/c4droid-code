/*c4droid代码手册
*英雄难过棍子关
*作者 鬼鬼
*转载请说明出处
*/
#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#define H 27
#define W 22
void Q();
void SJZ();
void GZ();
void CH();
void SW();
char qj[H][W] = { };

int a, b, c, d = 0;
int aaam = 1,jf=0;
char *rw;
void Q()
{
	int i, j;
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			if (i == 0 || i == H - 1 || j == 0 || j == W - 1)
				qj[i][j] = '9';
			else
				qj[i][j] = '0';
		}
}

void SJZ()
{
	int m, n;
	a = 0, b = 0;
	while (d < 2)
	{
		if (c == 0)
		{
			c = a;
		}
		srand((unsigned)time(NULL));
		a = rand() % (5 - 1 + 1) + 1;
		d++;}
		b = rand() % ((W - a - c-2) - 0 + 1) + 0;
	
	d = 1;
	for (m = 19; m < H-1; m++)
		for (n = 1; n < c + 1; n++)
			qj[m][n] = '5';
	for (m = 19; m < H-1; m++)
		for (n = c + b+1 ; n < c + b+1+a; n++)
			qj[m][n] = '6';
		rw = &qj[18][c];
	*rw = '2';
	clrscr();
			CH();
}

void GZ()
{
	int g, aa, m, n, bb;
	scanf("%d", &g);
	getchar();
	for (aa = 18; aa > 0 && aa > 18 - g; aa--)
		qj[aa][c+1] = '1';
		clrscr();
	CH();
	usleep(1000000);
	clrscr();
	CH();
	for (m = 0; m < H; m++)
		for (n = 0; n < W; n++)
			if (qj[m][n] == '1')
				qj[m][n] = '0';
	for (aa = c + 1; aa < 21 && aa < c + 1 + g; aa++)
		qj[18][aa] = '3';
	clrscr();
	CH();
		usleep(1000000);
	bb = c;
	for (;;)
	{
		++bb;
		*rw='3';
		rw = &qj[18][bb];
		switch (*rw)
		{
		case '3':
			*rw='2';
			break;
		case '0':
			if (qj[19][bb] == '6')
			{
			*rw='2';
				goto x;
				}
			else if (qj[19][bb] != '6')
			{
				SW();
				goto xx;
			}
			break;
		case '9':
			SW();
			goto xx;
			break;
		}
		clrscr();
		CH();
		usleep(200000);
	} x:;
	c = a;
	xx:;
}

void CH()
{
	int i, j;
	for (i = 0; i < H; i++)
	{
		for (j = 0; j < W; j++)
		{
			switch (qj[i][j])
			{
			case '0':
				printf("  ");
				break;
			case '1':
				printf("||");
				break;
			case '2':
				printf("㊣");
				break;
			case '3':
				printf("__");
				break;
				case '5':
							printf("\33[45;37m  \33[40;37m");
				break;
				case '6':
							printf("\33[44;37m  \33[40;37m");
				break;
			case '9':
				printf("\33[46;37m  \33[40;37m");
				break;
			}
		}
		if(i==H-1)
		printf("得分:%d",jf);
	printf("\n");
	}
}

void SW()
{
	char rmb;
	clrscr();
	printf("\n\tGAME OVER!!!\n\t你死了\n");
printf("\t得分:%d",jf);	printf("\n\t输入0退出，其它键继续…\n");
scanf("%c",&rmb);

	if (rmb == '0')
		exit(0);
	else
	{
		aaam = 9;
	}
}
void ZS()
{
	char pdks;
	sdd:;
	printf("*******英雄难过棍子关 1.0*******\n");

	printf("1.开始游戏\n2.帮助\n3.退出\n");
	printf("*********英雄难过棍子关*********\n");
  sd:;
	pdks = getchar();
	getchar();
	if (pdks == '2')
	{
	 clrscr();	
	 printf("\t\t帮助\n"); printf("进入游戏后输入数字输入的越大柱子就越长。。\n只能输入数字不然会出错，输完后要确认\n");
			usleep(4000000);
			 clrscr();
			goto sdd;
	}
	else if (pdks == '1')
	{
	}	
	else if (pdks == '3')
	{	exit(0);
	}
	else
	{
		printf("输入错误!!请重新输入……");
		goto sd;
	}

}

main()
{
	ZS();
	for(;;){
	Q();
    SJZ();
	 GZ();
	 clrscr();
	CH();
	 if(aaam==9)
	 {
	 	aaam=0;
	 	qj[H][W] = { };
 a=0, b=0, c=0, d = 0,jf=0;

	 }
	 ++jf;
	}
	
}