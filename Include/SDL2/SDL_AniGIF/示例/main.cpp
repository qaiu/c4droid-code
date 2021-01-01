#include "SDL2/SDL.h"
#include<cstdio>
#include<vector>
#include "../SDL_AniGIF.h"
#include "../SDL_AniGIF.c"
int main(int argc,char**argv)
{
	SDL_Event event;
	bool g_bRunning = true;
	int currentFrame = 0;
	SDL_Rect destRect={0,0,0,0};
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window*gWin = SDL_CreateWindow("gif test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_SHOWN);
	SDL_Renderer*gRen = SDL_CreateRenderer(gWin,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	//获取最大数量
	int number = AG_LoadGIF("1.gif",NULL,0);
	AG_Frame*frames = new AG_Frame[number];
	AG_LoadGIF("1.gif",frames,number);

	printf("the number of gif is:%d",number);
	//获取大小
	destRect.w = frames[0].surface->w;
	destRect.h = frames[0].surface->h;
	//由frames创建texture
	std::vector<SDL_Texture*> textures;
	for(int i=0;i<number;i++)
	{
		SDL_Texture*texture = SDL_CreateTextureFromSurface(gRen,frames[i].surface);
		textures.push_back(texture);
	}
	while(g_bRunning)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				g_bRunning = false;
				break;
			}
		}
		SDL_SetRenderDrawColor(gRen,255,200,100,255);
		SDL_RenderClear(gRen);
		SDL_RenderCopy(gRen,textures.at(currentFrame++%number),NULL,&destRect);
		SDL_RenderPresent(gRen);
		SDL_Delay(frames->delay);
	}
	delete[] frames;
	SDL_DestroyRenderer(gRen);
	SDL_DestroyWindow(gWin);
	return 0;
}