/*=========================
               游戏: 扫雷
               
作者: LOTOOHE   版本: 1.0.0

联系邮箱: lotoohe.zz@gmail.com或549654313@qq.com

 ——自己编的第一部游戏

请不要恶意使用此代码或者进行商业行为!

*****************************
*                                                      *
*               游戏愉快!                       *
*                                                      *
*****************************

           日期:  2013年12月7日
==========================*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#define P_W 884
#define P_H 884
#define N 15
	/* 窗口宽度与雷区大小的计算公式为P_W==P_H==(N-2)*68 */
int M = 20;						/* 雷数设定 */
SDL_Surface *open0, *open1, *open2, *open3, *open4, *open5, *open6, *open7,
	*open8, *open10, *mine, *blank, *chuang_copy =
	NULL, *font_text, *font_win, *menu_1, *menu_2, *menu_j, *menu_shu[10],
	*scanf_mine, *sur;
TTF_Font *font = NULL, *font_menu = NULL, *font_mine = NULL, *font_sy = NULL;
SDL_Color cr = { 250, 0, 250 };	/* 开始菜单及 输赢字体颜色 */

Uint32 ger = 0x00ff00;			/* 开始菜单背景颜色 */
SDL_Event event, menu_event;	/* 事件结构体定义 */
SDL_Surface *image(char wen[30])	/* blit图像处理函数 */
{
	SDL_Surface *aa = NULL;
	SDL_Surface *bb = NULL;
	bb = IMG_Load(wen);
	if (bb != NULL)
	{
		aa = SDL_DisplayFormat(bb);
		SDL_FreeSurface(bb);
	}
	return aa;
}

SDL_Surface *image_zhu(char wen[20], Uint32 color)	/* blit图像处理函数 */
{
	SDL_Surface *aa = NULL;
	aa = image(wen);
	if (aa != NULL)
	{
		SDL_SetColorKey(aa, SDL_SRCCOLORKEY, color);	/* 应用你的色键 */
	}
	return aa;
}

void rand(int a[N][N])			// 雷区生成
{
	int x, y, m = M, i = 0, j = 0, v, bit_x[100], bit_y[100];
	for (x = 0; x < N; x++)
		for (y = 0; y < N; y++)
			a[x][y] = 0;
	for (m = M; m > 0;)
	{
		int t = 0;
		x = rand() % (N - 1);
		y = rand() % (N - 1);

		do
		{
			if ((x != bit_x[t]) || (y != bit_y[t]))
			{
				v = 1;
			}
			else
			{
				v = 0;
				break;
			}
			t++;
		}
		while (t < i);
		if (v == 1)
		{
			if (x >= 1 && x <= (N - 2) && y >= 1 && y <= (N - 2))
			{
				bit_x[j] = x;
				bit_y[j] = y;
				a[x][y] = -1;
				m--;
				j++;
				i++;
			}
		}
		else;
	}

	for (y = 1; y <= (N - 2); y++)
	{
		for (x = 1; x <= (N - 2); x++)
		{
			if (a[y][x] == -1)
			{
				if (a[y - 1][x] != -1)
					++a[y - 1][x];
				if (a[y + 1][x] != -1)
					++a[y + 1][x];
				for (i = y - 1; i < (y - 1 + 3); i++)
				{
					if (a[i][x + 1] != -1)
						++a[i][x + 1];
					if (a[i][x - 1] != -1)
						++a[i][x - 1];
				}
			}
		}
	}
}

void weizhi(int x, int y, SDL_Surface * tub, SDL_Surface * chuang)	/* 图像位置移动函数 
																	 */
{

	SDL_Rect a;					/* 定义一个矩形a */
	a.x = x;
	a.y = y;					/* 分别溅入其坐标 */

	SDL_BlitSurface(tub, NULL, chuang, &a);	/* 粘贴到矩形a所在的位置 
											 */
}

