/*
扫雷v1.3
for c4droid任意编译器
©千百度QAIU 2017-6-27
最后调试于 2017-6-30 20:30
调试日志
v1.1解决了gcc++无法编译的bug
由于g++不支持变长数组做形参，故采用了全局变量做为地图
v1.2 改进了所有变长数组，默认编译器可编译
原来4个方向移动光标改为8个方向
改进主菜单及整体配色方案
v1.3
使用一套随机算法将第一下展开的格子周围的地雷随机分配到其他空白区域，保证首次展开的位置及周围一定是空格，以减少失败的运气成分
*/
#include  <stdio.h>
#include  <conio.h> 
#include  <stdlib.h> 
#include  <time.h> 
//生成随机地图
void generate_map(int m,  int n, int w); 
//递归展开空格
void open_blank_space(int x, int y, int w, int h);
//绘制地图
void draw_map(int w, int h); 
//初始化地图中的地雷数
void initmap_mine_num(int w, int h); 
//判断游戏胜利
void judge_win(int w, int h); 
//游戏失败
void gameover(int w, int h); 
//按键事件，控制移动
void key_event(int w, int h); 
//全局变量，储存方格周围的地雷雷数字符串数组
const char str[8][8] = {"１", "２", "３", "４", "５", "６", "７", "８"}; 
int init_mine_map[20][30] = {}; //原始地图
int print_mine_map[20][30] = {}; //要打印的地图
int void_mine_map[20][30] = {}; //空地图，用于置空初始地图
 int x=0, y=0; //游戏光标
int first_key = 1; //判断首次展开
int judge; //判断游戏结束
int game_time = 0; //游戏时间
int mine_num; //雷数
int mine_num_r; //去掉插旗的剩余雷数
int mine_num_b; //雷区空格数

int main(void)
{
	int w=0, h=0, key; 
	star:
	clrscr();
	w = h = mine_num = 0;
	printf("\033[1;36m\t\t**********************\n\t\t*      扫雷v1.3      *\n\t\t**********************\n\033[0m"); 
	printf("\033[40;33m\t\t请输入难度序号(任意键默认简单):\n\t\t1 简单(9*9*10)\n\t\t2 一般(16*16*40)\n\t\t3 困难(30*16*99)\n\t\t0 自定义(宽*高*雷数)\n\t\t\033[32m++++++++++++++++++++++++\n\t\t\033[32m4 帮助(关于)\n"); 
	key = getch(); 
	switch (key)
	{
		case '0': 
		clrscr(); 
		printf("\033[1;36m\t\t**********************\n\t\t*      自定义难度    *\n\t\t**********************\n\033[0;33m\t\t宽度范围(w): (8~30);\n\t\t高度范围(h): (8~18); \n\t\t地雷数量(n): (8~w*h*60%%);\n\t\t输入: w  h  n 回车确认\n\t\t比如: 15 12 9\n\t\t      \033[32m"); 
		while (w < 8 || w > 30 || h < 8 || h > 18 || mine_num < 8 || mine_num > w*h*0.6)
		{
			scanf("%d%d%d", &w, &h, &mine_num); 
			if (w < 8 || w > 30 || h < 8 || h > 18 || mine_num < 8 || mine_num > w*h*0.6)
			printf("\t\t\033[35m输入错误，请重新输入：\033[32m\n\t\t      "); 
		}
		break; 
		case '1':
			w = 9; h = 9; mine_num = 10; 
		break; 
		case '2':
			w = 16; h = 16; mine_num = 40; 	
		break; 
		case '3':
			w = 30; h = 16; mine_num = 99; 
		break; 
		case '4':
		clrscr();
		printf("\033[1;36m\t\t**********************\n\t\t*      扫雷帮助      *\n\t\t**********************\n\033[0;32m    扫雷就是要把所有非\033[47;30m地雷\033[0;32m的格子揭开即胜利\n    踩到\033[47;30m地雷\033[0;32m格子就算失败。\n    游戏主区域由很多个方格组成。按0键随机点击一个方格，方格即被打开并显示出方格中的数字；方格中数字则表示其周围的8个方格隐藏了几颗雷；如果点开的格子为空白格，即其周围有0颗雷，则其周围格子自动打开；如果其周围还有空白格，则会引发连锁反应；\033[1m而且首次点击周围一定是空格\033[0;32m\n    游戏中\033[41;37;1m红旗\033[0;32m用于标记已明确的\033[47;30m地雷\033[0;32m，标志了\033[41;37;1m红旗\033[0;32m，就不能按了，要取消才可以。\033[43;31;1m问号\033[0;32m做暂时标记\n    如果方块上出现数字，则表示在其周围的八个方块中共有多少颗\033[47;30m地雷\033[0;32m，比如：1 周围8个格里只有一颗雷，2 周围8个格里就有两颗雷，以此类推\n    游戏区包括雷区、\033[47;30m地雷\033[0;32m计数器和计时器\n    数字就是这个方块周围的8个方块里有的雷的数量，\033[43;31;1m问号\033[0;32m表示不确定有没有雷，旗子就是确定那里有雷了\n\033[37;1m\n\t按任意键返回……\033[0m\n\t\033[33;1m\n\t查看源码+QQ:736224600\n\tcopyright © 2017 千百度");
		getch();
			goto star;
		break; 
		default:
			w = 9; h = 9; mine_num = 10; 
		break; 
	}
	mine_num_r = 0, judge = 0, x = w/2, y = h/2, game_time = 0; 
	srand(time(NULL)); 
	generate_map(mine_num, w*h, w); 
	while (!judge)//主循环
	{
		clrscr(); 
		draw_map(w, h); 
		key_event(w, h); 
		judge_win(w, h); 
		usleep(50000); //每次间隔0.1秒
	}
	memset(init_mine_map,0,sizeof(void_mine_map)); //游戏结束后置空原始地图
	memset(print_mine_map,0,sizeof(void_mine_map)); //置空打印地图
	first_key = 1; 
	goto star; 
	return 0; 
}

