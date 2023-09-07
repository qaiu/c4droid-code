/*
 * 命令行经典俄罗斯方块v1.5.5
 * by.QAIU
 * date:2017.5
 * 修改于2019.6.8 
 * 改进输出缓冲机制，解决闪屏问题，多线程优化
 */

#include <conio.h> 	// usleep() getch() stdio.h ……
#include <string.h>	// 字符串处理
#include <stdlib.h>	// rand()
#include <stdbool.h>   // bool类型
#include <signal.h>	// signal()定时器信号函数
#include <sys/time.h>  // 定时器struct itimerval, setitimer()
#include <time.h>
#include <pthread.h>   //多线程


#define H 21
#define W 16
#define W0 5
#define T_SIZE 4
#define T0 800000
#define LOCK_DELAY 500000

typedef struct
{
	int x[4][4];
	int y[4][4];
	int type;
} TETRIS;
TETRIS tet[8] = { };			// 方块结构体

TETRIS GetTetris();				// 随机获取一组方块
void initmap();					// 初始化地图数据(缓冲画面)
void drawmap();					// 绘制地图 (打印画面)
void refresh_map(int );				// 刷新一帧画面
void* event(void *);			// 按键事件监听，独立线程
bool can_drop(TETRIS );		// 判断方块能否下落(是否触底)
void drop(int delay);			// 下落一格，触底延时锁定
void create_timer(int );		// 创建下落定时器
void set_timer(int );			// 设置定时器间隔时间
void timer_move(int );			// 定时执行的下降函数
TETRIS rotate(TETRIS, bool );		// 旋转方块
void judge();					// 判断消除行得分
void loading();					// 载入游戏
void gameover();				// 游戏结束
void game_exit();

const char str0[3][7][16] = 
{ "经", "典", "俄", "罗", "斯", "方", "块", "##", "##", "##", "##", "##", "##", "##" };
char str[7][16] = { };

static int tetris_coord_x[7][2][4] = 
{	  						  // 方块横坐标
	{0, 2, 0, 0, 1, 2, 3, 0},	// T型方块
	{0, 0, 0, 0, 1, 2, 3, 4},	// I
	{0, 2, 3, 0, 0, 2, 3, 0},	// O
	{1, 2, 0, 0, 0, 2, 3, 0},	// Z
	{0, 2, 3, 0, 1, 2, 0, 0},	// S
	{1, 0, 0, 0, 1, 2, 3, 0},	// J
	{0, 0, 3, 0, 1, 2, 3, 0}	 // L

}, tetris_coord_y[7][4][4] = 
{				  		  	// 方块纵坐标
	{0, 1, 0, 0, 2, 2, 2, 0},	// T型方块
	{0, 0, 0, 0, 2, 2, 2, 2},	// I
	{0, 1, 1, 0, 0, 2, 2, 0},	// O
	{1, 1, 0, 0, 0, 2, 2, 0},	// Z
	{0, 1, 1, 0, 2, 2, 0, 0},	// S
	{1, 0, 0, 0, 2, 2, 2, 0},	// J
	{0, 0, 1, 0, 2, 2, 2, 0} 	// L
};

enum	// DIRECTION
{
	UP = '2',
	DOWN = '5',
	LEFT = '4',
	RIGHT = '6',
	PAUSE = '0',
	DIRECT = ' ',
};

/* int pau 暂停flag 用于控制子线程 pau=-1
   暂停按键监听(将监听交给主线程) 暂停界面刷新 pau=0
   按键暂停 保持监听 pau=1 按键继续 */
int pau = 1;
int pau0 = 1;



/*
 * bool k 下降flag 用于确定下降和锁定 
 * k=0 下面没有方块阻挡可以下落 
 * k=1 下面有方块阻挡应当锁定 
 */
bool k = 0;

int t_sum[8];			// 方块数统计
int line_sum[5] = { };	// 消行数统计

int score_top = 0;			// 最高分
int score = 0;			// 分数
int map[H + 1][W + 1] = { };	// 地图数组
int t = T0;			// 步时 (下落速度)
int lv = 0, lv0 = 0;	// 等级
int history[4] = { };	// 历史方块记录

const char *filename = "sample.dat";	// 分值记录文件
//int sl = 50000;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


char *printStr;
// 主函数
int main(void)
{
	loading();
	while (1)
	{
		
		if (pau > 0)
		{
			drop(0);
			refresh_map(t);
		}
	}
}

