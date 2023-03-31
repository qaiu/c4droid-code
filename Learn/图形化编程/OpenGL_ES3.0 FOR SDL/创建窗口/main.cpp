#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengles.h"


int mIsAlive = 1;
// 程序从 main 函数开始
int main(int args,char*argv[])
{
	// SDL 初始化
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
		SDL_LogError(0,"%s",SDL_GetError());
		return-1;
	}
	
	//*** opengles 使用版本 3.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);

	
	//*** 窗口创建
	SDL_Window*win = SDL_CreateWindow("Hello World!",0,0,1920,1080,SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	if(win==NULL){
		SDL_LogError(0,"%s",SDL_GetError());
		return-1;
	}
	// *** opengles 创建上下文 
	SDL_GL_CreateContext(win);
	
	//*** 清除背景 
	glClearColor(0,1,0,1);
	
	mIsAlive = 1;
	while(mIsAlive)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type==SDL_QUIT){
				mIsAlive = 0;
				break;
			}
			
		}
		//*** 清除颜色缓冲 *** 深度缓冲
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		//*** opengles 更新窗口 
		SDL_GL_SwapWindow(win);
		SDL_Delay(20);
	}
	//*** 释放窗口
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}