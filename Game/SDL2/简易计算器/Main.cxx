//c4droid代码手册
//简易计算器[v1.1]
//请赐教摩羯 编写
//白狼·圣狂 修改
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "unistd.h"
#define N 200

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

SDL_Texture *tex[3] = { NULL };
SDL_Surface *sur[2] = { NULL };

SDL_Color color[2] = {
	{255, 0, 0}
	, {0, 255, 0}
};

SDL_Rect p[3];

SDL_Event e;

TTF_Font *font[3] = { NULL };

char *pic = "resources/image/cbg.png",*ttf = "../../../Resources/Font/D2-coding.ttf";          /*
所应用文件的相对路径*/
char way[2][100];          /*
存储路径 */
char token[N];					/* 存放表达式字符串的数组 */
char str[N];					/* 获取输入字符串数组 */
char result[N / 4];				/* 存放结果的数组 */
char K = 'k';					/* 用来判断触摸字符 */
int W, H;						/* 屏幕宽高 */
int x, y, y_1, pst = -1;		/* 用来获取触摸位置 */
int n = 0, m = 0, t = 0;		/* 初始化 */
int computed = 0;				/* 判断是否已经计算0表示没有计算 */
bool quit = false;

void init();					// 加载

void clear();					// 清理

double CX(void);				/* 
								   处理乘除并计算 */

double FS(void);				/* 处理括号,数字,小数点 */

double JJ(void);				/* 处理加减并计算 */

void compute();					/* 
								   最后计算结果 */

int main(int argc, char *argv[])
{
  /*  得到应用文件路径  */
	for ( int i=0; i<2; i++ )
	getcwd ( way[i],sizeof ( way[i] ) );
	strcat ( way[0],pic );
	strcat (way[1],ttf );
	
	init();

	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_FINGERDOWN:
				SDL_DestroyTexture(tex[0]);
				tex[0] = NULL;
				x = e.tfinger.x * 4;
				y = e.tfinger.y * 6;
				y_1 = e.tfinger.y * 12;
				if (x == 0 && y == 2)
					K = '7';
				if (x == 1 && y == 2)
					K = '8';
				if (x == 2 && y == 2)
					K = '9';
				if (x == 3 && y == 2)
					K = '/';
				if (x == 0 && y == 3)
					K = '4';
				if (x == 1 && y == 3)
					K = '5';
				if (x == 2 && y == 3)
					K = '6';
				if (x == 3 && y == 3)
					K = '*';
				if (x == 0 && y == 4)
					K = '1';
				if (x == 1 && y == 4)
					K = '2';
				if (x == 2 && y == 4)
					K = '3';
				if (x == 3 && y == 4)
					K = '-';
				if (x == 0 && y == 5)
					K = '.';
				if (x == 1 && y == 5)
					K = '0';
				if (x == 2 && y == 5)
					K = '=';
				if (x == 3 && y == 5)
					K = '+';
				if (x == 0 && y_1 == 3)
					K = '(';
				if (x == 2 && y_1 == 3)
					K = ')';

				if (x == 3 && y_1 == 3)
				{
					if (str[0] != '\0')
						str[--n] = '\0';
				}

				if (K != 'k')
				{
					str[n++] = K;
					str[n] = '\0';
					if (K == '=')
					{
if ( str[0] == '-' || str[0] == '+' || str[0] == '*' || str[0] == '/' )
{
	for ( int b = n;b >=1;b-- )
           str[b] = str[b-1];
           str[0] = '0';
}
						compute();
						computed = 1;
						sur[0] =
							TTF_RenderText_Blended(font[1], result, color[0]);
						tex[0] = SDL_CreateTextureFromSurface(ren, sur[0]);
						SDL_FreeSurface(sur[0]);
						SDL_QueryTexture(tex[0], NULL, NULL, &p[0].w, &p[0].h);
						p[0].x = 0, p[0].y = H / 8;
						m = 0;
						t = 0;
					}
					K = 'k';
				}

				if (p[1].w > W)
				{
					p[1].x = -(p[1].w - W), p[1].y = 0;
				}
				else
				{
					p[1].x = p[1].y = 0;
				}
				sur[1] = TTF_RenderText_Blended(font[0], str, color[1]);
				tex[1] = SDL_CreateTextureFromSurface(ren, sur[1]);
				SDL_QueryTexture(tex[1], NULL, NULL, &p[1].w, &p[1].h);
				SDL_FreeSurface(sur[1]);
				if (computed == 1)
				{
					for (; n >= 0; --n)
						str[n] = '\0';
					n = 0;
					computed = 0;
				}
				break;
			case SDL_FINGERUP:
				break;
			case SDL_FINGERMOTION:
			if ( e.tfinger.dx*W>100 )
				quit = true;
				break;
			default:
				break;
			}

			SDL_RenderClear(ren);
			SDL_RenderCopy(ren, tex[2], NULL, &p[2]);
			SDL_RenderCopy(ren, tex[1], NULL, &p[1]);
			if (tex[0] != NULL)
				SDL_RenderCopy(ren, tex[0], NULL, &p[0]);
			SDL_RenderPresent(ren);
			SDL_Delay(10);
		}

		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, tex[2], NULL, &p[2]);
		SDL_RenderCopy(ren, tex[1], NULL, &p[1]);
		if (tex[0] != NULL)
			SDL_RenderCopy(ren, tex[0], NULL, &p[0]);
		SDL_RenderPresent(ren);
		SDL_Delay(10);
	}

	clear();
	TTF_Quit();
	SDL_Quit();
}