bool init()						/* 初始化SDL函数 */
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;
	if (TTF_Init() == -1)
		return false;
	if ((chuang_copy = SDL_SetVideoMode(P_W, P_H, 32, SDL_SWSURFACE)) == NULL)
		return false;
	return true;
}

bool in_image()					// 加载图片，字体资源
{
	char sum[10][2] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	int b;
	font = TTF_OpenFont("../../../Resources/Font/05.ttf", 50);
	font_menu = TTF_OpenFont("../../../Resources/Font/05.ttf", 80);
	font_mine = TTF_OpenFont("../../../Resources/Font/05.ttf", 35);
	font_sy = TTF_OpenFont("../../../Resources/Font/05.ttf", 28);
	font_text =
		TTF_RenderUTF8_Solid(font, "很不幸你被炸死了！！", cr);
	font_win = TTF_RenderUTF8_Solid(font, "你真是排雷神人！！", cr);
	menu_1 = TTF_RenderUTF8_Blended(font_menu, "开始", cr);
	menu_2 = TTF_RenderUTF8_Blended(font_menu, "退出", cr);
	scanf_mine =
		TTF_RenderUTF8_Solid(font_mine, "输入雷的个数0到99之间:", cr);
	sur =
		TTF_RenderUTF8_Blended(font_sy,
							   "雷的初始个数为20个,本游戏由lotoohe编写,邮箱lotoohe.zz@gmail.com",
							   cr);
	for (b = 0; b < 10; b++)
		menu_shu[b] = TTF_RenderUTF8_Solid(font, &sum[b][0], cr);	// 利用for循环将0-9载入到表面
	open0 = image("resources/open0.png");
	open1 = image("resources/open1.png");
	open2 = image("resources/open2.png");
	open3 = image("resources/open3.png");
	open4 = image("resources/open4.png");
	open5 = image("resources/open5.png");
	open6 = image("resources/open6.png");
	open7 = image("resources/open7.png");
	open8 = image("resources/open8.png");
	open10 = image_zhu("resources/open10.png", 0xbce7bb);
	mine = image("resources/mine.png");
	blank = image("resources/blank.png");
	menu_j = image_zhu("resources/j.png", 0x000000);

	return true;
}

void quit()						// 退出
{
	int b;
	for (b = 0; b < 10; b++)
		SDL_FreeSurface(menu_shu[b]);
	SDL_FreeSurface(font_text);
	SDL_FreeSurface(font_win);
	SDL_FreeSurface(open0);
	SDL_FreeSurface(open1);
	SDL_FreeSurface(open2);
	SDL_FreeSurface(open3);
	SDL_FreeSurface(open4);
	SDL_FreeSurface(open5);
	SDL_FreeSurface(open6);
	SDL_FreeSurface(open7);
	SDL_FreeSurface(open8);
	SDL_FreeSurface(menu_1);
	SDL_FreeSurface(menu_j);
	SDL_FreeSurface(sur);
	SDL_FreeSurface(scanf_mine);
	SDL_FreeSurface(menu_2);
	SDL_FreeSurface(open10);
	SDL_FreeSurface(mine);
	SDL_FreeSurface(blank);
	TTF_CloseFont(font);
	TTF_CloseFont(font_menu);
	TTF_CloseFont(font_mine);
	TTF_CloseFont(font_sy);
	SDL_Quit();
}

int panduan2(int bit_x[], int bit_y[], int x, int y, int *n_d)	// 判断坐标是否重复
{
	int m, v = 0;
	for (m = 0; m <= *n_d; m++)
	{
		if (((x / 68 + 1) != bit_x[m]) || ((y / 68 + 1) != bit_y[m]))
			v = 1;
		else
		{
			v = 0;
			break;
		}
	}
	return v;
}

