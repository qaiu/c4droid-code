/*      

*******************声明*********************

           Flappy Bird C+SDL移植v3.0
           作者:木影随风
           QQ:718475086
           完成日期:2014.6.5
           代码:C+SDL1.2
           若要转载源码，请注明作者出处
           禁止修改为自己名义发布源码     
         
****************终端配置说明*****************

           推荐关闭默认按钮
           
           配置按键6为UP，配置按键5为LEFT
           UP:控制小鸟飞行，开始/继续游戏
           LEFT:退出游戏(游戏中无效。。。)
           
           祝游戏愉快^_^
           
*******************************************

*/


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include "head.h"
#include "funcs.c"

#define FPS 30

unsigned long ticks=1000,nextticks=0,delay=1000/FPS;

int main(int argc,char *args[]){
	
	if(init()==false){
		return 0;
	}
	
	load_files();
	//初始化系统载入文件
	
	//Mix_PlayMusic(music,-1);
	//背景音乐

	while(1){
		
		ticks=SDL_GetTicks();
		
		if(start==0){
	        welcome();
		}
		if(start==1){
			playing();
		}
		messagebox();
    	SDL_Flip(screen);
		fps+=5;
		//便于和管道移动速度同步，用以计分
		
		nextticks=SDL_GetTicks();
		
		if((nextticks-ticks)<1000/FPS){
	    	SDL_Delay(1000/FPS-(nextticks-ticks));
		}
		//帧限制
		
		if(fps==100000000){
			fps=0;
		}
		
	}
	//主循环
}










