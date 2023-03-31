/* 所有的全局变量,变量初始化和启动函数都在此文件 */

// 包含的头文件
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdlib.h>
#include <string>
#define W 960
#define H 540
// 屏幕
#define BOX_W 60
// 方块边长
#define F 10
#define FPS 60
// 游戏帧速
#define FPSM 50
// 菜单帧速

SDL_Surface *screen = NULL;		// 屏幕
SDL_Surface *background = NULL;
// 背景
SDL_Surface *box_a = NULL;		// 方块a
SDL_Surface *box_b = NULL;		// 方块b
SDL_Surface *box_rect1 = NULL;
SDL_Surface *box_rect2 = NULL;
SDL_Surface *box_rect3 = NULL;
SDL_Surface *box_rect4 = NULL;
SDL_Surface *box_rect5 = NULL;
SDL_Surface *box_rect6 = NULL;
// 障碍物
SDL_Surface *menu_b1 = NULL;
// 菜单
SDL_Surface *menu_b2 = NULL;
// 菜单背景
SDL_Surface *menu_help = NULL;
// 帮助背景
SDL_Surface *menu_lock = NULL;
// 关卡上锁
SDL_Surface *num_1, *num_2, *num_3, *num_4, *num_5, *num_6, *num_7, *num_8;	// 关卡数字
SDL_Surface *m_0 = NULL;
SDL_Surface *m_1 = NULL;
SDL_Surface *m_2 = NULL;
SDL_Surface *m_3 = NULL;
SDL_Surface *m_4 = NULL;
SDL_Surface *m_5 = NULL;
SDL_Surface *m_6 = NULL;
SDL_Surface *m_7 = NULL;
SDL_Surface *m_8 = NULL;
// 文字信息
SDL_Surface *blue[8];			// 框
SDL_Surface *red[8];
TTF_Font *font = NULL;			// 字体
SDL_Rect boxa;
SDL_Rect boxb;
SDL_Event event, event_h;		// 按键

// 颜色
SDL_Color color = { 250, 250, 250 };
SDL_Color color2 = { 255, 180, 87 };

// 音效和音乐
Mix_Chunk *start_ogg = NULL;
Mix_Chunk *die_ogg = NULL;
Mix_Chunk *win_ogg = NULL;
Mix_Music *menu_ogg = NULL;
Mix_Music *game_ogg = NULL;

// 各个变量
int i = 0, frame = 0, n = 1, frame_s = 0;
int vx = 0, vy = 0;
int life = 1, next = 0;
int l = 1, lock_level = 1, open_level = 1;
int ch_save = 0;
int ch_m = 0, ch_set = 0, ch_level = 0;
int m_n = 0;
int q = 1, q_h = 1, q_ch = 1, q_set = 0, q_save = 1, q_r = 0, q_level = 0;
int set_1 = 0;
char t[10], le[10];

// 方块结构体
struct box
{
	int x;
	int y;
} a, b;

// 箭头结构体
struct dot
{
	SDL_Surface *Dot = NULL;
	int dx = 0;
	int dy = 0;
	int dv = 0;
} da;

// 时间结构体
struct timer
{
	int startTicks = 0;
	int pausedTicks = 0;
	int loadTicks = 0;
	int ticks = 0;
} Timer, fps, fpsm, t_save;

// 保存游戏结构体
typedef struct gsave
{
	int g_x1;
	int g_y1;
	int g_x2;
	int g_y2;
	int g_l;
	int g_time;
};

typedef struct save_l
{
	int ol = 1;
};

typedef struct save_v
{
	int off_v = 0;
};

gsave g_data;
save_l g_lock;
save_v g_voice;

SDL_Surface *load_image(std::string filename)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;
	loadedImage = IMG_Load(filename.c_str());
	if (loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	return optimizedImage;
}