void generate_map(int m,  int n,  int w)
{
	int a[1000]; 
	int i, j, k; 
	for (i = 0; i < n; i++)
		a[i] = i; //生成原始序列
	for (i = 0; i < m; i++)
	{//将原始序列随机打乱
		j = rand()%(n-i)+i; 
		k = a[i]; 
		a[i] = a[j]; 
		a[j] = k; 
		if ((a[i]+1)%w == 0)
		/*将随机序列赋值给原始地图，
		需要考虑二维矩阵数组和一维线性数组的相互转换*/
			init_mine_map[(a[i]+1)/w-1][w-1] = -1; 
		else
			init_mine_map[(a[i]+1)/w][(a[i]+1)%w-1] = -1; 
	}
}

void open_blank_space(int x, int y, int w, int h)
{
	/*循环判断目标格子及周围9个方格是否为空格
	如果是空格递归展开
	直到地图边界或雷数格然后赋值给打印地图*/
	for (int i = y-1; i <= y+1; i++)
	for (int j = x-1; j <= x+1; j++)
	{
		if (i  >= 0 && i < h && j  >= 0 && j < w && (print_mine_map[i][j] == 0 || print_mine_map[i][j] == -2 || print_mine_map[i][j] == -3))
		{
			if (init_mine_map[i][j])
				print_mine_map[i][j] = init_mine_map[i][j]; 
			else
			{
				print_mine_map[i][j] = -4; 
				open_blank_space(j, i, w, h); //此处开始递归
			}
		}
	}
}

void draw_map(int w, int h)
{//各种打印，几乎游戏过程中所有信息
	printf("\033[43;34m 12346789控制光标移动, 5翻开, 0插旗或标记, 空格回主菜单\033[0m\n"); 
	for (int i = 0; i < w; i++)
		printf("--"); 
	printf("\n"); 
	mine_num_b = mine_num_r = 0; 
	for (int i = 0; i <= h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (i < h)
			switch (print_mine_map[i][j])
			{
				case -1:
					printf("\033[41;30m💣💣\033[0m"); 
				break; 
				case -5:
					printf("\033[46;31m❌❌\033[0m"); 
					break; 
				case -4:
				if (x == j && y == i)
					printf("\033[41m  \033[0m"); 
				else
					printf("\033[40;5m  \033[0m"); 
				break; 
				case -3:
					mine_num_r++; 
					if (x == j && y == i)
						printf("\033[47m🚩🚩\033[0m"); 
				else
					printf("\033[40m🚩🚩\033[0m"); 
				break; 
				case -2:
				if (x == j && y == i)
					printf("\033[47m❓❓\033[0m"); 
				else
					printf("\033[46m❓❓\033[0m"); 
				break; 
				case 0:
					mine_num_b++; 
				if (x == j && y == i)
					printf("\033[41m◽◽\033[0m"); 
				else
					printf("\033[40;5m⬜⬜\033[0m"); 
				break; 
				default:
				if (x == j && y == i)
					printf("\033[41;37m%s\033[0m", str[print_mine_map[i][j]-1]); 
				else
					printf("\033[4%d;3%dm%s\033[0m", (print_mine_map[i][j]+1)%7+1, print_mine_map[i][j]%7, str[print_mine_map[i][j]-1]); 
				break; 
			}
			else
				printf("--"); 
		}
		if (i == h)
		{
			if (judge == 1)
				mine_num_b += mine_num; 
			printf("\n\033[40;33m时间:%2d\033[0m  \033[40;32m还剩%2d颗地雷\033[40;36m 进度:%0.2f%%  \033[37m难度:\033[0m%d*%d*%d", ++game_time/20, mine_num-mine_num_r, (double)(w*h-mine_num_b)*100/(w*h), w, h, mine_num); 
		}
		printf("\n"); 
	}
}

