/* c4下，g++编译通过。如果没有通过，请在c4设置中，g++参数 
   -lm前添加-std=c++11 (注，前后有空格) */
/* 使用sdl自带方向键控制，有点不好控制，自己慢慢来 */
/* 这里仅提供模板，更多功能，自己添加，去下载我制作的apk吧！ */
/* c4droid吧，TTHHR制作，改写时请保留原注释！ */

#include<SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include<time.h>
// 头文件

#define SCREEN_WIDTH 480		// 所使用的图片的宽,自己修改
#define SCREEN_HEIGHT 800		// 使用图片的高，自己修改

SDL_Rect clip[9];				// 用来存各个小图
SDL_Surface *s = NULL;			// 加载的图片
int map[3][3];					// 和小图对应的二维数组，用来方便移动和判断
int a, b;						// 空图的坐标
int i, j;						// 用于循环的变量
bool quit = false;				// 控制程序退出的变量
SDL_Surface *screen = NULL;		// 屏幕指针

void picrand();					// 用来初始化小图数组，随机
bool judge();					// 判断是否赢了
void init()						// 启动和初始化
{
	SDL_Init(SDL_INIT_EVERYTHING);
	// 启动sdl
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);	// 设置屏幕
	clip[1] =
	{
	0, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3};
	clip[2] =
	{
	SCREEN_WIDTH / 3, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3};
	clip[3] =
	{
	2 *SCREEN_WIDTH / 3, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 3};
	clip[4] =
	{
	0, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3};
	clip[5] =
	{
	SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3};
	clip[6] =
	{
	2 *SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3};
	clip[7] =
	{
	0, 2 * SCREEN_HEIGHT / 3, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3};
	clip[8] =
	{
	SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3};
	// 各小图的具体位置
	clip[0] =
	{
	NULL, NULL, NULL, NULL};	// 留空的小图
	picrand();					// 随机
}

SDL_Surface *loadimg(char *str)	// 加载优化图片
{
	SDL_Surface *loadimg = NULL;
	SDL_Surface *opti = NULL;
	loadimg = IMG_Load(str);
	if (loadimg != NULL)
	{
		opti = SDL_DisplayFormat(loadimg);
		SDL_FreeSurface(loadimg);
	}
	return opti;
}

void apply_surface(int x, int y, SDL_Surface * source, SDL_Surface * destination, SDL_Rect * clip = NULL)	// 将小图粘贴到屏幕
{
	// Holds offsets
	SDL_Rect offset;
	// Get offsets
	offset.x = x;
	offset.y = y;
	// Blit
	SDL_BlitSurface(source, clip, destination, &offset);
}

void draw()
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));	// 将窗口填充黑色
	for (i = 0; i <= 8; i++)
	{
		switch (i)
		{
		case 0:
			apply_surface(2 * SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3, s,
						  screen, &clip[map[2][2]]);
			break;
		case 1:
			apply_surface(0, 0, s, screen, &clip[map[0][0]]);
			break;
		case 2:
			apply_surface(SCREEN_WIDTH / 3, 0, s, screen, &clip[map[0][1]]);
			break;
		case 3:
			apply_surface(2 * SCREEN_WIDTH / 3, 0, s, screen, &clip[map[0][2]]);
			break;
		case 4:
			apply_surface(0, SCREEN_HEIGHT / 3, s, screen, &clip[map[1][0]]);
			break;
		case 5:
			apply_surface(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, s, screen, &clip[map[1][1]]);
			break;
		case 6:
			apply_surface(2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, s, screen, &clip[map[1][2]]);
			break;
		case 7:
			apply_surface(0, 2 * SCREEN_HEIGHT / 3, s, screen, &clip[map[2][0]]);
			break;
		case 8:
			apply_surface(SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3, s, screen, &clip[map[2][1]]);
			break;
		default:;
		}						// 安小图数组顺序来粘贴小图
	}
	SDL_Flip(screen);			// 更新屏幕
}

int main(int argc, char *args[])
{
	SDL_Event event;			// 定义一个事件变量
	init();						// 启动，初始化
	s = loadimg("background.jpg");
	// 加载自己使用的图片
	draw();						// 画出初始的图
	while (quit == false)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)	// 判断按键是什么
				{
				case SDLK_UP:
					if (b + 1 != 3)	// 把这个判断去掉会有大bug
					{
						map[b][a] = map[b + 1][a];	// 空图变目标图片
						map[b + 1][a] = 0;	// 目标图片变空
						b++;	// 完成小图位置互换
					}
					break;
				case SDLK_DOWN:
					if (b - 1 != -1)
					{
						map[b][a] = map[b - 1][a];
						map[b - 1][a] = 0;
						b--;
					}
					break;
				case SDLK_LEFT:
					if (a + 1 != 3)
					{
						map[b][a] = map[b][a + 1];
						map[b][a + 1] = 0;
						a++;
					}
					break;
				case SDLK_RIGHT:
					if (a - 1 != -1)
					{
						map[b][a] = map[b][a - 1];
						map[b][a - 1] = 0;
						a--;
					}
					break;
				default:;
				}
				draw();			// 移动完成后，重新画图
			}
			else if (event.type == SDL_QUIT)
			{
				// Quit the program
				quit = true;
			}
		}
		quit = judge();			// 对移动后的数组进行判断
	}
	SDL_FreeSurface(s);
	SDL_Quit();					// 退出sdl
	return 0;
}

void picrand()					// 随机
{
	srand((int)time(NULL));
	b = rand() % 3;
	a = b;
	map[b][2] = 0;
	for (i = 0; i != 3; i++)
	{
		for (j = 0; j != 3; j++)
		{
			if ((b == i) && (j == 2))
				;
			else
			{
				if (a >= 8)
					a = 0;
				a++;
				map[i][j] = a;
			}
		}
	}
	a = 2;						// 因为要确定0代表的空图在哪，所以把a重新赋值2
}

bool judge()
{								// 这是判断，我觉得一点点一点点的判断，程序运行比较快
	if (map[0][0] == 1 && map[2][2] == 0)
	{
		if (map[0][1] == 2 && map[0][2] == 3)
		{
			if (map[1][0] == 4 && map[1][1] == 5)
			{
				if (map[1][2] == 6 && map[2][0] == 7)
				{
					if (map[2][1] == 8)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}