double JJ(void)
{
	double acting = CX();
	while ((token[m] == '+') || (token[m] == '-'))
		switch (token[m])
		{
		case '+':
			token[++m] = str[t++];
			acting += CX();
			break;
		case '-':
			token[++m] = str[t++];
			acting -= CX();
			break;
		}
	return acting;
}

double CX(void)
{
	double cs;
	double acting = FS();
	while ((token[m] == '*') || (token[m] == '/'))
		switch (token[m])
		{
		case '*':
			token[++m] = str[t++];
			acting *= FS();
			break;
		case '/':
			token[++m] = str[t++];
			cs = FS();
			if (cs == 0)		/*
			除数为0时*/
			{
			sprintf(result, "error!");
			}
			acting /= cs;
			break;
		}
	return acting;
}

double FS(void)
{
	double acting;
	char number[N];
	int i = 0;
	if (token[m] == '(')
	{
		token[++m] = str[t++];
		acting = JJ();
		if (token[m] == ')')
			token[++m] = str[t++];
	}
	else if (isdigit(token[m]) || token[m] == '.')
	{
		while (isdigit(token[m]) || token[m] == '.')	/* 将字符串转换为浮点数  */
		{
			number[i++] = token[m++];
			token[m] = str[t++];
		}
		number[i] = '\0';
		acting = atof(number);
	}
	return acting;
}

void compute()
{
	double end;
	token[m] = str[t++];
	end = JJ();
	if (token[m] == '=')
	{
		token[m] = '\0';
		sprintf(result, "=%g", end);
	}
}

void clear()
{
	for (int i = 0; i < 3; i++)
	{
		SDL_DestroyTexture(tex[i]);
		if (i <=1 )
			TTF_CloseFont(font[i]);
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
}

void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	font[0] = TTF_OpenFont(ttf, 45);
	font[1] = TTF_OpenFont(ttf, 80);
	
	win =
		SDL_CreateWindow("t", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						 720, 1280, SDL_WINDOW_SHOWN);
	ren =
		SDL_CreateRenderer(win, -1,
						   SDL_RENDERER_ACCELERATED |
						   SDL_RENDERER_PRESENTVSYNC);

	SDL_GetWindowSize(win, &W, &H);
	
	p[2].x = p[2].y = 0;
	p[2].w = W, p[2].h = H;

	tex[2] = IMG_LoadTexture(ren, pic);

}