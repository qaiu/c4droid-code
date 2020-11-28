/*c4droid代码手册
*转载请注明出处
*赛车
*作者 鬼鬼 菜刀
*/
#include<stdio.h>
#include <conio.h>
#include<time.h>
#include<stdlib.h>
#define H 21
#define W 13
float N = 25000;
struct qj
{
	char ft;
	char fs;
} qj[H + 1][W];
int a = 18, b = 6;
int jf = 0, aaam = 0;
void HT()
{

	int i, j;
	for (i = 0; i < H; i++)
	{
		for (j = 0; j < W; j++)
		{
			switch (j)
			{
			case 0:
				qj[i][j].ft = '*';
				break;
			case 4:
			case 8:
				qj[i][j].ft = '|';
				break;
			case 12:
				qj[i][j].ft = '*';
				break;
			default:
				qj[i][j].ft = ' ';
				qj[i][j].fs = '0';
			}
			if (i == H - 1)
				qj[i + 1][j].ft = ' ';
			qj[i + 1][j].fs = '0';
		}
	}
}



void DF()						// 敌方赛车和移动
{
	int zz;
	srand((unsigned)time(NULL));
	zz = rand() % (3 - 1 + 1) + 1;
	qj[0][2 + (zz - 1) * 4].ft = '=';
	qj[1][1 + (zz - 1) * 4].ft = '[';
	qj[1][2 + (zz - 1) * 4].ft = '|';
	qj[1][3 + (zz - 1) * 4].ft = ']';
	qj[2][2 + (zz - 1) * 4].ft = '+';

	qj[0][2 + (zz - 1) * 4].fs = '5';
	qj[1][1 + (zz - 1) * 4].fs = '5';
	qj[1][2 + (zz - 1) * 4].fs = '5';
	qj[1][3 + (zz - 1) * 4].fs = '5';
	qj[2][2 + (zz - 1) * 4].fs = '5';

}

void YD()						// 移动
{
	int op, aa, bb;
	char z;
	qj[a][b].ft = ' ';
	qj[a + 1][b - 1].ft = ' ';
	qj[a + 1][b].ft = ' ';
	qj[a + 1][b + 1].ft = ' ';
	qj[a + 2][b - 1].ft = ' ';
	qj[a + 2][b].ft = ' ';
	qj[a + 2][b + 1].ft = ' ';
	// 清除
	if (kbhit())
	{
		z = getch();

		if (z == '4')
		{

			if (b != 2)
			{
				b = b - 4;
			}
		}
		else if (z == '6')
		{
			if (b != 10)
			{
				b = b + 4;
			}
		}

	};
	while (kbhit())
		z = getch();


	qj[a][b].ft = '+';
	qj[a + 1][b - 1].ft = '|';
	qj[a + 1][b].ft = '-';
	qj[a + 1][b + 1].ft = '|';
	qj[a + 2][b - 1].ft = '|';
	qj[a + 2][b].ft = '_';
	qj[a + 2][b + 1].ft = '|';
	for (aa = H; aa > 0; aa--)
		for (bb = W; bb > 0; bb--)
		{
			if (qj[aa - 1][bb - 1].fs == '5')
			{
				qj[aa][bb - 1] = qj[aa - 1][bb - 1];
				qj[aa - 1][bb - 1].ft = ' ';
				qj[aa - 1][bb - 1].fs = '0';
				if (aa == H)
					jf++;
			}
			if (aa == H - 1)
				qj[aa + 1][bb].ft = ' ';
		}

}



void CH()						// 重绘
{
	int i, j;
	for (i = 0; i < H; i++)
	{
		for (j = 0; j < W; j++)
		{
			if (i == H - 1 && j == W - 1)
			{

				printf("\033[40;32m%c\033[5m",qj[i][j].ft);
				printf(" 过的车数:%d", jf / 5);
			}
			else
				printf("%c", qj[i][j].ft);

		}
		printf("\n");
	}
}

void ZS()
{
	char pdks;
	printf("*********赛车 1.0*********\n");

	printf("1.开始游戏\n2.退出\n");
	printf("***********赛车***********\n");
	printf("*****制造人:鬼鬼 菜刀*****\n");
  sd:
	pdks = getchar();
	getchar();
	if (pdks == '2')
		exit(0);
	else if (pdks == '1')
	{
	}
	else
	{
		printf("输入错误!!请重新输入……");
		goto sd;
	}

}

void PDSW()						// 判断死亡
{
	char rmb;
	if (qj[a][b].fs == '5' || qj[a + 1][b - 1].fs == '5'
		|| qj[a + 1][b].fs == '5' || qj[a + 1][b + 1].fs == '5'
		|| qj[a + 2][b - 1].fs == '5' || qj[a + 2][b].fs == '5'
		|| qj[a + 2][b + 1].fs == '5')
	{
		usleep(1000 * 800);
		clrscr();
		printf("\n\tGAME OVER!!!\n\t你死了\n");

		printf("\t过的车数:%d", jf / 5);
		printf("\n\t输入0退出，其它键继续…\n");
		rmb = getchar();
		getchar();
		if (rmb == '0')
			exit(0);
		else
		{
			aaam = 9;
		}

	}
}

main()
{
	int num = 1, mn = 1;
	int M = 50;

	ZS();
	HT();
	// 画图
	do
	{
		if (N > 4500 && N < 4700)
			M = 5;
		if (N > 3000)
			N -= M;
		usleep(10 * N);
		if (mn == 1)
			DF();
		if (mn == 9)
			mn = 0;
		YD();					// 移动
		PDSW();
		if (aaam == 9)
		{
			aaam = 0;
			N = 25000;
			a = 18, b = 6;
			jf = 0;
			clrscr();		
			ZS();
			HT();
		}
		clrscr();				// 清屏
		CH();					// 重绘
		num++;
		mn++;
	}
	while (1);
}