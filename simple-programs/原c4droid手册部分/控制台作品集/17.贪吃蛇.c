/*c4droid代码手册
 *贪食蛇
 *啊o额iu鱼编写
 *TTHHR收集整理
 *转载请说明出处
*/
// 加了大量注释，只注释与游戏本身内容相关的，简单的函数的使用就不说明了……
//更新内容:
//v2.0解决横纵比不同的问题，两个并列的符号走起来难看，现在用有色背景，主要看颜色，就不觉得难看了……
//v1.2原帖36楼。修复了一个大bug，关于函数嵌套的，over函数中不断嵌套start函数会导致start函数始终不能返回，占内存不能释放，迟早内存溢出……
//v1.1原帖7楼30楼。修复了咬尾巴bug，增加了挡路石……
//v1.0原版……
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define H 16
// 高度high
#define W 24
// 宽度weight
#define N 4
// 难度,步时为(1/N)s
#define UP  '2'
#define DOWN '5'
#define LEFT '4'
#define RIGHT '6'
// 自定义方向键…
void move();
void start();
void draw();
void getturn();
void getrand(int &a, int &b);
void turn(char direction);
int judge();
void getfood();
void getrock();
void over();
	// 好多自定义函数…
int x, y;
	// 蛇头坐标…
// x横坐标即列数，y纵坐标即行数…
int l = 0;						// 蛇肚子长
int n, i, j, jg;
int x1, y1;
		// 食物和石头的坐标…
char *head, *tail, *tt;
		// 指向头尾，tt是临时的尾巴…
char map[H][W];
			// 用二维数组存全图…
char *belly[H * W], *food, *rock;	// 肚子食物和石头…
char left, right, front, back;
	// 本人用的是前后左右移动…
int main()
{
	for (;;)					// v1.2改…
		start();
	// 额，简洁明了的主函数...
}

void start()					// 开始…
{
	for (i = 0; i != H; ++i)
		for (j = 0; j != W; ++j)
		{
			if ((i == 0) || (i == H - 1) || (j == 0) || (j == W - 1))
				map[i][j] = '9';
			else
				map[i][j] = '0';
		}
	x = W / 2, y = H / 2, l = 0;
	head = &map[y][x + 1];
	belly[l] = &map[y][x];
	tail = &map[y][x - 1];
	*head = '1';
	*belly[0] = '2';
	*tail = '3';
	left = UP, right = DOWN;
	front = RIGHT, back = LEFT;
	turn(front);
	getfood();
	draw();
	// 前面都是初始化，现在才开始…
	do
	{
		usleep(1000000 / N);
		// 步时为(1/N)s
		getturn();
		// 读取拐弯方向…
		move();					// 移动…
		jg = judge();
		// 判断头移到那了…
		clrscr();				// 清屏…
		draw();					// 重绘…
	}
	while (jg);					// v1.2改，用返回值判断游戏是否结束…
	clrscr();
	return;						// 游戏结束就返回，这句有没有都无所谓…
}

void getturn()
{
	char t;
	while (kbhit())				// 判断有无输入值…
	{
		t = getch();			// 有则捕获…
		if (t == left)
			// 判断方向并拐弯…
		{
			turn(left);
			left = back;
			back = right;
			right = front;
			front = t;
			goto gett;
		}
		else if (t == right)
		{
			turn(right);
			right = back;
			back = left;
			left = front;
			front = t;
			goto gett;
		}
	}
	turn(front);
  gett:;
	while (kbhit())
		t = getch();
	// 把多输入的吃掉…
}

void getfood()					// 来个吃的…
{
	getrand(x1, y1);
	// 食物要落在空地上…
	food = &map[y1][x1];
	*food = '4';
	// 移指针并赋值…
	getrock();
	// 顺便来个石头…
	// 不喜欢可以去掉…
}

void getrock()
{
	getrand(x1, y1);
	rock = &map[y1][x1];
	// 要是正好石头就在面前只能说是运气不好了……
	*rock = '8';
}

void getrand(int &a, int &b)
{
	do
	{
		a = random() % (W - 2) + 1;
		b = random() % (H - 2) + 1;
	}
	while (map[b][a] != '0');
}

void turn(char direction)
// 转向，想不到更好的单词了，直走也算turn…
{
	switch (direction)
		// 蛇头坐标先走…
	{
	case UP:
		--y;
		break;
	case DOWN:
		++y;
		break;
	case LEFT:
		--x;
		break;
	case RIGHT:
		++x;
		break;
	}

}

void move()
// 蛇肚子走…
{
	tt = belly[l];
	// 先临时的尾巴走…
	for (i = l; i != 0; --i)	// 挪动…
		belly[i] = belly[i - 1];
	belly[0] = head;
	head = &map[y][x];
	// 蛇肚子先走才走蛇头…
	*belly[0] = '2';
	// 蛇肚子2了……
}

int judge()
{
	switch (*head)
	{
	case '2':;
	case '8':;
	case '9':
		over();
		return 0;				// v1.2改…
		// 咬肚子或撞墙死…
		// 咬尾巴不算，尾巴马上走了…
		break;
	case '4':
		getfood();
		// 吃了还有…
		++l;
		belly[l] = tt;
		// 伸长，真的尾巴就不用动了…
		*head = '1';
		// 蛇头跟上…
		break;
	case '0':
		*tail = '0';
	case '3':
		*head = '1';
		tail = tt;
		*tail = '3';
		// 没东西就让真尾巴也跟上…
		//v1.1改，解决咬尾巴bug…
		break;
	}
	return 1;
}

void draw()
// 绘图…//v2.0改输出颜色和宽度……
{
	for (i = 0; i != H; ++i)
	{
		for (j = 0; j != W; ++j)
			switch (map[i][j])
			{
			case '0':
				printf("  ");//v2.0改，两个空格，下面的也都是两个……
				break;
			case '9':
				printf("\033[47;36m##\33[40;37m");
				break;
			case '8':
				printf("\33[47;36m¤¤\33[40;37m");
				break;
				// 石头…
			case '1':
				printf("\033[41;33m@@\33[40;37m");
				break;
			case '2':
				printf("\33[42;33mOO\33[40;37m");
				break;
			case '3':
				printf("\33[42;34moo\33[40;37m");
				break;
			case '4':
				printf("\33[46;37m♥♥\33[40;37m");
				//还是宽度问题，如果有任何不对劲的地方，就把输出符号改下试试……
				break;
			}
		printf("\n");
	}
}

void over()
	// game over!!!
{
	printf("\tGame over!!!\n");
	printf("\t\tAgain?\n");
	getch();
	// 输入任意字符继续…
	clrscr();
	// start();
	// 重新…
	// v1.2改，原来的不断嵌套使用start函数不返回，会导致内存被占用越来越多，最后内存溢出…
}