/* 
 * 数字拼图v1.3 请自行调整字体大小
 * by 千百度 2017-6-5
 */
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#define W wh*wh
#define NUM(i,j) *(num +(i)*wh+j)

const char* str[] = {"０","１", "２", "３", "４", "５", "６", "７", "８","９"};

int p = 1, q = 1;
/* 获取随机矩阵 */
int randint(int *num, int wh)
{
	int w, t, k = 0, m = W;
	int a[W];
	for (int i = 0; i < m; i++)
	{
		a[i] = i + 1;
		w = rand() % (i + 1);
		t = a[i];
		a[i] = a[w];
		a[w] = t;
	}
	for (int i = m = 0; i < wh; i++)	// 将一维数组转化为二维数组
	{
		for (int j = 0; j < wh; j++)
		{
			NUM(i, j) = a[m++];
		}
	}
	for (int i = 0; i < W - 1; i++)	// 冒泡法获取逆序和
	{
		for (int j = i + 1; j < W; j++)
		{
			if (a[i] > a[j]) k++;
		}
	}
	for (int i = 0; i < wh; i++)
	{
		for (int j = 0; j < wh; j++)
		{
			if (NUM(i, j) == W)
			{
				m = 2 - j + 2 - i;
			}
		}
	}
	if ((k % 2 == 0 && m % 2 != 0) || (k % 2 != 0 && m % 2 == 0))
	{							// 根据逆序和的奇偶性判断矩阵是否有解
		randint(num, wh);		// 无解时递归调用直至有解
		return 0;
	}
}

// 绘制矩阵
void drawmap(int *num, int wh)
{
	printf("\033[40;33;1m\t数字拼图v1.3\033[0m\n\n");
	printf("\033[40;33m＊");
	for (int i = 0; i < wh; i++)
		printf("---");
	printf("\033[40;33m＊\n");
	for (int i = 0; i < wh; i++)
	{
		printf("\033[40;33m┃ ");
		for (int j = 0; j < wh; j++)
		{
			if (NUM(i, j) < W) {
				if (NUM(i, j) < 10) {
					printf("\033[4%d;3%d;2m%s\33[40m ", (NUM(i, j) + 1) % 7 + 1, NUM(i, j) % 7, str[NUM(i, j)]);
				} else {
					printf("\033[4%d;3%d;2m%d\33[40m ", (NUM(i, j) + 1) % 7 + 1, NUM(i, j) % 7, NUM(i, j));	
				}
				
			}else{
				printf("\033[40;37;5m  \033[0;40m ");	// 4个空格
			}
		}
		if (i < wh - 1){
			printf("\033[40;33m┃\n＊");
			for (int j = 0; j < wh; j++) {
				printf("   ");
			}
			printf("＊\n");
		}
		else
			printf("\033[40;33m┃");
	}
	printf("\n\033[40;33m＊");
	for (int i = 0; i < wh; i++)
		printf("---");
	printf("\033[40;33m＊\n\033[41;33m2 4 6 8 控制移动方向*重新开始\033[0m\n");
}

// 事件函数
void event(int *num, int *sum, int wh)
{
	while (kbhit())				// 判断输入，有则循环
	{
		int n, m, k;
		for (int i = 0; i < wh; i++)
		{
			for (int j = 0; j < wh; j++)
			{
				if (NUM(i, j) == W)
				{				// 空方块位置
					m = i;
					n = j;
				}
			}
		}
		int key = getch();		// 获取按键
		switch (key)
		{
		case '2':
			if (m != wh - 1)
			{
				NUM(m, n) = NUM(m + 1, n);	// 交换空方块
				NUM(m + 1, n) = W;
				(*sum)++;
			}
			break;
		case '4':
			if (n != wh - 1)
			{
				NUM(m, n) = NUM(m, n + 1);
				NUM(m, n + 1) = W;
				(*sum)++;
			}
			break;
		case '6':
			if (n != 0)
			{
				NUM(m, n) = NUM(m, n - 1);
				NUM(m, n - 1) = W;
				(*sum)++;
			}
			break;
		case '8':
			if (m != 0)
			{
				NUM(m, n) = NUM(m - 1, n);
				NUM(m - 1, n) = W;
				(*sum)++;
			}
			break;
		case '*':
			p = q = 0;
			break;
		}
	}
}

// 成功判定
int judge(int *num, int *sum, int wh)
{
	int k = 0;
	q = 1;
	for (int i = 0; i < W - 1; i++)
		if (*(num + i) > *(num + i + 1))
			k = 1;
	return k;
}

int main(void)					// 主函数
{
	int wh = 5, num[10][10], sum = 0;
	double i;
	srand(time(NULL));
	printf("\033[40m\033[2J");
	while (1)
	{
		do
		{
			printf("\033[40;36m欢迎来到数字拼图"
				   "\n请输入数字范围(对应难度2~10)：\n");
			do
			{
				scanf("%d", &wh);
				if (wh < 2 || wh > 10)
					printf("输入错误，请重新输入：\n");
			}
			while (wh < 2 || wh > 10);
			randint(*num, wh);
			i = 0;
			p = judge(*num, &sum, wh);
			while (p)
			{
				p = judge(*num, &sum, wh);
				clrscr();
				drawmap(*num, wh);
				printf("\033[37m时间:%g\n\033[40;32m"
					   "步骤:%ld\n", i++ / 100, sum);
				event(*num, &sum, wh);
				usleep(20000);
			}
			clrscr();
		}
		while (!q);
		drawmap(*num, wh);
		printf("成功！\n用时：%gs\n\033[40;32m步骤:%d\n"
			   "\033[40;36m任意键继续游戏，" "0退出：\033[0m\n", i / 100, sum);
		sum = 0;
		if (getch() == '0')
		{
			clrscr();
			printf("千百度制作\nQQ:736226400");
			return 0;
		}
	}
}