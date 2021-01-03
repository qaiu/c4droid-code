/*
 *  Nes模拟器1.1 SDL2版
 *  原作品: 见mynes.c文件
 *  移植: 白狼·圣狂
 *  说明: 从SDL到SDL2的移植，
 *       原作品稍有修改，优化代码，
 *       运行从此处开始
 *  编译:
 *       32位G++17
 */


#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


#define SDLglue_Surface SDL_Surface
#define SDLglue_Event SDL_Event

#define SDLglue_KEYDOWN SDL_KEYDOWN
#define SDLglue_KEYUP SDL_KEYUP
#define SDLglue_INIT_EVERYTHING (SDL_INIT_VIDEO | SDL_INIT_EVENTS)
#define SDLglue_HWSURFACE NULL

#define SDLglue_Delay SDL_Delay
#define SDLglue_GetTicks SDL_GetTicks
#define SDLglue_PollEvent SDL_PollEvent
#define SDLglue_MUSTLOCK SDL_MUSTLOCK
#define SDLglue_LockSurface SDL_LockSurface
#define SDLglue_UnlockSurface SDL_UnlockSurface
#define SDLglue_FillRect SDL_FillRect
#define SDLglue_Init SDL_Init


enum BUTTONENUM{
	BUTTON_BACK,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_ENTER,
	BUTTON_ESC,
	BUTTON_MAX
};

const char* BUTTONPATH[BUTTON_MAX] = {
	"../../../Resources/Image/AndroidUI/lineLight/lineLight07.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight01.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight08.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight03.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight04.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight34.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight35.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight31.png",
	"../../../Resources/Image/AndroidUI/lineLight/lineLight33.png"
};


SDL_Window* window;
SDL_Renderer* render;

SDL_bool UsableButton;
SDL_bool ButtonState[BUTTON_MAX];
SDL_Texture* ButtonImg[BUTTON_MAX];
SDL_Rect ButtonRect[BUTTON_MAX];


int SDLglue_UpdateRect(SDL_Surface* surface, int x, int y, int w, int h)
{
	static SDL_Texture* texture;
	static SDL_Rect rect = {};
	static int temp;
	
	SDL_GetWindowSize(window, &rect.w, &rect.h);
	if (rect.w > rect.h)
	{
		temp = rect.w;
		rect.w = w * rect.h / h;
		rect.x = (temp - rect.w) / 2;
		rect.y = 0;
	} else {
		temp = rect.h;
		rect.h = h * rect.w / w;
		rect.y = (temp - rect.h) / 2;
		rect.x = 0;
	}
	
	if (!(texture = SDL_CreateTextureFromSurface(render, surface)))
		return -1;
	SDL_RenderCopy(render, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	
	return 0;
}

SDL_Surface* SDLglue_SetVideoMode(int w, int h, int dip, int padding)
{
	if (!(window = SDL_CreateWindow("Nes模拟器", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1028, 720, SDL_WINDOW_SHOWN)))
		exit(EXIT_FAILURE);
	if (!(render = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)))
		exit(EXIT_FAILURE);
	
	return SDL_CreateRGBSurface(0, w, h, dip, 0, 0, 0, 0);
}

void SDLglue_UnableEvent()
{
	SDL_EventState(
	        SDL_MOUSEMOTION  |
	        SDL_FINGERMOTION ,
	        SDL_DISABLE      );
}

void SDLglue_InitButton()
{
	int i;
	static SDL_Surface* surface;
	
	for (i = BUTTON_BACK; i != BUTTON_MAX; i++)
	{
		if (!(surface = IMG_Load(BUTTONPATH[i])))
		{
			UsableButton = SDL_FALSE;
			return;
		}
		
		if (!(ButtonImg[i] = SDL_CreateTextureFromSurface(render, surface)))
		{
			SDL_FreeSurface(surface);
			UsableButton = SDL_FALSE;
			return;
		}
		
		ButtonState[i] = SDL_FALSE;
		
		SDL_FreeSurface(surface);
	}
	
	UsableButton = SDL_TRUE;
}

void SDLglue_FixButtonRect()
{
	static int w, h;
	static int min;
	int i;
	
	SDL_GetWindowSize(window, &w, &h);
	min = w < h ? w : h;
	
	for (i = BUTTON_BACK; i != BUTTON_MAX; i++)
		switch (i)
		{
			case BUTTON_BACK:
				ButtonRect[i] = {
					x: 0,
					y: h - min / 2,
					w: min / 2,
					h: min / 2
				};
				break;
			
			case BUTTON_ENTER:
				ButtonRect[i] = {
					x: w - min / 8,
					y: 0,
					w: min / 8,
					h: min / 8
				};
				break;
			
			case BUTTON_ESC:
				ButtonRect[i] = {
					x: 0,
					y: 0,
					w: min / 8,
					h: min / 8
				};
				break;
			
			case BUTTON_A:
				ButtonRect[i] = {
					x: w - min / 2,
					y: h - min / 4,
					w: min / 4,
					h: min / 4
				};
				break;
			
			case BUTTON_B:
				ButtonRect[i] = {
					x: w - min / 4,
					y: h - min / 2,
					w: min / 4,
					h: min / 4
				};
				break;
			
			case BUTTON_UP:
				ButtonRect[i] = {
					x: min / 6,
					y: h - min / 2,
					w: min / 6,
					h: min / 4
				};
				break;
			
			case BUTTON_DOWN:
				ButtonRect[i] = {
					x: min / 6,
					y: h - min / 4,
					w: min / 6,
					h: min / 4
				};
				break;
			
			case BUTTON_LEFT:
				ButtonRect[i] = {
					x: 0,
					y: h - min / 3,
					w: min / 4,
					h: min / 6
				};
				break;
			
			case BUTTON_RIGHT:
				ButtonRect[i] = {
					x: min / 4,
					y: h - min / 3,
					w: min / 4,
					h: min / 6
				};
				break;
			
			default:
				break;
		}
}

