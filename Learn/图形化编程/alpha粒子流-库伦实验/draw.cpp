#include "draw.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
bool Draw::on=false;
SDL_Window* Draw::win=NULL;
SDL_Renderer* Draw::ren=NULL;
int Draw::w=0;
int Draw::h=0;
Draw::Coord_mode Draw::coord_mode=A;
std::vector<SDL_Texture*> Draw::words;
TTF_Font* Draw::font_lib=NULL;

void Draw::conv(SDL_Point& obj)
{
	if (!on)
		return;
	if (coord_mode==A)
		return;
	else if (coord_mode==B)
		obj.x=w-obj.x;
	else if (coord_mode==C)
	{
		obj.x=w-obj.x;
		obj.y=h-obj.y;
	}
	else if (coord_mode==D)
		obj.y=h-obj.y;
}

void Draw::conv(SDL_Rect& obj)
{
	if (!on)
		return;
	if (coord_mode==A)
		return;
	else if (coord_mode==B)
		obj.x=w-obj.x-obj.w;
	else if (coord_mode==C)
	{
		obj.x=w-obj.x-obj.w;
		obj.y=h-obj.y-obj.h;
	}
	else if (coord_mode==D)
		obj.y=h-obj.y-obj.h;
}

bool Draw::init()
{
	if (on)
		return true;
	if (SDL_Init(SDL_INIT_EVERYTHING)!=0)
	{
		return false;
	}
	if (TTF_Init()!=0)
	{
		return false;
	}
	win=SDL_CreateWindow ("Tetris",SDL_WINDOW_ALLOW_HIGHDPI,0,0,0,0);
	if (win==NULL)
	{
		SDL_Quit();
		return false;
	}
	ren=SDL_CreateRenderer (win,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
	if (ren==NULL)
	{
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	}
	SDL_GetWindowSize(win,&w,&h);
	return on=true;
}

void Draw::quit()
{
	if (!on)
		return;
	if (ren!=NULL)
		SDL_DestroyRenderer(ren);
	if (win!=NULL)
		SDL_DestroyWindow(win);
	if (font_lib!=NULL)
		TTF_CloseFont(font_lib);
	for (int i=0;i<words.size();i++)
	{
		if (words[i]!=NULL)
		{
			SDL_DestroyTexture(words[i]);
		}
	}
	words.clear();
	TTF_Quit();
	SDL_Quit();
}

void Draw::clear()
{
	if (!on)
		return;
	SDL_Color col=get_color();
	SDL_SetRenderDrawColor(ren,0,0,0,0);
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren,col.r,col.g,col.b ,0);
}

SDL_Color Draw::get_color()
{
	SDL_Color col;
	SDL_GetRenderDrawColor(ren,&col.r,&col.g, &col.b,&col.a);
	return col;
}

void Draw::point(SDL_Point& cd)
{
	if (!on)
		return;
	SDL_Point temp=cd;
	conv(temp);
	SDL_RenderDrawPoint(ren,temp.x,temp.y);
}

void Draw::points(SDL_Point* points,int n)
{
	if (!on)
		return;
	SDL_Point* temp=new SDL_Point[n];
	for (int i=0;i<n;i++)
	{
		temp[i]=points[i];
		conv(temp[i]);
	}
	SDL_RenderDrawPoints(ren,temp,n);
	delete [] temp;
}

void Draw::line(SDL_Point& cd1,SDL_Point& cd2)
{
	if (!on)
		return;
	SDL_Point temp1=cd1,temp2=cd2;
	conv(temp1);
	conv(temp2);
	SDL_RenderDrawLine(ren,temp1.x,temp1.y, temp2.x,temp2.y);
}

void Draw::lines(SDL_Point* points,int n)
{
	if (!on)
		return;
	SDL_Point* temp=new SDL_Point[n];
	for (int i=0;i<n;i++)
	{
		temp[i]=points[i];
		conv(temp[i]);
	}
	SDL_RenderDrawLines(ren,temp,n);
	delete [] temp;
}

void Draw::rect(Draw::Rect_draw_mode mod,SDL_Rect& rect)
{
	if (!on)
		return;
	SDL_Rect temp=rect;
	conv(temp);
	if (mod==Fill)
	{
		SDL_RenderFillRect(ren,&temp);
	}
	else if (mod==Nofill)
	{
		SDL_RenderDrawRect(ren,&temp);
	}
}

void Draw::rects(Draw::Rect_draw_mode mod,SDL_Rect* rects,int n)
{
	if (!on)
		return;
	SDL_Rect* temp=new SDL_Rect[n];
	for (int i=0;i<n;i++)
	{
		temp[i]=rects[i];
		conv(temp[i]);
	}
	if (mod==Fill)
	{
		SDL_RenderFillRects(ren,temp,n);
	}
	else if (mod==Nofill)
	{
		SDL_RenderDrawRects(ren,temp,n);
	}
}

int Draw::add_word(char* ch,SDL_Color col,int size)
{
	if (size<=0)
	{
		return -1;
	}
	font_lib=TTF_OpenFont ("/system/fonts/DroidSansFallback.ttf",size);
	if (font_lib==NULL)
	{
		return -1;
	}
	SDL_Surface* temp=NULL;
	SDL_Texture* result=NULL;
	temp=TTF_RenderUTF8_Blended (font_lib,ch,col);
	result= SDL_CreateTextureFromSurface(ren,temp);
	words.push_back(result);
	SDL_FreeSurface(temp);
	TTF_CloseFont(font_lib);
	return words.size()-1;
}

void Draw::copy_word(int index,int x,int y)
{
	if (index<0 or index>words.size()-1)
	{
		return;
	}
	SDL_Rect rect{x,y,0,0};
	SDL_QueryTexture(words[index],NULL,NULL, &rect.w,&rect.h);
	SDL_RenderCopy(ren,words[index],NULL,&rect);
}

int Draw::get_word_height(int index)
{
	if (index<0 or index>words.size()-1)
	{
		return -1;
	}
	int h=0;
	SDL_QueryTexture (words[index],NULL,NULL,NULL,&h);
	return h;
}

int Draw::get_word_width(int index)
{
	if (index<0 or index>words.size()-1)
	{
		return -1;
	}
	int w=0;
	SDL_QueryTexture (words[index],NULL,NULL,&w,NULL);
	return w;
}