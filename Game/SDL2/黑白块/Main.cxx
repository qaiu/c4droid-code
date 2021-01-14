/*
 *  项目:     黑白块(Black-White)[v1.0]
 *  创建时间:  2020-8-29
 *  作者:     白狼·圣狂
 *
 *
 *  代码开发环境:
 *     编辑器:  C4droid 6.97
 *     编译器:  G++
 *     系统:   Android 6.0(arm)
 *
 *  编译&运行:
 *     支持所有安装了SDL2.0.3
 *  的C++编译器，支持Windows/
 *  Android(Linux没试过，有意
 *  者可以帮忙测试)
 *
 *
 *  游戏方法:
 *     与经典黑白块相同，在黑块
 *  离开屏幕之前按下，一定要避免
 *  按到白块
 *  
 *  其它特性:
 *     增加了对玩家的容错，以解
 *  决因为黑块移动过快而造成按下
 *  失灵的情况。
 *
 *  ps:
 *     目前仅实现了街机模式下的
 *  黑白块，各部分较为单调，BUG
 *  较多，后续将会推翻重写
 *     目前已发现的BUG有:
 *
 *     1.结束时往上滑的阶段会出
 *  现新增黒块被提上来
 *     2.因为设备分辨率原因导致
 *  字体过小
 *     3.调整块数量会改变游戏速
 *  度
 *
 *    如果发现除此之外的BUG或者
 *  有更多改进建议，欢迎告知作者
 *  ，万分感谢!
 */  


/**************************/
/*         头文件          */
/**************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <GLES/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/**************************/
/*          宏定义         */
/**************************/

/*
 *  每个块的状态:
 *
 *    STATE_WHITE_UP   白块_弹起
 *         _WHITE_DOWN 白块_按下
 *         _BLACK_UP   黑块_弹起
 *         _BLACK_DOWN 黑块_按下
 */  
#define STATE_WHITE_UP 0x01
#define STATE_WHITE_DOWN 0x03
#define STATE_BLACK_UP 0x02
#define STATE_BLACK_DOWN 0x04

/*
 *  颜色量，便于直接替换颜料桶参数:
 *
 *    COLOR_BLACK      黑色
 *         _WHITE      白色
 *         _GREEN      绿色
 *         _RED        红色
 *         _GRAY       灰色
 *         _YELLOW     黄色
 *         _ORANGE     橙色
 */   
#define COLOR_BLACK 0,0,0,255
#define COLOR_WHITE 255,255,255,255
#define COLOR_GREEN 0,255,0,255
#define COLOR_RED 255,0,0,255
#define COLOR_BLUE 0,150,250,255
#define COLOR_GRAY 235,235,235,255
#define COLOR_YELLOW 255,200,0,255
#define COLOR_ORANGE 255,180,0,255

/*
 *  布局状态，控制显示的场景:
 *
 *    TYPE_MENT         封面
 *        _GAME         游戏内
 */
#define TYPE_MENT 0x01
#define TYPE_GAME 0x02

/*
 *  游戏状态，控制游戏的进行:
 *
 *    GAME_STATE_OVER   结束
 *              _PLAUING
 *                      正在进行
 *              _UNDEFINE
 *                      未在游戏
 */ 
#define GAME_STATE_OVER 0x01
#define GAME_STATE_PLAYING 0x3
#define GAME_STATE_UNDEFINE 0x04

/*
 *  块的全局属性，可以修改:
 *
 *    STACK_COUNT       行数
 *         _COMP        长宽比例
 */
#define STACK_COUNT 4
#define STACK_COMP 0.6

/*
 *  字体文件载入大小:
 *    
 *    windowx是一个int型的全局变量
 *  ，是当前窗口宽的像素值
 *
 *  ps:
 *    其实想改成一个函数的，不过嫌麻
 *  烦于是就直接这样写了
 */
#define FONT_SIZE ((windowx/3))

/*
 *  休眠函数参数(弃用但请勿删除，否
 *  则编译不过关)
 */
#define DELAY_SLEEP 1.0

/*
 *  游戏数据校验函数:
 *
 *    SAVEBAST      要校验的字节
 */
#define MAKENUM(SAVEBAST)((~((SAVEBAST&0X0F)^((SAVEBAST>>8)&0XF0)|(SAVEBAST>>16)&(SAVEBAST>>24+0X03))))


/****************************/
/*          结构体           */
/****************************/

