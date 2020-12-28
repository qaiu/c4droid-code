				// ------头文件区---------
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
	// #include<windows.h>
	// ------宏定义区---------
#define MAP_H 9
#define MAP_W 9
	// ------结构体区---------
	// ------全局变量区-------
int g_map[MAP_H][MAP_W] = { 0 };

int g_get = 0;					/* 是否可以获取数字 */
int score = 0;					/* 分数 */
// ------全局函数区-------
int GameInit();					/* 游戏初始化 */
void GamePaint();				/* 游戏绘图 */
void GameClean();				/* 游戏清理 */
void GameMoveNum(int direct);	/* 数字移动 */
void GameUp();					/* 上移 */
void GameDown();				/* 下移 */
void GameLeft();				/* 左移 */
void GameRight();				/* 右移 */
void GameKey();					/* 按键输入 */
void GameOver();				/* 游戏结束 */
void GameGetNum();				/* 随机获取数字 */
void GameWin();					/* 游戏获胜 */
// ------主函数入口区------
int main()
{
	if (!GameInit())
	{
		printf("游戏初始化失败！");
		return -1;
	}
	while (1)
	{
		usleep(100000);
		GameKey();
		GameGetNum();
		GamePaint();
		GameOver();
		GameClean();
		GameWin();
	}
	return 0;
}

// ------游戏初始化入口------
int GameInit()
{
	/* 随机数 */
	srand(time(0));
	/* 说明:列-下标为奇数则打印"—" */
	/* 行-下标为偶数则打印"|" */
	/* 其余的打印空 */
	for (int i = 0; i < MAP_H; i++)
	{
		for (int j = 0; j < MAP_W; j++)
		{
			if (i % 2 == 0)
			{
				if (j % 2 != 0)
				{
					g_map[i][j] = -1;
				}
				else
				{
					g_map[i][j] = 0;
				}
			}
			else
			{
				if (j % 2 == 0)
				{
					g_map[i][j] = -2;
				}
				else
				{
					g_map[i][j] = 0;
				}
			}
		}
	}
	g_get = 1;
	GameGetNum();
	g_get = 1;
	GameGetNum();
	return 1;
}

// ------游戏绘图入口------
void GamePaint()
{
	/* 绘制地图 */
	for (int i = 0; i < MAP_H; i++)
	{
		for (int j = 0; j < MAP_W; j++)
		{
			switch (g_map[i][j])
			{
			case -1:
				printf("%4s", "---");
				break;
			case -2:
				printf("%4c", '|');
				break;
			case 0:
				printf("%4c", ' ');
				break;
			case 2:
				printf("%4d", 2);
				break;
			case 4:
				printf("%4d", 4);	// 不同数字对应不同颜色
				break;
			case 8:
				printf("%4d", 8);
				break;
			case 16:
				printf("%4d", 16);
				break;
			case 32:
				printf("%4d", 32);
				break;
			case 64:
				printf("%4d", 64);
				break;
			case 128:
				printf("%4d", 128);
				break;
			case 256:
				printf("%4d", 256);
				break;
			case 512:
				printf("%4d", 512);
				break;
			case 1024:
				printf("%4d", 1024);
				break;
			case 2048:
				printf("%4d", 2048);
				break;
			}
		}
		printf("\n");
	}
	printf("\n你当前分数为：%d\n", score);
}

void GameClean()
{
	// system("cls");
	clrscr();
}

// ------游戏数字获取入口------
void GameGetNum()
{
	int num_x = 0, num_y = 0;
	if (g_get == 1)
	{
		while (1)
		{
			/* 随机获取数字位置 */
			num_x = rand() % MAP_W;
			num_y = rand() % MAP_H;
			if ((num_x % 2 != 0) && (num_y % 2 != 0) && (g_map[num_y][num_x] == 0))
			{
				g_map[num_y][num_x] = 2;
				g_get = 0;
				break;
			}
		}
	}
}

// ------数字移动函数入口------
/* direct：2上 4左 6右 8下 */
void GameMoveNum(int direct)
{
	switch (direct)
	{
	case 2:
		GameUp();
		break;
	case 8:
		GameDown();
		break;
	case 4:
		GameLeft();
		break;
	case 6:
		GameRight();
		break;
	}
}