// 启动
int init_1()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return 1;
	}
	if (TTF_Init() == -1)
	{
		return 1;
	}
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	font = TTF_OpenFont("data/Font.ttf", 30);
	screen = SDL_SetVideoMode(W, H, 32, SDL_SWSURFACE);
	m_0 = TTF_RenderUTF8_Solid(font, "时间", color);
	m_2 = TTF_RenderUTF8_Solid(font, "关卡", color);
	m_4 = TTF_RenderUTF8_Solid(font, "保存成功", color);
	m_5 = TTF_RenderUTF8_Solid(font, "是否开启声音?", color2);
	m_6 = TTF_RenderUTF8_Solid(font, "开", color2);
	m_7 = TTF_RenderUTF8_Solid(font, "关", color2);
	m_8 = TTF_RenderUTF8_Solid(font, "重置进度", color2);
}

// 启动各个资源
void init_2()
{
	box_a = load_image("data/res/box_a.png");
	box_b = load_image("data/res/box_b.png");
	da.Dot = IMG_Load("data/res/m_j.png");
	menu_b1 = load_image("data/res/menu1.png");
	menu_b2 = load_image("data/res/menu.png");
	menu_help = load_image("data/res/menu_help.png");
	menu_lock = load_image("data/res/menu_lock.png");
	num_1 = IMG_Load("data/res/101.png");
	num_2 = IMG_Load("data/res/102.png");
	num_3 = IMG_Load("data/res/103.png");
	num_4 = IMG_Load("data/res/104.png");
	num_5 = IMG_Load("data/res/105.png");
	num_6 = IMG_Load("data/res/106.png");
	num_7 = IMG_Load("data/res/107.png");
	num_8 = IMG_Load("data/res/108.png");
	box_rect1 = load_image("data/res/boxRect1.png");
	box_rect2 = load_image("data/res/boxRect2.png");
	box_rect3 = load_image("data/res/boxRect3.png");
	box_rect4 = load_image("data/res/boxRect4.png");
	box_rect5 = load_image("data/res/boxRect5.png");
	box_rect6 = load_image("data/res/boxRect6.png");
	background = load_image("data/res/background.png");

	blue[0] = IMG_Load("data/res/blue1.png");
	blue[1] = IMG_Load("data/res/blue2.png");
	blue[2] = IMG_Load("data/res/blue3.png");
	blue[3] = IMG_Load("data/res/blue4.png");
	blue[4] = IMG_Load("data/res/blue5.png");
	blue[5] = IMG_Load("data/res/blue6.png");
	blue[6] = IMG_Load("data/res/blue7.png");
	blue[7] = IMG_Load("data/res/blue8.png");

	red[0] = IMG_Load("data/res/red1.png");
	red[1] = IMG_Load("data/res/red2.png");
	red[2] = IMG_Load("data/res/red3.png");
	red[3] = IMG_Load("data/res/red4.png");
	red[4] = IMG_Load("data/res/red5.png");
	red[5] = IMG_Load("data/res/red6.png");
	red[6] = IMG_Load("data/res/red7.png");
	red[7] = IMG_Load("data/res/red8.png");

	menu_ogg = Mix_LoadMUS("data/music/menu.ogg");
	game_ogg = Mix_LoadMUS("data/music/game.ogg");
	start_ogg = Mix_LoadWAV("data/music/start.ogg");
	win_ogg = Mix_LoadWAV("data/music/win.ogg");
	die_ogg = Mix_LoadWAV("data/music/die.ogg");

	a.x = 5;
	a.y = 5;
	b.x = W - BOX_W - 5;
	b.y = H - BOX_W - 5;
}

// 限帧函数
void Fps()
{
	fps.startTicks = SDL_GetTicks();
	fps.ticks = fps.startTicks - fps.pausedTicks;
	if (fps.ticks < 1000 / FPS)
	{
		SDL_Delay(1000 / FPS - fps.ticks);
	}
	fps.pausedTicks = fps.startTicks;
}

void Fpsm()
{
	fpsm.startTicks = SDL_GetTicks();
	fpsm.ticks = fpsm.startTicks - fpsm.pausedTicks;
	if (fpsm.ticks < 1000 / FPSM)
	{
		SDL_Delay(1000 / FPSM - fpsm.ticks);
	}
	fpsm.pausedTicks = fpsm.startTicks;
}