/*
 *  块结构:
 *
 *    stack          块位置与大小
 *    state          记录块属性
 *    next           下个块
 */
struct STACK
{
	SDL_Rect stack;
	unsigned short state;
	struct STACK* next;
}*stack;


/****************************/
/*          全局变量         */
/****************************/

// 按钮位置与大小
// [1]为退出预留位。暂未实现
SDL_Rect ment_buttons[2];

// 窗口
SDL_Window* window;

// 渲染器
SDL_Renderer* render;

// 字体文件
TTF_Font* font;

// 事件容器
SDL_Event event;

// 背景音乐文件
Mix_Music* ment_music;

// 音量
float ment_volume;

// 窗口大小
int windowx, windowy;

// 暂停标志位(弃用但请勿删除，
//          否则编译将不过关)
bool IsPause;

// 程序运行控制
bool Run;

// 场景状态
int type;

// 延时控制
double sheep;

// 游戏状态
int game_state;

// 游戏分数(最高)
int game_bast = 0;

// 当前游戏分数
int game_count;

// 链表宽(虚拟)，就是块行的数量
int w_c;

// 链表长(虚拟)，就是块列的数量，
// 这是根据块属性和屏幕计算的，
// 以解决不同分辨率拉伸问题
int h_c;


/****************************/
/*          函数声明         */
/****************************/

/*
 *  初始化环境，如图形界面、资源文
 *  件等等
 *  
 *  参数:
 *     无需参数
 */
void Init();

/*
 *  关闭环境，如图形界面、数据文件
 *  等等
 *
 *  参数:
 *     无需参数
 */
void Quit();

/*
 *  封面按钮处理，检测并且处理按钮
 *  事件
 *
 *  参数:
 *     无需参数
 */  
void MentButton();

/*
 *  显示菜单界面，这是一个管理函数
 *  其下面有子函数
 *
 *  参数:
 *     无需参数
 */
void ShowMent();

/*
 *  块按钮处理，检测并且处理按钮事
 *  件
 *
 *  参数:
 *     无需参数
 */
void GameButton();

/*
 *  显示游戏界面
 *
 *  参数:
 *     无需参数
 */
void ShowGame();

/*
 *  显示封面界面
 *
 *  参数:
 *     无需参数
 */
void ShowMainMenu();	

/*
 *  显示结束界面
 *
 *  参数:
 *     无需参数
 *
 *  ps:
 *     结束界面就是死亡回顾画面
 */
void ShowGameOver();

/*
 *  显示电量图标
 *
 *  参数:
 *     无需参数
 */
void ShowPowerState();

/*
 *  显示时间
 *
 *  参数:
 *     无需参数
 */
void ShowTime();

/*
 *  播放背景音乐
 *
 *  参数:
 *     无需参数
 */
void PlayMainMusic();

/*
 *  停止背景音乐
 *
 *  参数:
 *     无需参数
 */
void HaltMainMusic();

/*
 *  开始游戏
 *
 *  参数:
 *     无需参数
 */
void PlayGame();

/*
 *  初始化游戏资源、如链表
 *
 *  参数:
 *     无需参数
 */
void ReadlyGame();

/*
 *  初始化链表并填充块
 *
 *  参数:
 *     无需参数
 */
void AllocStack();

/*
 *  释放链表
 *
 *  参数:
 *     无需参数
 */
void FreeStack();

/*
 *  显示所有块
 *
 *  参数:
 *     无需参数
 */
void ShowStack();		

/*
 *  显示最好的成绩印在第一个块上
 *
 *  参数:
 *     无需参数
 */
void ShowFirstStack();

/*
 *  让块整体往下移动
 *
 *  参数:
 *     无需参数
 */
void MoveStackDown();

/*
 *  让块整体往上移动
 *
 *  参数:
 *     无需参数
 */
void MoveStackUp();

/*
 *  查找第一个块的位置
 *
 *  参数:
 *     x坐标，y坐标，w宽度，h长度
 */
void FindFirstStack(int* x, int* y, int* w, int* h);

/*
 *  检查并改变块的状态
 *
 *  参数:
 *     x点触坐标，y点触坐标
 *
 *  返回值:
 *     返回块原先的状态
 */
int ChangeLineStack(int x,int y);

/*
 *  显示分数
 *
 *  参数:
 *     无需参数
 */
void ShowCount();