void initmap_mine_num(int w, int h)
{//循环生成周围有地雷的格子的地雷数并赋值给对应的格子
	int k; 
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			k = 0; 		
			for (int y = i-1; y <= i+1; y++)
			for (int x = j-1; x <= j+1; x++)
			{//循环其周围的9个格子
				if (x  >= 0 && !(x == j && y == i) && x < w && y  >= 0 && y < h)
				{
					if (init_mine_map[y][x] == -1)
					k++; 
				}
			}
			if (init_mine_map[i][j] != -1)
				init_mine_map[i][j] = k; 
		}
	}
}

void judge_win(int w, int h)//判断游戏成功
{
	int k = 0; 
	for (int i = 0; i < h; i++)
	for (int j = 0; j < w; j++)
	if (print_mine_map[i][j] > 0 || print_mine_map[i][j] == -4)
	k++; 
	if (k+mine_num == w*h)
	{
		for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		if (init_mine_map[i][j] == -1)
			print_mine_map[i][j] = -3; 
		judge = 2; 
		clrscr(); 
		draw_map(w, h); 
		printf("\033[47;34m成功！任意键继续\033[0m\n"); 
		getch(); 
		clrscr(); 
	}
}

void gameover(int w, int h)//游戏结束
{
	for (int i = 0; i < h; i++)
	for (int j = 0; j < w; j++)
	{
		if (init_mine_map[i][j] == -1 && print_mine_map[i][j] != -3)
			print_mine_map[i][j] = init_mine_map[i][j]; 
		if (init_mine_map[i][j] != -1 && print_mine_map[i][j] == -3)
			print_mine_map[i][j] = -5; 
	}
	judge = 1; 
	clrscr(); 
	draw_map(w, h); 
	printf("\033[47;31m失败！任意键重新开始\033[0m\n"); 
	getch(); 
	clrscr(); 
}

void key_event(int w, int h)
{
	while (kbhit())//判断输入，有则循环
	{
		int key  =  getch(); //获取按键
		switch(key)
		{
			case '1'://左上移
			if (y > 0 && x > 0)
				x--, y--; 
			break; 
			case '2':
			if (y > 0)
				y--; 
			else
				y = h-1; 
			break; 
			case '3':
			if (y > 0 && x < w-1)
				x++, y--; 
			break; 
			case '4':
			if (x > 0)
				x--; 
			else
				x = w-1; 
			break; 
			case '6':
			if (x < w-1)
				x++; 
			else
				x = 0; 
			break; 
			case '7':
			if (y < h-1 && x > 0)
				x--, y++; 
			break; 
			case '8':
			if (y < h-1)
				y++; 
			else
				y = 0; 
			break; 
			case '9':
			if (y < h-1 && x < w-1)
				x++, y++; 
			break; 
			case 32://重新开始
				clrscr(); 
				judge = 2; 
			break; 
			case '0'://标记
				if (print_mine_map[y][x] == 0)
					print_mine_map[y][x] = -3; 
				else
				if (print_mine_map[y][x] == -3)
					print_mine_map[y][x] = -2; 
				else
				if (print_mine_map[y][x] == -2)
					print_mine_map[y][x] = 0; 
			break; 
			case '5'://展开
			if (print_mine_map[y][x] != -3)
			{
			   if (first_key)
			   {
			   	first_key = 0;
		int *blank[1000] = {}, *mine[9] = {}, a = 0, n = 0, m = 0; 
		for (int i = y-1; i <= y+1; i++)
		for (int j = x-1; j <= x+1; j++)
		{
			if(i  >= 0 && i < h && j  >= 0 && j < w)
			{
				if (init_mine_map[i][j] == 0)
					init_mine_map[i][j] = -1; 
				else
					a++; 
				mine[n++] = &init_mine_map[i][j]; 
			}
		}
		for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		{
			if (init_mine_map[i][j] == 0)
				blank[m++] = &init_mine_map[i][j]; 
		}
		for (int i = 0; i < a; i++)
		{
			int j = rand()%(m-i)+i; 
			int *k = blank[i]; 
			blank[i] = blank[j]; 
			blank[j] = k; 
			*blank[i] = -1; 
		}
		for (int i = 0; i < n; i++)
			*mine[i] = 0; 
		initmap_mine_num(w, h); 
		open_blank_space(x, y, w, h); 
	}
	else if (init_mine_map[y][x] != 0 && init_mine_map[y][x] != -1)
		print_mine_map[y][x] = init_mine_map[y][x]; 
	else if (init_mine_map[y][x] == -1)
		gameover(w, h);
		else
		open_blank_space(x, y, w, h); 
		  }
			break; 
		}
	}
}