void enter1(int x, int y, int a[N][N])	// 方向按键雷区重复刷新
{
	int b, m, n;
	b = a[y / 68 + 1][x / 68 + 1];
	if (b == 0)
	{
		weizhi(x, y, open0, chuang_copy);
	}
	if (b == 1)
		weizhi(x, y, open1, chuang_copy);
	if (b == 2)
		weizhi(x, y, open2, chuang_copy);
	if (b == 3)
		weizhi(x, y, open3, chuang_copy);
	if (b == 4)
		weizhi(x, y, open4, chuang_copy);
	if (b == 5)
		weizhi(x, y, open5, chuang_copy);
	if (b == 6)
		weizhi(x, y, open6, chuang_copy);
	if (b == 7)
		weizhi(x, y, open7, chuang_copy);
	if (b == 8)
		weizhi(x, y, open8, chuang_copy);
	if (b == -1)
	{
		/* weizhi(130, 200, font_text, chuang_copy); */
		weizhi(x, y, mine, chuang_copy);
	}
}

int enter(int x, int y, int a[N][N])	// 确实键坐标雷数判定
{
	int b, big = 0, i, j;
	b = a[y / 68 + 1][x / 68 + 1];
	if (b == 0)
	{
		weizhi(x, y, open0, chuang_copy);
		big = 2;
	}
	if (b == 1)
		weizhi(x, y, open1, chuang_copy);
	if (b == 2)
		weizhi(x, y, open2, chuang_copy);
	if (b == 3)
		weizhi(x, y, open3, chuang_copy);
	if (b == 4)
		weizhi(x, y, open4, chuang_copy);
	if (b == 5)
		weizhi(x, y, open5, chuang_copy);
	if (b == 6)
		weizhi(x, y, open6, chuang_copy);
	if (b == 7)
		weizhi(x, y, open7, chuang_copy);
	if (b == 8)
		weizhi(x, y, open8, chuang_copy);
	if (b == -1)
	{
		big = 1;
		weizhi(x, y, mine, chuang_copy);
		for (i = 0; i <= (N - 2) * 68 - 68; i += 68)
			for (j = 0; j <= (N - 2) * 68 - 68; j += 68)
			{
				if (a[i / 68 + 1][j / 68 + 1] == -1)
					enter1(j, i, a);
			}
		weizhi((P_W - font_win->w) / 2, (P_H - font_win->h) / 2, font_text,
			   chuang_copy);
	}
	return big;
}

void panduan(int bit_x[], int bit_y[], int x, int y, int *n_d, int a[N][N])	// 生成界面是否存在判定
{
	int m, v = -1;
	for (m = 0; m <= *n_d; m++)
	{
		if (((x / 68 + 1) != bit_x[m]) || ((y / 68 + 1) != bit_y[m]))
			v = 1;
		else
		{
			v = 0;
			break;
		}
	}
	if (v == 1)
		weizhi(x, y, blank, chuang_copy);
	else
		enter1(x, y, a);
}

void panduan1(int x, int y)		// 光标
{
	weizhi(x, y, open10, chuang_copy);

}

void enter_back(int bit_x[], int bit_y[], int x, int y, int a[N][N], int *n_d)	// 空白区域刷新递归调用
{
	int m, n;
	if (y >= 0 && y <= (N - 2) * 68 - 68 && x >= 0 && x <= (N - 2) * 68 - 68)
	{
		if (a[y / 68 + 1][x / 68 + 1] != 0)
			return;
		for (m = y - 68; m <= y + 68; m += 68)
		{
			for (n = x - 68; n <= x + 68; n += 68)
			{
				if (m >= 0 && m <= (N - 2) * 68 - 68 && n >= 0
					&& n <= (N - 2) * 68 - 68)
				{
					if (a[m / 68 + 1][n / 68 + 1] == -1)
						return;
					if (panduan2(bit_x, bit_y, n, m, n_d) == 1)
					{
						bit_x[*n_d] = (n / 68 + 1);
						bit_y[*n_d] = (m / 68 + 1);
						(*n_d)++;
						enter1(n, m, a);
						enter_back(bit_x, bit_y, n, m, a, n_d);
					}
				}
			}
		}
		return;
	}
	return;
}