/* 设置级别 */
void setlv()
{
	setbuf(stdin, NULL);
	printf("\e[5;20H\e[37m'5'键开始游戏\n");
	printf("\e[6;20H\e[37m'1'键设置级别\n");
	printf("\e[7;20H\e[37m'2'键设置样式\n");
	printf("\e[8;20H\e[37m'0'退出游戏\n");

	int k = getch();
	while (true)
	{
		if (k == '1')
		{
			clrscr();
			do
			{
				printf("请输入初始级别(0-16)\n ");
				scanf("%d", &lv0);
			}
			while (lv0 > 16 || lv0 < 0);
			break;
		}
		else if (k == '2')
		{
			int k;
			do
			{
				clrscr();
				printf
					("请选择方块填充字符:\n1.使用汉字 \e[37;40;5m经\e[0;41;37m典\e[42;37m俄\e[43;37m罗\e[43;34m斯\e[45;37m方\e[46;37m块\e[40;37m经\e[0;31m典\e[32m俄\e[33m罗\e[34m斯\e[35m方\e[36m块\n\e[0m2.只使用符号'#' 同时低帧降低闪屏");

				k = getch();
			}
			while (k > '2' || k < '1');
			memcpy(str, str0[k - '1'], sizeof(str));
			initmap();
			clrscr();
			drawmap();
			setlv();
			break;
		}
		else if (k == '5')
			break;
		else if (k == '0')
			game_exit();
		k = getch();
	}
	lv = lv0;
	t = lv > 15 ? T0 / (lv * 1.2) : T0 - lv * 50000;
	setbuf(stdin, NULL);
	while (kbhit())
		getch();
}

/* 暂停帮助 */
void help()
{
	printf("\e[9;20H\e[43;30m+------------+\n");
	printf("\e[18;20H\e[43;30m+------------+\n");
	printf("\e[10;20H\e[43;30m|\e[47;31m'4'或'a'左移\e[0;43;30m|\e[0m\n");
	printf("\e[11;20H\e[43;30m|\e[47;31m'6'或'd'右移\e[0;43;30m|\e[0m\n");
	printf("\e[12;20H\e[43;30m|\e[47;31m'2'或'w'旋转\e[0;43;30m|\e[0m\n");
	printf("\e[13;20H\e[43;30m|\e[47;31m'5'或's'下移\e[0;43;30m|\e[0m\n");
	printf("\e[14;20H\e[43;30m|\e[47;31m空格回车硬降\e[0;43;30m|\e[0m\n");
	printf("\e[15;20H\e[43;30m|\e[47;31m'0'或'p'暂停\e[0;43;30m|\e[0m\n");
	printf
		("\e[16;20H\e[43;30m|\e[47;30m使用\e[47;31m← \e[47;35m→ \e[31m↓ \e[35m↑ \e[0;43;30m|\e[0m\n");
	printf("\e[17;20H\e[0;43;30m|\e[0m\e[47;30m可以连续移动\e[0;43;30m|\e[0m\n");
}

// 加载界面,初始化
void loading()
{
	FILE *fp;
	char readline[256] = { '\0' };
	if ((fp = fopen(filename, "r")) != NULL)
	{
		fgets(readline, 64, fp);
		score_top = atoi(readline);
		fclose(fp);
	}
	else
	{
		fopen(filename, "w");
	}
	printf("\e[?25l");			// 隐藏光标
	printStr = (char *)malloc(10000);
	srand(time(NULL));			// 初始化伪随机数生成器 
	for (int i = 0; i < 8; i++)
		tet[i] = GetTetris();
	memcpy(str, str0[0], sizeof(str));
	refresh_map(0);
	help();
	setlv();
	refresh_map(0);
	pthread_t pid1, pid2;
	pthread_create(&pid1,NULL, event, NULL);
//	create_timer(t);
}

static struct timeval tv = {};
void refresh_map(int t)
{
	tv.tv_sec = 0; 
	tv.tv_usec = t;
	
	initmap();
	
	pthread_mutex_lock(&mut);
	drawmap();
	pthread_mutex_unlock(&mut);
	select(0, NULL, NULL, NULL, &tv);
}

/* 创建/关闭定时器 */
void timer_move(int t)
{
	if (pau > 0)
	{
		drop(0);
		refresh_map(1000);
	}
}

void create_timer(int t)
{

	signal(SIGALRM, timer_move);
	struct itimerval tick = { };
	tick.it_value.tv_usec = t;
	tick.it_interval.tv_usec = t;
	setitimer(ITIMER_REAL, &tick, NULL);
}