void SDLglue_ShowButton()
{
	int i;
	
	for (i = BUTTON_BACK; i != BUTTON_MAX; i++)
		switch (i)
		{
			case BUTTON_BACK:
			case BUTTON_A:
			case BUTTON_B:
			case BUTTON_ENTER:
			case BUTTON_ESC:
				SDL_RenderCopy(render, ButtonImg[i], NULL, &ButtonRect[i]);
				break;
			
			case BUTTON_UP:
			case BUTTON_DOWN:
			case BUTTON_LEFT:
			case BUTTON_RIGHT:
				if (ButtonState[i] == SDL_TRUE)
					SDL_RenderCopy(render, ButtonImg[i], NULL, &ButtonRect[i]);
				break;
			
			default:
				break;
		}
}

SDL_bool SDLglue_JudgeInclude(int x, int y, SDL_Rect* rect)
{
	if ((x >= rect->x && x <= rect->x + rect->w) && (y >= rect->y && y <= rect->y + rect->h))
		return SDL_TRUE;
	
	return SDL_FALSE;
}

void SDLglue_HookButton(const SDL_Event* event)
{
	static int Fingers[2] = {-1, -1};
	static SDL_Event sevent;
	static int w, h;
	static int x, y;
	
	if (event->type == SDL_FINGERDOWN && event->tfinger.fingerId < 2)
	{
		SDL_GetWindowSize(window, &w, &h);
		x = event->tfinger.x * w;
		y = event->tfinger.y * h;
		
		if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_LEFT]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_LEFT;
			sevent.key.keysym.sym = SDLK_LEFT;
		}
		else if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_RIGHT]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_RIGHT;
			sevent.key.keysym.sym = SDLK_RIGHT;
		}
		else if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_DOWN]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_DOWN;
			sevent.key.keysym.sym = SDLK_DOWN;
		}
		else if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_UP]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_UP;
			sevent.key.keysym.sym = SDLK_UP;
		}
		else if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_B]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_B;
			sevent.key.keysym.sym = SDLK_4;
		}
		else if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_A]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_A;
			sevent.key.keysym.sym = SDLK_3;
		} 
		else if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_ENTER]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_ENTER;
			sevent.key.keysym.sym = SDLK_RETURN;
		} 
		else if (SDLglue_JudgeInclude(x, y, &ButtonRect[BUTTON_ESC]) == SDL_TRUE)
		{
			Fingers[event->tfinger.fingerId] = BUTTON_ESC;
			sevent.key.keysym.sym = SDLK_ESCAPE;
		}
		else
			return;
		
		sevent.type = SDL_KEYDOWN;
		ButtonState[Fingers[event->tfinger.fingerId]] = SDL_TRUE;
	} 
	else if (event->type == SDL_FINGERUP && event->tfinger.fingerId < 2)
	{
		switch (Fingers[event->tfinger.fingerId])
		{
			case BUTTON_UP:
				sevent.key.keysym.sym = SDLK_UP;
				break;
			
			case BUTTON_DOWN:
				sevent.key.keysym.sym = SDLK_DOWN;
				break;
			
			case BUTTON_LEFT:
				sevent.key.keysym.sym = SDLK_LEFT;
				break;
			
			case BUTTON_RIGHT:
				sevent.key.keysym.sym = SDLK_RIGHT;
				break;
			
			case BUTTON_A:
				sevent.key.keysym.sym = SDLK_3;
				break;
			
			case BUTTON_B:
				sevent.key.keysym.sym = SDLK_4;
				break;
			
			case BUTTON_ENTER:
				sevent.key.keysym.sym = SDLK_RETURN;
				break;
			
			case BUTTON_ESC:
				sevent.key.keysym.sym = SDLK_ESCAPE;
				break;
				
			default:
				return;
		}
		
		sevent.type = SDL_KEYUP;
		ButtonState[Fingers[event->tfinger.fingerId]] = SDL_FALSE;
		Fingers[event->tfinger.fingerId] = -1;
	}
	else
		return;
	
	SDL_PushEvent(&sevent);
}

#include "mynes.c"