void put(int bit_x[], int bit_y[], int *n_d, int a[N][N])	// 生成界面
{
	int i, j;
	for (i = 0; i <= (N - 2) * 68 - 68; i += 68)
		for (j = 0; j <= (N - 2) * 68 - 68; j += 68)
			panduan(bit_x, bit_y, j, i, n_d, a);
}

void esc(int bit_x[], int bit_y[], int a[N][N], int *n_d)	// 首要
{
	int i;
	for (i = 0; i < (N - 2) * (N - 2); i++)
	{
		bit_x[i] = -10;
		bit_y[i] = -10;
	}
	rand(a);
	put(bit_x, bit_y, n_d, a);
	weizhi(0, 0, open10, chuang_copy);
}

void menu_img(int x, int y)		// 菜单界面
{
	SDL_FillRect(chuang_copy, &chuang_copy->clip_rect, ger);
	weizhi(x, y, menu_j, chuang_copy);
	weizhi((P_W - menu_1->w) / 2, (P_H - menu_1->h) / 2, menu_1, chuang_copy);
	weizhi((P_W - menu_2->w) / 2,
		   (P_H - menu_2->h + menu_2->w + 80) / 2, menu_2, chuang_copy);
	SDL_Flip(chuang_copy);
}

void sum_char(int sum[2])		// 数字拆分
{
	int b, c;
	if (M < 10)
	{
		sum[0] = 0;
		sum[1] = M;
	}
	else
	{
		b = M / 10;
		c = M % ((M / 10) * 10);
		sum[0] = b;
		sum[1] = c;
	}
}

void menu_sum()					// 雷数输入界面及数字打印
{
	int sum[2];
	int b, x = (P_W - menu_shu[0]->w * 3) / 2;
	SDL_FillRect(chuang_copy, &chuang_copy->clip_rect, ger);
	weizhi((P_W - sur->w) / 2, 0, sur, chuang_copy);
	weizhi(20, (P_W - menu_shu[0]->h) / 2 + 10, scanf_mine, chuang_copy);
	sum_char(sum);
	for (b = 0; b < 2; b++, x = x + 50)
	{
		if (sum[b] == 0)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[0], chuang_copy);
		if (sum[b] == 1)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[1], chuang_copy);
		if (sum[b] == 2)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[2], chuang_copy);
		if (sum[b] == 3)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[3], chuang_copy);
		if (sum[b] == 4)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[4], chuang_copy);
		if (sum[b] == 5)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[5], chuang_copy);
		if (sum[b] == 6)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[6], chuang_copy);
		if (sum[b] == 7)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[7], chuang_copy);
		if (sum[b] == 8)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[8], chuang_copy);
		if (sum[b] == 9)
			weizhi(x, (P_W - menu_shu[0]->h) / 2, menu_shu[9], chuang_copy);
	}
	SDL_Flip(chuang_copy);
}