void set_timer(int t)
{
	struct itimerval tick = { };
	tick.it_value.tv_usec = t;
	tick.it_interval.tv_usec = t;
	setitimer(ITIMER_REAL, &tick, NULL);
}

// 初始化地图
void initmap(void)
{
	for (int i = 0;i <= H; ++i)	// 定义地图
	{
		for (int j = 0; j <= W; ++j)
		{
			if ((i == H || i == 0) && j >= W0)
				map[i][j] = 3;
			else if ((i == H || i == 0) && j < 4)
				map[i][j] = -6;
			else if (j == W0 || j == W)
				map[i][j] = 2;
			else if (j == W0 - 1)
				map[i][j] = -7;
			else if (map[i][j] < W0 - 1)
				map[i][j] = 0;
		}
	}
	map[H][4] = map[0][W0 - 1] = -8;
	map[0][W] = map[H][W] = map[0][W0] = map[H][W0] = -2;
	TETRIS tet1=tet[0],tet0=tet[0];
	
	for (int i = 0; i < T_SIZE; i++)
	{
		for (int j = 0; j < T_SIZE; j++)
		{
			if (tet[0].y[i][j] != 0 && tet[0].x[i][j] != 0)
			{
				map[tet[0].y[i][j]][tet[0].x[i][j]] = 1;
				map[H][tet[0].x[i][j]] = 11;
			}
			for (int k = 1; k < 5; k++)
			{
				if (tet[k].y[i][j] != 0 && tet[k].x[i][j] != 0)
				{
					map[tet[k].y[i][j] + (k - 1) * 3][tet[k].x[i][j] - 9] = -9 - k;
				}
			}
		}
	}
	while(1)
	{
		for (int i = 0; i < T_SIZE; i++)
		{
			for (int j = 0; j < T_SIZE; j++)
			{
				if (map[tet0.y[i][j]][tet0.x[i][j]] > 3)
				{
					goto _flag_;
				}
			}
		}
		
		tet1=tet0;
		for (int i = 0; i < T_SIZE; i++)
		{
			for (int j = 0; j < T_SIZE; j++)
			{
				if (tet0.y[i][j] != 0 && tet0.x[i][j] != 0)
				tet0.y[i][j]++;				
			}
		}
	}
	
	_flag_:
	for (int i = 0; i < T_SIZE; i++)
	{
		for (int j = 0; j < T_SIZE; j++)
		{
			if (tet1.y[i][j] != 0 && tet1.x[i][j] != 0 &&  map[tet1.y[i][j]][tet1.x[i][j]]!=1)
				map[tet1.y[i][j]][tet1.x[i][j]] = -1;			
		}
	}

}

// 获取随机方块
TETRIS GetTetris()
{
	TETRIS a={};
	int k = (double)rand() / RAND_MAX * 7;
	for (int i = 0; i < 4; i++)
	{
		if (k != history[0] && k != history[1] && k != history[2] && k != history[3])
			break;
		k = (double)rand() / RAND_MAX * 7;
	}
	history[0] = history[1];
	history[1] = history[2];
	history[2] = history[3];
	history[3] = k;
//	k=1; //单个调试
	a.type = k;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < T_SIZE; j++)
		{
			a.x[i][j] = tetris_coord_x[k][i][j];
			a.y[i][j] = tetris_coord_y[k][i][j];
			if (a.y[i][j] != 0 && a.x[i][j] != 0) a.x[i][j] += 8;
		}
	}
	return a;
}

