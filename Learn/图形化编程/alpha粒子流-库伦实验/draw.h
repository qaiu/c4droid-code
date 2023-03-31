#ifndef _DRAW_H_
#define _DRAW_H_
#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
// SDL2绘图引擎
class Draw
{
public:
	// 函数get_coord_mode和函数set_coord_mode
	enum Coord_mode:int
	{
/* A：以左上角为原点
 * 向右为X轴正方向，向下为Y轴正方向
 */
		A=0,
/* B：以右上角为原点
 * 向左为X轴正方向，向下为Y轴正方向
 */
		B,
/* C：以右下角为原点
 * 向左为X轴正方向，向上为Y轴正方向
 */
		C,
/* D：以左下角为原点
 * 向右为X轴正方向，向上为Y轴正方向
 */
		D
	};
	// 函数rect
	enum Rect_draw_mode:int
	{
		Fill=0,
		Nofill
	};
private:
	static bool on;
	static SDL_Window* win;
	static SDL_Renderer* ren;
	static int w;
	static int h;
	static Coord_mode coord_mode;
	static std::vector<SDL_Texture*> words;
	static TTF_Font* font_lib;
private:
	Draw(){}
	// 这两个函数根据coord_mode的值来转换obj的xy坐标
	// 注：原来的obj的xy坐标必须以屏幕左上角为原点！
	static void conv(SDL_Point& obj);
	static void conv(SDL_Rect& obj);
public:
	static bool init();
	static void quit();
	static Coord_mode get_coord_mode()
	{
		return coord_mode;
	}
	static void set_coord_mode(Coord_mode mod)
	{
		coord_mode=mod;
	}
	static void show()
	{
		if (!on)
			return;
		SDL_RenderPresent(ren);
	}
	static void clear();
	// 获取和更改颜色
	static SDL_Color get_color();
	static void set_color(SDL_Color& c)
	{
		SDL_SetRenderDrawColor (ren,c.r,c.g,c.b,c.a);
	}
	static void set_color(Uint8 r,Uint8 g,Uint8 b,Uint8 a)
	{
		SDL_SetRenderDrawColor(ren,r,g,b,a);
	}
	// 获取屏幕宽高
	static int get_w()
	{
		return w;
	}
	static int get_h()
	{
		return h;
	}
public:
	// 画一个点或者一些点
	static void point(SDL_Point& cd);
	static void points(SDL_Point* points,int n);
	// 画一条线或者一些线
	// 注：lines函数通过将points按下标依次连接来作图
	static void line(SDL_Point& cd1,SDL_Point& cd2);
	static void lines(SDL_Point* points,int n);
	// 绘制矩形
	static void rect(Rect_draw_mode mod,SDL_Rect& rect);
	static void rects(Rect_draw_mode mod,SDL_Rect* rects,int n);
	// 添加文字
	static int add_word(char* ch,SDL_Color col,int size);
	// 将文字复制到render 
	static void copy_word(int index,int x,int y);
	// 查询字符串高度和宽度
	static int get_word_height(int index);
	static int get_word_width(int index);
};
#endif