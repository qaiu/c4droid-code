/* c4droid代码手册 *影分身
*dd7262000编写 *TTHHR收集整理
*转载请说明出处 */
// 请开始自动横屏并横屏玩耍！！！！
#include<math.h>
#include<pthread.h>
#include "SDL2/SDL.h"
#define PI 3.14
#define LENGTH(x,y) \
sqrt((x)*(x)+(y)*(y))
int w, h;						// 屏幕大小
float l_angle = 0;				// 羊菇头角度
bool button_m = true, button_o = true, button_r = false;
bool l = false, lm = false, lr = false;
// 羊菇头开关
bool m = false;					// 瞬移
bool totton = false, tottton = true, intton = false;	// 桶
bool other = false, othermove = false;
int otr = 10;
		 // 关于分身
bool rush = false, rushing = false;
int times = 0;
float speed;					// 关于冲撞
typedef struct ton
{
	float x;
	float y;
	struct ton *next;
} tons;
tons *head;

struct warp
{
	float x;
	float y;
	float x2;
	float y2;
	float speed;
	float move;
	float round;
	tons *toton;
	tons *rushton;
} warp;

SDL_Window *win = NULL;			// 窗口
SDL_Renderer *render = NULL;
// 渲染器

#include "gettouch.c"
#include "check.c"
#include "do.c"
#include "draw.c"
#include "drawrush.c"

int main(int, char **)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow("SDL2 DRAW", 0, 0, 0, 0, 1);
	render = SDL_CreateRenderer(win, -1, NULL);	// 初始化

	SDL_GetWindowSize(win, &w, &h);
	// 获取屏幕宽高
	warp.x = w / 2;
	warp.y = h / 2;
	warp.x2 = 0;
	warp.y2 = 0;
	warp.round = 0;
	warp.toton = NULL;

	head = (tons *) malloc(sizeof(tons));	// 申请内存空间
	if (head == NULL)
		exit(0);
	head->next = NULL;			// 建立头结点

	pthread_t tid1;				// 线程号
	pthread_create(&tid1, NULL, gettouch, NULL);
	// 启动线程以获取触屏

	addtons(40, 80);
	addtons(180, 80);
	addtons(50, 200);
	while (1)
	{
		SDL_GetWindowSize(win, &w, &h);
		// 获取屏幕宽高
		warp.speed = w / 270;
		warp.move = w / 5;
		if (lm == true && intton == false)
		{
			warp.x += warp.speed * cos(l_angle / 180 * PI);
			warp.y += warp.speed * sin(l_angle / 180 * PI);
		}
		if (warp.x < 30)
			warp.x = 30;
		if (warp.x > w - 30)
			warp.x = w - 30;
		if (warp.y < 30)
			warp.y = 30;
		if (warp.y > h - 30)
			warp.y = h - 30;

		warp.round += 7;
		draw();

		if (rush == true)
			if (warp.rushton != NULL)
				drawrush();
			else
				rush = false;
	}
}