/* 判定消除和得分 */
void judge(int high)
{
	pau = -1;
	bool line[4] = { };
	int sum = 0;
	/* 判断消行 */
	for (int i = high, k = 0; i > high - 4; i--)
	{
		line[k++] = true;
		for (int j = W0 + 1; j < W; j++)
		{
			if (map[i][j] < 4)
			{
				line[k - 1] = false;
				break;
			}
		}
	}
	/* 实现动态消行 */
	if (line[0] || line[1] || line[2] || line[3])
	{
		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (line[i])
				{
					map[high - i][11 + j] = 0;
					map[high - i][10 - j] = 0;
				}
			}
			drawmap();
			usleep(50000);
		}
	}
	
	for (int i = high - 3, k = 3; i <= high; ++i)
	{
		if (line[k--])
		{
			for (int j = i; j > 1; --j)
			{
				for (int i = W0 + 1; i < W; ++i)
				{
					map[j][i] = map[j - 1][i];
				}
			}
			sum++;
		}
	}

	/* 得分 */
	switch (sum)
	{
	case 1:
		score += (40 * (lv + 1));
		line_sum[0]++;
		break;
	case 2:
		score += (100 * (lv + 1));
		line_sum[1]++;
		break;
	case 3:
		score += (300 * (lv + 1));
		line_sum[2]++;
		break;
	case 4:
		score += (1200 * (lv + 1));
		line_sum[3]++;
		break;
	}
	line_sum[4] = 0;
	for (int i = 0; i < 4; i++)
		line_sum[4] += line_sum[i] * (i + 1);
	lv = line_sum[4] / 10 + lv0;
	t = lv > 15 ? T0 / (lv + 1) : T0 - lv * 50000;
	if (score > score_top) //新纪录
	{
		score_top = score;
		FILE *fp;
		char str[256];
		sprintf(str, "%d", score_top);
		if ((fp = fopen(filename, "w")) != NULL)	// 打开文件
			fputs(str, fp);
		fclose(fp);
	}
	pau = 1;
}

/*
 * 判断方块下方是否有障碍物(即能否下降)
 * 参数: 要判断的方块对象(结构体)
 * 返回值: true:有障碍; false:无障碍
 *
 */
bool can_drop(TETRIS tet)
{
	for (int i = 0; i < T_SIZE; i++)
	{
		for (int j = 0; j < T_SIZE; j++)
		{
			if (map[tet.y[i][j] + 1][tet.x[i][j]] >= 3 )
			{
				return true;
			}
		}
	}
	return false;
}

// 方块平移
void horizontal_move(int direction)
{
	int flag = false;
	for (int i = 0; i < T_SIZE; i++)
	{
		for (int j = 0; j < T_SIZE; j++)
		{
			if (tet[0].x[i][j]
				&& (map[tet[0].y[i][j]][tet[0].x[i][j] + direction] == 2
					|| map[tet[0].y[i][j]][tet[0].x[i][j] + direction] > 3
					|| map[tet[0].y[i][j]][tet[0].x[i][j] + direction] < -1))
			{
				flag = false;
				return;
			}
			else
			{
				flag = true;
			}
		}
	}
	if (flag)
	{
		for (int i = 0; i < T_SIZE; i++)
		{
			for (int j = 0; j < T_SIZE; j++)
			{
				if (tet[0].y[i][j] != 0 && tet[0].x[i][j] != 0)
					tet[0].x[i][j] += direction;
			}
		}
	}
	 k = can_drop(tet[0]);
//		return;
}


// 按键事件
void* event(void * ptr)
{
	while (true)
	{
		
		pthread_mutex_lock(&mut);
		while (!pau0)
		{
			pthread_cond_wait(&cond, &mut);
		}
		pthread_mutex_unlock(&mut);
		
		while (kbhit()) 	// 判断输入，有则循环
		{
			int key = getch();// 获取按键
			switch (key)
			{
			case 'C':
			case RIGHT:
			case 'd':
				if (pau > 0)
					horizontal_move(1);
				break;
			case 'D':
			case 'a':
			case LEFT:
				if (pau > 0)
					horizontal_move(-1);
				break;
			case 'A':
			case UP:
			case 'w':
				if (pau > 0)
				{
					tet[0] = rotate(tet[0],0);
					k = can_drop(tet[0]);
				}
				break;
			case '1':
				if (pau > 0)
				{
					tet[0] = rotate(tet[0],1);
					k = can_drop(tet[0]);
				}
				break;
			case 'B':
			case 's':
			case DOWN:
				if (pau > 0)
				{
					pau=0;
					drop(0);
					pau=1;
				}
				break;
			case DIRECT:
			case 10:
				if (pau > 0)
				{
					
					pau = 0;
					usleep(5000);
					pthread_mutex_lock(&mut);
					while (!k)
						drop(0);
							
					pthread_mutex_unlock(&mut);
					refresh_map(0);
					setbuf(stdin, NULL);
					while (kbhit())
						getch();
					drop(0);
					pau = 1;
				}
				break;
			case PAUSE:
			case 'p':
				pau = -pau;
				if (pau < 0)
				{
					puts("\e[5;23H休息一下!");
					puts("\e[6;22H'0'继续游戏");
					puts("\e[7;22H'1'回主菜单");
					help();
				}
				break;
/*
			case '1':
				if (pau < 0)
				{

					for (int i = 0; i < 5; i++)
						tet[i] = GetTetris();
					for (int i = 0; i < 8; i++)
						t_sum[i] = 0;
					for (int i = 1; i < H; i++)
					{
						for (int j = 1; j < W; j++)
						{
							map[i][j] = 0;
						}
					}
					refresh_map(1000);
					setlv();
			//		set_timer(t);
					pau = true;
				}
				break;*/
			}
			
			if (pau > 0) refresh_map(0);
			while (kbhit()) getch();
		}
	
	}
	return NULL;
}