// ------上移函数入口------
void GameUp()
{
	/* 分别表示：temp1-起始位置 */
	/* temp2-终止位置 */
	/* temp3-偏移大小 */
	int temp1 = 7, temp2 = 1, temp3 = 2;
	/* 跳转行 */
	for (int col = 1; col < 8; col += 2)
	{
		for (int i = 0; i < 4; i++)
		{
		  up:
			for (int j = temp1; j != temp2; j -= temp3)
			{
				if ((g_map[j - temp3][col] == 0) && (g_map[j][col] != 0))
				{
					g_map[j - temp3][col] = g_map[j][col];
					g_map[j][col] = 0;
				}
				else if ((g_map[j - temp3][col] == g_map[j][col]) && (g_map[j][col] != 0))
				{
					score += g_map[j - temp3][col];
					g_map[j - temp3][col] *= 2;
					g_map[j][col] = 0;
					goto up;
				}
			}
		}
	}
}

// ------下移函数入口------
void GameDown()
{
	/* 分别表示：temp1-起始位置 */
	/* temp2-终止位置 */
	/* temp3-偏移大小 */
	int temp1 = 1, temp2 = 7, temp3 = 2;
	/* 跳转行 */
	for (int col = 1; col < 8; col += 2)
	{
		for (int i = 0; i < 4; i++)
		{
		  down:
			for (int j = temp1; j != temp2; j += temp3)
			{
				if ((g_map[j + temp3][col] == 0) && (g_map[j][col] != 0))
				{
					g_map[j + temp3][col] = g_map[j][col];
					g_map[j][col] = 0;
				}
				else if ((g_map[j + temp3][col] == g_map[j][col]) && (g_map[j][col] != 0))
				{
					score += g_map[j + temp3][col];
					g_map[j + temp3][col] *= 2;
					g_map[j][col] = 0;
					goto down;
				}
			}
		}
	}
}

// ------左移函数入口------
void GameLeft()
{
	/* 分别表示：temp1-起始位置 */
	/* temp2-终止位置 */
	/* temp3-偏移大小 */
	int temp1 = 7, temp2 = 1, temp3 = 2;
	/* 跳转行 */
	for (int col = 1; col < 8; col += 2)
	{
		for (int i = 0; i < 4; i++)
		{
		  left:
			for (int j = temp1; j != temp2; j -= temp3)
			{
				if ((g_map[col][j - temp3] == 0) && (g_map[col][j] != 0))
				{
					g_map[col][j - temp3] = g_map[col][j];
					g_map[col][j] = 0;
				}
				else if ((g_map[col][j - temp3] == g_map[col][j]) && (g_map[col][j] != 0))
				{
					score += g_map[col][j - temp3];
					g_map[col][j - temp3] *= 2;
					g_map[col][j] = 0;
					goto left;
				}
			}
		}
	}
}

// ------右移函数入口------
void GameRight()
{
	/* 分别表示：temp1-起始位置 */
	/* temp2-终止位置 */
	/* temp3-偏移大小 */
	int temp1 = 1, temp2 = 7, temp3 = 2;
	/* 跳转行 */
	for (int col = 1; col < 8; col += 2)
	{
		for (int i = 0; i < 4; i++)
		{
		  right:
			for (int j = temp1; j != temp2; j += temp3)
			{
				if ((g_map[col][j + temp3] == 0) && (g_map[col][j] != 0))
				{
					g_map[col][j + temp3] = g_map[col][j];
					g_map[col][j] = 0;
				}
				else if ((g_map[col][j + temp3] == g_map[col][j]) && (g_map[col][j] != 0))
				{
					score += g_map[col][j + temp3];
					g_map[col][j + temp3] *= 2;
					g_map[col][j] = 0;
					goto right;
				}
			}
		}
	}
}

// ------按键函数入口------
void GameKey()
{
	char t;
	while (kbhit())
	{
		t = getch();
		switch (t)
		{
		case 'w':
			g_get = 1;
			GameMoveNum(2);
			break;
		case 's':
			g_get = 1;
			GameMoveNum(8);
			break;
		case 'a':
			g_get = 1;
			GameMoveNum(4);
			break;
		case 'd':
			g_get = 1;
			GameMoveNum(6);
			break;
		}
	}
}

// ------游戏结束函数入口------
void GameOver()
{
	/* 记录棋盘0的个数-棋盘0的个数总为16 */
	int temp = 1;
	for (int i = 0; i < MAP_H; i++)
	{
		for (int j = 0; j < MAP_W; j++)
		{
			if (g_map[i][j] == 0)
				temp++;
		}
	}
	if (temp == 26)
	{
		printf("游戏结束,你的分数为%d\n", score);
		getchar();
	}
	// printf("\n%d\n",temp);
}

// ------游戏获胜函数入口------
void GameWin()
{
	for (int i = 0; i < MAP_H; i++)
	{
		for (int j = 0; j < MAP_W; j++)
		{
			if (g_map[i][j] == 2048)
			{
				printf("你赢了\n");
				getchar();
			};
		}
	}
}