// 游戏保存
void game_save(gsave s)
{
	FILE *fp;
	fp = fopen("data/game_dat", "wb");
	fwrite(&s, sizeof(gsave), 6, fp);
	fclose(fp);
}

void save_level(save_l v)
{
	FILE *fp1;
	fp1 = fopen("data/game_level", "wb");
	fwrite(&v, sizeof(save_l), 1, fp1);
	fclose(fp1);
}

void save_voice(save_v vo)
{
	FILE *fp2;
	fp2 = fopen("data/game_voice", "wb");
	fwrite(&vo, sizeof(save_v), 1, fp2);
	fclose(fp2);
}

// 读取数据
void input_data()
{
	FILE *fp;
	fp = fopen("data/game_dat", "rb");
	fread(&g_data, sizeof(gsave), 6, fp);
	a.x = g_data.g_x1;
	a.y = g_data.g_y1;
	b.x = g_data.g_x2;
	b.y = g_data.g_y2;
	Timer.loadTicks = g_data.g_time;
	l = g_data.g_l;
	fclose(fp);
	vx = 0;
	vy = 0;
}

void input_level()
{
	FILE *fp1;
	fp1 = fopen("data/game_level", "rb");
	fread(&g_lock, sizeof(save_l), 1, fp1);
	lock_level = g_lock.ol;
	fclose(fp1);
}

void input_voice()
{
	FILE *fp2;
	fp2 = fopen("data/game_voice", "rb");
	fread(&g_voice, sizeof(save_l), 1, fp2);
	set_1 = g_voice.off_v;
	fclose(fp2);
}

// 清理函数
void clean()
{
	SDL_FreeSurface(screen);
	SDL_FreeSurface(background);
	SDL_FreeSurface(box_a);
	SDL_FreeSurface(box_b);
	SDL_FreeSurface(box_rect1);
	SDL_FreeSurface(box_rect2);
	SDL_FreeSurface(box_rect3);
	SDL_FreeSurface(box_rect4);
	SDL_FreeSurface(box_rect5);
	SDL_FreeSurface(box_rect6);
	SDL_FreeSurface(menu_b1);
	SDL_FreeSurface(menu_b2);
	SDL_FreeSurface(menu_help);
	SDL_FreeSurface(menu_lock);
	SDL_FreeSurface(m_0);
	SDL_FreeSurface(m_1);
	SDL_FreeSurface(m_2);
	SDL_FreeSurface(m_3);
	SDL_FreeSurface(m_4);
	SDL_FreeSurface(m_5);
	SDL_FreeSurface(m_6);
	SDL_FreeSurface(m_7);
	SDL_FreeSurface(m_8);
	SDL_FreeSurface(num_1);
	SDL_FreeSurface(num_2);
	SDL_FreeSurface(num_3);
	SDL_FreeSurface(num_4);
	SDL_FreeSurface(num_5);
	SDL_FreeSurface(num_6);
	SDL_FreeSurface(num_7);
	SDL_FreeSurface(num_8);
	int k;
	for (k = 0; k < 9; k++)
	{
		SDL_FreeSurface(blue[k]);
		SDL_FreeSurface(red[k]);
	}
	TTF_CloseFont(font);
	TTF_Quit();
	Mix_FreeChunk(win_ogg);
	Mix_FreeChunk(die_ogg);
	Mix_FreeChunk(start_ogg);
	Mix_FreeMusic(menu_ogg);
	Mix_FreeMusic(game_ogg);
	Mix_CloseAudio();
	SDL_Quit();
}

int menu();
int choose();
void new_game();
void con_game();
void help();
int help_input();
int choose_level();
void setting();
void game_start();
void win();
void input_d();
void move_d();
void apply(int, int, SDL_Surface *, SDL_Surface *, SDL_Rect *);
bool check(SDL_Rect, SDL_Rect);
void over();
void game();