// 方块下落
void drop(int temp) //形参是定时器需要，实际无用
{
	if (!k)
	{
	//	k = can_drop(tet[0]);
		for (int i = 0; i < T_SIZE; i++)
		{
			for (int j = 0; j < T_SIZE; j++)
			{
				if (map[tet[0].y[i][j]][tet[0].x[i][j]] < 2)
					tet[0].y[i][j]++;
			}
		}
		k = can_drop(tet[0]);
		return;
	}
	/* 锁定延时 */
	if (pau&&LOCK_DELAY>t)
	{
		struct timeval tv1={tv1.tv_sec = 0,tv1.tv_usec = LOCK_DELAY}; 
		select(0, NULL, NULL, NULL, &tv1);
	}
	if (!k) return; 	// 通过按键监听线程获取动态k值

	int high;
	for (int i = 0; i < T_SIZE; i++)
	{
		for (int j = 0; j < T_SIZE; j++)
		{
			if (tet[0].y[i][j] != 0 && tet[0].x[i][j] != 0)
			{
				map[tet[0].y[i][j]][tet[0].x[i][j]] = tet[0].type + 4;
				high = tet[0].y[i][j];
			}
		}
	}
	if (map[2][8] > 3 || map[2][9] > 3)
	{
		gameover();
	}
	else
	{
		score += high / 2;
		judge(high);
		t_sum[tet[0].type]++;
		t_sum[7]++;
		for (int i = 0; i < 7; i++)
			tet[i] = tet[i + 1];
		tet[7] = GetTetris();
		k = can_drop(tet[1]);
	}

}

// 绘制游戏图像并刷新
void drawmap()
{
	static char temp[2000];
	static char printStr[20000];
	strcpy(printStr, "\e[1;1f   \e[31m+");
	for (int i = 0; i <= H; i++)	// 画地图
	{
		for (int j = 0; j <= W; j++)
		{
			switch (map[i][j])
			{
			case 0:
				if(j>4)
				strcat(printStr, "\e[38;5;240m. \e[0m");
				else 
				strcat(printStr, "\e[40;37m  \e[0m");
				break;
			case -2:
				strcat(printStr, "\e[1;33m＊\e[0m");
				break;
			case -6:
				strcat(printStr, "\e[31m--\e[0m");
				break;
			case -7:
				strcat(printStr, "\e[31m│ \e[0m");
				break;
			case -8:
				strcat(printStr, "\e[31m+ \e[0m");
				break;
			case 2:
				strcat(printStr, "\e[1;33m│ \e[0m");
				break;
			case 3:
				strcat(printStr, "\e[33;1m--\e[0m");
				break;
			case 11:
				sprintf(temp, "\e[3%d;1m==\e[0m", tet[0].type);
				strcat(printStr, temp);
				break;
			case 1:
				sprintf(temp, "\e[47;3%dm%s\e[0m", tet[0].type, str[tet[0].type]);
				strcat(printStr, temp);
				break;
			case -1:
				sprintf(temp, "\e[38;5;244m[]\e[0m");
				strcat(printStr, temp);
				break;
			default:
				if (map[i][j]>=-16&&map[i][j]<=-10)
				{
					sprintf(temp, "\e[47;3%dm%s\e[0m", tet[-map[i][j]-9].type, str[tet[-map[i][j]-9].type]);
					strcat(printStr, temp);		
				}
				else
				{
					sprintf(temp, "\e[4%d;37;%dm%s\e[0m", map[i][j] - 4, map[i][j] == 4 ? 5 : 2,str[map[i][j] - 4]);
					strcat(printStr, temp);
				}
				break;
			}
		}
		sprintf(temp, "%s",i <= 20 ? (i == 20 ? "\n   \e[31m+\e[0m" : "\n   \e[31m|\e[0m") : "\n    ");
		strcat(printStr, temp);
	}
	if (score >= score_top)
	{
		sprintf(temp, "\e[9;41H\e[36;1m新记录!\n");
		strcat(printStr, temp);
	}
	sprintf(temp,
			"\e[11;41H\e[34m| \e[37mT %03d\e[34m        |\n\e[12;41H\e[34m| \e[31mI %03d\e[34m        |\n\e[13;41H\e[34m| \e[32mO %03d\e[34m        |\n\e[14;41H\e[34m| \e[33mZ %03d \e[38;5;245m消1 %03d\e[34m|\n\e[15;41H\e[34m| \e[34;1mS %03d \e[38;5;247m消2 %03d\e[0;34m|\n\e[16;41H\e[34m| \e[35mJ %03d \e[38;5;248m消3 %03d\e[34m|\n\e[17;41H\e[34m| \e[36mL %03d \e[38;5;251m消4 %03d\e[34m|\n\e[19;41H\e[34m|  \e[33;1m%04d    \e[38;5;253m%04d\e[0;34m|\n\e[5;41H\e[32m| \e[33;1m 得分  %06d\e[0m\e[32m|\n\e[4;41H\e[32m| \e[32;1m 最佳  %06d\e[0m\e[32m|\n\e[6;41H\e[32m| \e[36;1m 级别  %02d    \e[0m\e[32m|\n\e[10;41H\e[34m+--------------+\n\e[18;41H\e[34m+\e[32m------\e[34;1msum\e[0;32m-----\e[34m+\n\e[20;41H\e[34m+--------------+\n\e[1;41H\e[32m+--------------+\n\e[3;41H\e[32m+--------------+\n\e[7;41H\e[32m+--------------+\n\e[2;41H\e[32m| \e[38;5;248mTETRIS v1.5.5\e[32m|\n\e[22;41H \e[36m ©千百度 2017\e[0m\n",
			t_sum[0], t_sum[1], t_sum[2], t_sum[3], line_sum[0], t_sum[4], line_sum[1], t_sum[5],
			line_sum[2], t_sum[6], line_sum[3], t_sum[7], line_sum[4], score, score_top, lv);
	strcat(printStr, temp);
	puts(printStr);
}