/*
 * 判断是否有红块(被按错)
 *
 *  参数:
 *     无需参数
 *
 *  返回值:
 *     返回非负数为真，否则为假
 */
int JudgeRedInScreen();

/*
 *  判断时候丢失黑块(漏按)
 *
 *  参数:
 *     无需参数
 *
 *  返回值:
 *     1为真，0为假
 */
int JudgeLostInScreen();

/*
 *  闪烁红块
 *
 *  参数:
 *     无需参数
 */
void FlashRed();

/*
 *  闪烁遗失块
 *
 *  参数:
 *     无需参数
 */
void FlashLost();

/*
 *  读取最好成绩
 *
 *  参数:
 *     无需参数
 */
void ReadBast();

/*
 *  写入最好成绩
 *
 *  参数:
 *     无需参数
 */
void WriteBast();

/*
 *  主函数
 */
int main(int argc, char** args)
{
	Init();
	while (Run)
	{
		// 事件循环
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
				case SDL_QUIT:
					IsPause = true;
					Run = false;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					// 动作更新
					if (type == TYPE_MENT)
						MentButton();
					else
						GameButton();
					break;
				
				default:
					break;
			}
		
		// UI更新
		if (type == TYPE_MENT)
			ShowMent();
		else
			ShowGame();
			
		if (event.type == SDL_MOUSEBUTTONDOWN)
			SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
		
		// 延时
		SDL_Delay(DELAY_SLEEP);
	}
	
	Quit();
	
	return 0;
}

void Init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
	TTF_Init();
	
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720 ,1280, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_GetWindowSize(window, &windowx, &windowy);
	SDL_RenderClear(render);
	SDL_RenderPresent(render);
	
	if (!(font = TTF_OpenFont("../../../Resources/Font/05.ttf", FONT_SIZE)))
		if (!(font = TTF_OpenFont("/system/fonts/DroidSans.ttf", FONT_SIZE)))
			exit(EXIT_FAILURE);
	
	
	
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	ment_music = Mix_LoadMUS("resources/audio/Menu.mp3");
	Mix_VolumeMusic(0);
	ment_volume = 0.0;
	
	ReadBast();
	
	IsPause = true;
	Run = true;
	type = TYPE_MENT;
	game_state = GAME_STATE_UNDEFINE;

	SDL_EventState(SDL_DOLLARRECORD, SDL_DISABLE);
	SDL_EventState(SDL_DOLLARGESTURE, SDL_DISABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
	SDL_EventState(SDL_FINGERMOTION, SDL_DISABLE);
	SDL_EventState(SDL_FINGERDOWN, SDL_DISABLE);
	SDL_EventState(SDL_FINGERUP, SDL_DISABLE);
	SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
	SDL_EventState(SDL_KEYUP, SDL_DISABLE);
	
	atexit(FreeStack);
}