int menu()						// 开始菜单
{
	bool me = false, ke = false;
	int sum1 = -1, sum2 = -1;
	int x = (P_W - menu_1->w) / 2 - menu_j->w, y = (P_H - menu_1->h) / 2;
	menu_img(x, y);
	while (me != true)
	{
		if (SDL_PollEvent(&menu_event))
		{
			if (menu_event.type == SDL_KEYDOWN)
				switch (menu_event.key.keysym.sym)
				{
				case SDLK_UP:
					if (ke == false)
						if (y > (P_H - menu_1->h) / 2)
							y = y - 120;
					break;
				case SDLK_DOWN:
					if (ke == false)
						if (y < (P_H - menu_1->h + menu_2->w + 80) / 2)
							y = y + 120;
					break;
				case SDLK_RETURN:
					if (ke == false)
					{
						if (y == (P_H - menu_1->h) / 2)
						{
							ke = true;
							break;
						}
						if (y = (P_H - menu_1->h + menu_2->w + 80) / 2)
							quit();
					}
					if (ke == true)
					{
						me = true;
						return 1;
					}
					break;
				case SDLK_0:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 0;
							M = 0;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 0;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_1:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 1;
							M = 1;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 1;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_2:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 2;
							M = 2;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 2;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_3:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 3;
							M = 3;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 3;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_4:
					if (ke == true)
					{
						if (sum1 == -1)
						{
							sum1 = 4;
							M = 4;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 4;
							M = sum1 * 10 + sum2;
						}
					}
					break;
				case SDLK_5:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 5;
							M = 5;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 5;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_6:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 6;
							M = 6;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 6;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_7:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 7;
							M = 7;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 7;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_8:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 8;
							M = 8;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 8;
							M = sum1 * 10 + sum2;
						}
					break;
				case SDLK_9:
					if (ke == true)
						if (sum1 == -1)
						{
							sum1 = 9;
							M = 9;
						}
						else
						{
							if (sum2 == -1)
								sum2 = 9;
							M = sum1 * 10 + sum2;
						}
					break;
				}
			if (ke == false)
				menu_img(x, y);
			if (ke == true)
				menu_sum();

		}
	}
}

void up_down(int a[N][N], int bit_x[], int bit_y[], int *n_d)	// 按键处理及判断
{
	bool mes = false;
	int x = 0, y = 0, big = 0, l;
	while (mes == false)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					if (big != 1)
					{
						if (y > 0)
							y = y - 68;
						put(bit_x, bit_y, n_d, a);
						panduan1(x, y);
					}
					break;
				case SDLK_DOWN:
					if (big != 1)
					{
						if (y < (N - 2) * 68 - 68)
							y = y + 68;
						put(bit_x, bit_y, n_d, a);
						panduan1(x, y);
					}
					break;
				case SDLK_LEFT:
					if (big != 1)
					{
						if (x > 0)
							x = x - 68;
						put(bit_x, bit_y, n_d, a);
						panduan1(x, y);
					}
					break;
				case SDLK_RIGHT:
					if (big != 1)
					{
						if (x < ((N - 2) * 68 - 68))
							x = x + 68;
						put(bit_x, bit_y, n_d, a);
						panduan1(x, y);
					}
					break;
				case SDLK_RETURN:
					if (big != 1)
					{
						big = enter(x, y, a);
						panduan1(x, y);
						if (big == 1)
							break;
						if (panduan2(bit_x, bit_y, x, y, n_d) == 1)
						{
							bit_x[*n_d] = (x / 68 + 1);
							bit_y[*n_d] = (y / 68 + 1);
							panduan1(x, y);
							(*n_d)++;
							if (big == 2)
								enter_back(bit_x, bit_y, x, y, a, n_d);
							if (*n_d == ((N - 2) * (N - 2) - M))
							{
								weizhi((P_W - font_win->w) / 2,
									   (P_H - font_win->h) / 2, font_win,
									   chuang_copy);
								big = 1;
							}
						}
					}
					break;
				case SDLK_a:
					mes == true;
				case SDLK_ESCAPE:
					x = 0;
					y = 0;
					esc(bit_x, bit_y, a, n_d);
					big = 0;
					*n_d = 0;
					break;
				}
			if (mes == true)
				quit();
			SDL_Flip(chuang_copy);
		}
	}
}

int main(int argc, char *args[])
{
	int a[N][N] = {
		0
	}, bit_x[(N - 2) * (N - 2)], bit_y[(N - 2) * (N - 2)], n = 0, *n_d, me = 0;
	n_d = &n;
	if (init() == false)		/* 如果初始化失败则返回1 */
		return 1;
	if (in_image() == false)
		return 1;
	me = menu();
	if (me == 1)
	{
		esc(bit_x, bit_y, a, n_d);
		up_down(a, bit_x, bit_y, n_d);
	}
}