// 游戏结束
void gameover()
{
	pau = false;
	usleep(5000);
	refresh_map(1000);
	printf("\e[2;18H\e[47;30m游戏结束,回车继续\e[40;37m \n");
	score = lv0 = k = line_sum[0] = line_sum[1] = line_sum[2] = line_sum[3] = 0;
	getchar();
	usleep(1000);
	srand(time(NULL));
	for (int i = 0; i < 5; i++)
		tet[i] = GetTetris();
	for (int i = 0; i < 8; i++)
		t_sum[i] = 0;
	for (int i = 1; i < H; i++)
	{
		for (int j = 1; j < W; j++)
		{
			map[i][j] = 0;
		}
	}
	/* 清除键盘缓冲区 */
	setbuf(stdin, NULL);
	while (kbhit())
		getch();
	refresh_map(1000);
	
	pthread_mutex_lock(&mut);
	pau0=0;
	pthread_mutex_unlock(&mut);
	setlv();
	
	pthread_mutex_lock(&mut);
	pau0=1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mut);
	pau = true;
}

void game_exit()
{
	clrscr();
	puts("感谢使用\nQAIU作品\nQQ736226400");
	free(printStr);
	printStr=NULL;
	usleep(1000*3000);
	exit(0);	
}
	/* 方块的旋转 */
bool do_rotate(int arr_x[4][4], int arr_y[4][4], int size, bool k)
{
	int flag=0, temp_x[4][4] = { }, temp_y[4][4] = { };
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (k ? arr_x[j][size - 1 - i] : arr_x[size - 1 - j][i])
			{
				temp_y[i][j] = k ? arr_y[j][size - 1 - i] - j + i : arr_y[size - 1 - j][i] - size + j + i + 1;
				temp_x[i][j] = k ? arr_x[j][size - 1 - i] + 1 + i + j - size : arr_x[size - 1 - j][i] - i + j;
				if (map[temp_y[i][j]][temp_x[i][j]] > 1) flag=1;
			}
		}
	}
	if (!flag)
	{
		memcpy(arr_x, temp_x, sizeof(temp_x));
		memcpy(arr_y, temp_y, sizeof(temp_y));
		return true;
	}
	return false;
}

TETRIS rotate(TETRIS t,bool clockwise)
{
	if (t.type==2)
	return t;
	do_rotate(t.x,t.y,t.type==1?4:3,clockwise);
	return t;
}