void Quit()
{
	SDL_CloseAudio();
	Mix_HaltMusic();
	Mix_FreeMusic(ment_music);
	Mix_Quit();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void ShowMent()
{
	switch (game_state)
	{
		case GAME_STATE_OVER:
			ShowGameOver();
			break;
			
		case GAME_STATE_UNDEFINE:
			ShowMainMenu();
			PlayMainMusic();
			break;
	}
}

void ShowGameOver()
{
	if (game_count > game_bast)
	{
		game_bast = game_count;
		WriteBast();
	}
	
	if (stack)
		FreeStack();
	
	game_state = GAME_STATE_UNDEFINE;
}

void ShowMainMenu()
{
	ShowPowerState();
	SDL_Surface* face;
	SDL_Texture* text;
	SDL_Rect box;
	SDL_SetRenderDrawColor(render, COLOR_WHITE);
	SDL_RenderClear(render);
	
	if ((event.type == SDL_MOUSEBUTTONDOWN) && ((event.button.x >= ment_buttons[0].x) && (event.button.x <= ment_buttons[0].x + ment_buttons[0].w) && (event.button.y >= ment_buttons[0].y) && (event.button.y <= ment_buttons[0].y + ment_buttons[0].h)))
		face = TTF_RenderUTF8_Blended(font, " PLAY ", {COLOR_ORANGE});
	else
		face = TTF_RenderUTF8_Blended(font, " PLAY ", {COLOR_RED});
	
	text = SDL_CreateTextureFromSurface(render, face);
	box.x = (windowx - face->w / 3) / 2;
	box.y = (windowy - face->h / 3) / 4 * 3;
	box.w = face->w / 3;
	box.h = face->h / 3;
	SDL_RenderCopy(render, text, NULL, &box);
	glLineWidth(5);
	box.x -= 10;
	box.y -= 5;
	box.w += 10 * 2;
	box.h += 5 * 2;
	
	if ((event.type == SDL_MOUSEBUTTONDOWN) && ((event.button.x >= ment_buttons[0].x) && (event.button.x <= ment_buttons[0].x + ment_buttons[0].w) && (event.button.y >= ment_buttons[0].y) && (event.button.y <= ment_buttons[0].y + ment_buttons[0].h)))
	{
		glLineWidth(8);
		SDL_SetRenderDrawColor(render, COLOR_GREEN);
	}
	else
		SDL_SetRenderDrawColor(render, COLOR_RED);
	SDL_RenderDrawRect(render, &box);
	SDL_FreeSurface(face);
	SDL_DestroyTexture(text);
	
	ment_buttons[0] = box;
	
	SDL_SetRenderDrawColor(render, COLOR_BLACK);
	face = TTF_RenderUTF8_Blended(font, "BLACK-WHITE", {COLOR_BLACK});
	text = SDL_CreateTextureFromSurface(render, face);
	box.x = (windowx - face->w / 2.6) / 2;
	box.y = (windowy - face->h / 2.6) / 4; 
	box.w = face->w / 2.6;
	box.h = face->h / 2.6;	
	SDL_RenderCopy(render, text, NULL, &box);
	SDL_FreeSurface(face);
	SDL_DestroyTexture(text);
	
	face = TTF_RenderUTF8_Blended(font, "-By BLSK-", {COLOR_BLACK});
	text = SDL_CreateTextureFromSurface(render, face);
	box.x = (windowx - face->w / 5) / 2;
	box.y = (windowy - face->h / 5) / 4 + face->h / 3.5; 
	box.w = face->w / 5;
	box.h = face->h / 5;
	SDL_RenderCopy(render, text, NULL, &box);
	SDL_FreeSurface(face);
	SDL_DestroyTexture(text);
	
	ShowPowerState();
	ShowTime();
	
	SDL_RenderPresent(render);
}

void ShowGame()
{
	HaltMainMusic();
	
	switch (game_state)
	{
		case GAME_STATE_UNDEFINE:
			ReadlyGame();
			break;
			
		case GAME_STATE_PLAYING:
			PlayGame();
			break;
	}
}

void MentButton()
{
	static SDL_Event func_state;
	
	if (func_state.type != event.type)
		func_state = event;
	else 
		return;
	
	if (func_state.type != SDL_MOUSEBUTTONUP)
		return;
		
	switch (game_state)
	{
		case GAME_STATE_UNDEFINE:
			if ((func_state.button.x >= ment_buttons[0].x) && (func_state.button.x <= ment_buttons[0].x + ment_buttons[0].w) && (func_state.button.y >= ment_buttons[0].y) && (func_state.button.y <= ment_buttons[0].y + ment_buttons[0].h))
			type = TYPE_GAME;
			break;
		
		case GAME_STATE_OVER:
			
			break;
	}
}

void GameButton()
{
	static SDL_Event func_state = {type:SDL_MOUSEBUTTONUP};
	SDL_Rect box;
	
	if (func_state.type != event.type)
		func_state = event;
	else
		return;
	
	if (func_state.type != SDL_MOUSEBUTTONDOWN)
		return;
	
	switch (game_state)
	{
		case GAME_STATE_PLAYING:
			ChangeLineStack(func_state.button.x, func_state.button.y);
			break;
			
		case GAME_STATE_UNDEFINE:
			FindFirstStack(&box.x, &box.y, &box.w, &box.h);
			if ((func_state.button.x > box.x) && (func_state.button.x < box.x + box.w) && (func_state.button.y > box.y) && (func_state.button.y < box.y + box.h))
			{
				ChangeLineStack(func_state.button.x, func_state.button.y);
				game_state = GAME_STATE_PLAYING;	
			}
			else
			{
				if (stack)
					FreeStack();
				type=TYPE_MENT;
			}
			break;
	}
}

void ShowPowerState()
{
	int power;
	SDL_Rect box = {10, 10, windowx / 15, (int )(box.w / 2.5)};
	
	glLineWidth(2);
	SDL_SetRenderDrawColor(render, COLOR_BLACK);
	SDL_RenderDrawRect(render, &box);
	
	if (SDL_POWERSTATE_CHARGING == SDL_GetPowerInfo(NULL, &power))
		SDL_SetRenderDrawColor(render, COLOR_GREEN);
	else if (power > 60)
		SDL_SetRenderDrawColor(render, COLOR_BLUE);
	else if (power > 30)
		SDL_SetRenderDrawColor(render, COLOR_ORANGE);
	else
		SDL_SetRenderDrawColor(render, COLOR_RED);
	
	box.x += 2;
	box.y += 2;
	box.w *= power / 100.0;
	box.h -= 3;
	box.w -= 3;
	
	SDL_RenderFillRect(render, &box);
}

void ShowTime()
{
	SDL_Surface* face;
	SDL_Texture* text;
	SDL_Rect box;
	
	static char str_time[10];
	struct tm* local;
	
	time_t times;
	time(&times);
	local = localtime(&times);
	
	sprintf(str_time, "%d:%02d\0", local->tm_hour, local->tm_min);
	
	face = TTF_RenderUTF8_Blended(font, str_time, {COLOR_BLACK});
	text = SDL_CreateTextureFromSurface(render, face);
	
	box.h = windowx / 10;
	box.w = face->w * (box.h * 1.0 / face->h);
	box.x = windowx - box.w - 10;
	box.y = -10;
	
	SDL_RenderCopy(render, text, NULL, &box);
	
	SDL_FreeSurface(face);
	SDL_DestroyTexture(text);
}

void PlayMainMusic()
{
	if (ment_volume == 0.0 && !Mix_PlayingMusic())
		Mix_PlayMusic(ment_music, -1);
	
	if (ment_volume < 128.0)
	{
		ment_volume += 1.6 / 5;
		ment_volume > 128.0 ? ment_volume = 128.0:0;
		Mix_VolumeMusic((int )ment_volume);
	}
}

void HaltMainMusic()
{
	if (ment_volume == 0.0 && Mix_PlayingMusic())
		Mix_HaltMusic();
	
	if (ment_volume > 0.0)
	{
		ment_volume -= 1.6 / 5;
		ment_volume < 0 ? ment_volume = 0.0:0;
		Mix_VolumeMusic((int )ment_volume);
	}
}

void PlayGame()
{
	MoveStackDown();
	
	ShowStack();
	ShowCount();
	
	SDL_RenderPresent(render);
}

void ReadlyGame()
{
	if (!stack)
	{
		srand((unsigned short )time(NULL));
		AllocStack();
		
		game_count = 0;
		sheep = 1.0;
	}
	
	ShowStack();
	ShowFirstStack();
	ShowPowerState();
	ShowTime();
	
	SDL_RenderPresent(render);
}

void AllocStack()
{
	struct STACK* ptr,* pmove;
	float x, y;
	int i, j;
	int temp;
	
	w_c = STACK_COUNT;
	x = windowx * 1.0 / w_c;
	y = x / STACK_COMP;
	h_c = (int )(windowy / y) + 2;
	
	pmove = ptr = (struct STACK*)malloc(sizeof (struct STACK));
	
	for (i = 0; i < h_c; i++)
	{
		temp = rand() % w_c;
		for (j = 0; j < w_c; j++)
		{
			if (!stack)
			{
				stack = pmove;
			}
			else
			{
				pmove->next = ptr;
				pmove = ptr;
			}
			
			pmove->stack.x = (int )floor(j * x);
			pmove->stack.y = (int )floor(windowy - y - i * y);
			pmove->stack.w = x;
			pmove->stack.h = y;
			
			if (j == temp)
				pmove->state = STATE_BLACK_UP;
			else
				pmove->state = STATE_WHITE_UP;
			
			ptr = (struct STACK*)malloc(sizeof (struct STACK));
		}
	}
	
	free(ptr);
	pmove->next = NULL;
}

void FreeStack()
{
	struct STACK* ptr,* pmove;
	
	if (!stack)
		return;
		
	ptr = pmove = stack;
	
	while (ptr->next)
	{
		ptr = ptr->next;
		free(pmove);
		pmove = ptr;
	}
	
	free(pmove);
	stack = NULL;
}

void ShowStack()
{
	struct STACK* ptr = stack;
	glLineWidth(1);
	
	while (ptr)
	{
		switch (ptr->state)
		{
			case STATE_BLACK_UP:
				SDL_SetRenderDrawColor(render, COLOR_BLACK);
				SDL_RenderFillRect(render, &ptr->stack);
				break;
			
			case STATE_BLACK_DOWN:
				SDL_SetRenderDrawColor(render, COLOR_GRAY);
				SDL_RenderFillRect(render, &ptr->stack);
				break;
			
			case STATE_WHITE_UP:
				SDL_SetRenderDrawColor(render, COLOR_WHITE);
				SDL_RenderFillRect(render, &ptr->stack);
				break;
			
			case STATE_WHITE_DOWN:
				SDL_SetRenderDrawColor(render, COLOR_RED);
				SDL_RenderFillRect(render, &ptr->stack);
				break;
		}
		
		SDL_SetRenderDrawColor(render, COLOR_BLACK);
		SDL_RenderDrawRect(render, &ptr->stack);
		
		ptr = ptr->next;
	}
}

void ShowFirstStack()
{
	SDL_Surface* face;
	SDL_Texture* text;
	SDL_Rect box;
	
	struct STACK* ptr = stack;
	static char str[13];
	
	while (ptr && ptr->state != STATE_BLACK_UP)
		ptr = ptr->next;
		
	if (!ptr)
		return;
	
	sprintf(str, "BAST");
	face = TTF_RenderUTF8_Blended(font, str, {COLOR_WHITE});
	text = SDL_CreateTextureFromSurface(render, face);
	
	box.h = ptr->stack.h / 4.0;
	box.w = face->w * (box.h * 1.0 / face->h);
	box.x = ptr->stack.x + (ptr->stack.w - box.w) / 2;
	box.y = ptr->stack.y + ptr->stack.h / 5.0 * 1;
	
	SDL_RenderCopy(render, text, NULL, &box);
	SDL_FreeSurface(face);
	SDL_DestroyTexture(text);
	
	sprintf(str, "%d", game_bast);
	face = TTF_RenderUTF8_Blended(font, str, {COLOR_WHITE});
	text = SDL_CreateTextureFromSurface(render, face);
	
	box.h = ptr->stack.h / 5.0;
	box.w = face->w * (box.h * 1.0 / face->h);
	box.x = ptr->stack.x + (ptr->stack.w - box.w) / 2;
	box.y = ptr->stack.y + ptr->stack.h / 5.0 * 2.5;
	
	SDL_RenderCopy(render, text, NULL, &box);
	SDL_FreeSurface(face);
	SDL_DestroyTexture(text);
}

void MoveStackDown()
{
	struct STACK* ptr,* oldptr,* temp;
	int i, j, k;
	int numtemp;
	float sheeptemp = 100.0f;
	
	ptr = stack;
	
	for (i = 0; i < h_c; i++)
	{
		oldptr = ptr;
		for (j = 0; j < w_c; j++)
		{
			if (ptr->state == STATE_WHITE_DOWN)
			{
				temp = stack;
				
				while (temp && temp != ptr)
				{
					temp->stack.y -= floor(ptr->stack.h * sheep / sheeptemp);
					temp = temp->next;
				}
				
				while (!JudgeRedInScreen())
				{
					ShowStack();
					MoveStackUp();
					SDL_RenderPresent(render);
					SDL_Delay(DELAY_SLEEP);
				}
				FlashRed();
				game_state = GAME_STATE_OVER;
				type = TYPE_MENT;
				return;
			}
				
			if (ptr->state == STATE_BLACK_UP && (ptr->stack.y + floor(ptr->stack.h * sheep / sheeptemp)) >= windowy)
			{
				temp = stack;
				
				while (temp && temp != ptr)
				{
					temp->stack.y -= floor(ptr->stack.h * sheep / sheeptemp);
					temp = temp->next;
				}
				
				while (!JudgeLostInScreen())
				{
					ShowStack();
					MoveStackUp();
					SDL_RenderPresent(render);
					SDL_Delay(DELAY_SLEEP);
				}
				FlashLost();
				game_state = GAME_STATE_OVER;
				type = TYPE_MENT;
				return;
			}
			
			ptr->stack.y += floor(ptr->stack.h * sheep / sheeptemp);
			
			if (ptr->next)
				ptr = ptr->next;	
		}
		if (oldptr->stack.y > windowy)
		{
			temp = ptr;
			numtemp = rand() % w_c;
			
			while (temp->next)
				temp = temp->next;
				
			temp->next = oldptr;
			
			for (k = 0; k < w_c; k++)
			{
				if (i == h_c - 1)
					oldptr->stack.y = temp->stack.y - temp->stack.h;
				else
					oldptr->stack.y = temp->stack.y - temp->stack.h + floor(ptr->stack.h * sheep / sheeptemp);
				
				if (k == numtemp)
					oldptr->state = STATE_BLACK_UP;
				else
					oldptr->state = STATE_WHITE_UP;
					
				if (k != w_c - 1)
					oldptr = oldptr->next;
			}
			
			stack = oldptr->next;
			oldptr->next = NULL;
		}
	}
}

void MoveStackUp()
{
	struct STACK* pEnd = stack,* pFirst = NULL,* pTemp = stack;
	int px;
	int numtemp;
	int i = 0;
	float sheeptemp = 100.0f;
	
	while (pTemp)
	{
		pTemp->stack.y -= floor(pTemp->stack.h * sheep / sheeptemp);
		pTemp = pTemp->next;
	}
	
	while (pEnd->next)
	{
		if (!pFirst && pEnd->stack.y + pEnd->stack.h <= 0)
			pFirst = pEnd;
			
		if (pFirst)
			i++;
		
		pEnd = pEnd->next;
	}
	
	i = (i + 1) / w_c + 1;
	
	if (pFirst)
	{
		pEnd = pFirst;
		px = 0;
		while (pEnd->next)
		{
			if (px % w_c == 0)
			{
				numtemp = rand() % w_c;
				px = 0;
				pTemp = stack;
				i--;
			}
			
			if (px == numtemp)
				pEnd->state = STATE_BLACK_UP;
			else
				pEnd->state = STATE_WHITE_UP;
			
			pEnd->stack = pTemp->stack;
			pEnd->stack.y += pTemp->stack.h * i;
			
			pTemp = pTemp->next;
			pEnd = pEnd->next;
			px++;
		}
		
		if (px == numtemp)
			pEnd->state = STATE_BLACK_UP;
		else
			pEnd->state = STATE_WHITE_UP;
			
		pEnd->stack = pTemp->stack;
		pEnd->stack.y += pTemp->stack.h * i;
		
		pTemp = stack;
		if (pTemp != pFirst)
		{
			while (pTemp->next != pFirst)
			{
				pTemp = pTemp->next;
			}
			
			pTemp->next = NULL;
			pEnd->next = stack;
			stack = pFirst;
		}
	}
}

void FindFirstStack(int* x, int* y, int* w, int* h)
{
	struct STACK* ptr = stack;
	
	while (ptr)
	{
		if (ptr->state == STATE_BLACK_UP)
		{
			*x = ptr->stack.x;
			*y = ptr->stack.y;
			*w = ptr->stack.w;
			*h = ptr->stack.h;
			break;
		}
		
		ptr = ptr->next;		
	}
}

int ChangeLineStack(int x, int y)
{
	struct STACK* ptr,* temp;
	int flag = 0;
	int state = 0;
	int i, j;
	
	ptr = stack;
	
	for (i = 0; i < h_c; i++)
	{
		temp = ptr;
		for (j = 0; j < w_c; j++)
		{
			if (ptr->state == STATE_BLACK_UP)
			{
				flag = 1;
				break;
			}
			
			ptr = ptr->next;
		}
		
		if (flag)
			break;
	}
	
	ptr = temp;
	
	for (i = 0; i < w_c; i++)
	{
		if ((ptr->stack.x < x) && (ptr->stack.x + ptr->stack.w > x) && (ptr->stack.y - ptr->stack.y / 2 < y) && (ptr->stack.y + ptr->stack.h + ptr->stack.h / 2 > y ))
		{
			state = ptr->state;
			if (ptr->state == STATE_BLACK_UP)
			{
				ptr->state = STATE_BLACK_DOWN;
				game_count++;
				sheep += pow(0.5 / sheep, 1.7);
			}
			
			if (ptr->state == STATE_WHITE_UP && (ptr->stack.y < y) && (ptr->stack.y + ptr->stack.h > y ))
				ptr->state = STATE_WHITE_DOWN;
			break;
		}
		
		ptr = ptr->next;
	}
	
	return state;
}

void ShowCount()
{
	static char str[13];
	SDL_Rect box;
	SDL_Surface* face;
	SDL_Texture* text;
	
	sprintf(str, "%d", game_count);
	
	if (game_count > game_bast)
	{
		box.h = windowy / 8;
		face = TTF_RenderUTF8_Blended(font, str, {COLOR_RED});
	}
	else
	{
		box.h = windowy / 9;
		face = TTF_RenderUTF8_Blended(font, str, {COLOR_GREEN});	
	}
		
	text = SDL_CreateTextureFromSurface(render, face);
	
	box.w = box.h * 1.0 / face->h * face->w;
	box.x = (windowx - box.w) / 2.0;
	box.y = windowy / 13.0;
	
	SDL_RenderCopy(render, text, NULL, &box);
	
	SDL_FreeSurface(face);
	SDL_DestroyTexture(text);
}

int JudgeRedInScreen()
{
	struct STACK* ptr = stack;
	
	if (!ptr)
		return 0;
	
	while (ptr && ptr->state != STATE_WHITE_DOWN)
		ptr = ptr->next;
	
	return ptr->stack.y + ptr->stack.h < windowy;
}

int JudgeLostInScreen()
{
	struct STACK* ptr = stack;
	static int i = 0;
	float sheeptemp = 100.0f;
	
	if (!ptr)
		return 0;
	
	while (ptr && ptr->state != STATE_BLACK_UP)
		ptr = ptr->next;
	
	if (i >= ptr->stack.h)
	{
		i = 0;
		return 1;
	}
	
	i += floor(ptr->stack.h * sheep / sheeptemp);
	
	return 0;
}

void FlashRed()
{
	struct STACK* ptr = stack;
	SDL_Color color = {COLOR_RED};
	int i, j = 0;
	
	while (ptr && ptr->state != STATE_WHITE_DOWN)
		ptr = ptr->next;
	
	for (i = 0; i < 3; i++)
	{
		for (;j < 248; j+=8)
		{
			ShowStack();
			
			SDL_SetRenderDrawColor(render, color.r, color.g + j, color.b + j, color.a);
			SDL_RenderFillRect(render, &ptr->stack);
			
			SDL_RenderPresent(render);
			SDL_Delay(DELAY_SLEEP);
		}
		
		for (;j > 0; j-=8)
		{
			ShowStack();
			
			SDL_SetRenderDrawColor(render, color.r, color.g + j, color.b + j, color.a);
			SDL_RenderFillRect(render, &ptr->stack);
			
			SDL_RenderPresent(render);
			SDL_Delay(DELAY_SLEEP);
		}
	}
}

void FlashLost()
{
	struct STACK* ptr = stack;
	SDL_Color color = {COLOR_BLACK};
	int i, j = 0;
	
	while (ptr && !(ptr->state == STATE_BLACK_UP && ptr->stack.y + ptr->stack.h < windowy))
		ptr = ptr->next;
	
	for (i = 0; i < 3; i++)
	{
		for (;j < 248; j+=8)
		{
			ShowStack();
			
			SDL_SetRenderDrawColor(render, color.r + j, color.g + j, color.b + j, color.a);
			SDL_RenderFillRect(render, &ptr->stack);
			
			SDL_RenderPresent(render);
			SDL_Delay(DELAY_SLEEP);
		}
		
		for (;j > 0; j-=8)
		{
			ShowStack();
			
			SDL_SetRenderDrawColor(render, color.r + j, color.g + j, color.b + j, color.a);
			SDL_RenderFillRect(render, &ptr->stack);
			
			SDL_RenderPresent(render);
			SDL_Delay(DELAY_SLEEP);
		}
	}
}

void ReadBast()
{
	int num, make;
	FILE* fp = fopen("resources/user/score.dat", "r");
	
	if (!fp)
	{
		game_bast = 0;
		return;
	}
	
	fread(&num, sizeof (int ), 1, fp);
	fread(&make, sizeof (int ), 1, fp);
	
	if (MAKENUM(num) == make)
	{
		game_bast = num;
		fclose(fp);
	}
	else
	{
		game_bast = 0;
		fclose(fp);
		remove("resources/user/score.dat");
	}
}

void WriteBast()
{
	int num, make;
	FILE* fp = fopen("resources/user/score.dat", "w");
	
	if (!fp)
		return;
	
	num = game_bast;
	make = MAKENUM(num);
	
	fwrite(&num, sizeof (int ), 1, fp);
	fwrite(&make, sizeof (int ), 1, fp);
	
	fclose(fp);
}