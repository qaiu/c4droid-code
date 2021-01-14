bool init(void){
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
		return false;
	}
	//初始化所有SDL系统
	
	screen=SDL_SetVideoMode(SW,SH,SBPP,SDL_SWSURFACE);
	if(screen==NULL){
		return false;
	}
	//设置屏幕属性
	
	if(TTF_Init()==-1){
		return false;
	}
	//初始化TTF
	
	if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1){
		return false;
	}
	//初始化声音
	
	SDL_WM_SetCaption("Flappy Bird",NULL);
	//设置窗口标题
	
	return true;
	//如果一切正常返回true
}
//初始化


SDL_Surface *load_image(char *filename){
	SDL_Surface *loadedimage=NULL;
	SDL_Surface *optimizedimage=NULL;
	
	loadedimage=IMG_Load(filename);
	if(loadedimage!=NULL){
		optimizedimage=SDL_DisplayFormat(loadedimage);
		SDL_FreeSurface(loadedimage);
	}
	
	if(optimizedimage!=NULL){
		Uint32 ColorKey=SDL_MapRGB(optimizedimage->format,0x0,0x0,0x0);
		SDL_SetColorKey(optimizedimage,SDL_SRCCOLORKEY,ColorKey);
	}
	//去掉黑边
	
    return optimizedimage;
    //返回优化后的图片
}


void load_files(void){
	Map[0].background=load_image("resources/image/background_night.png");
	Map[0].ground=load_image("resources/image/ground.png");
	Map[0].pipe_top=load_image("resources/image/pipe1_1.png");
	Map[0].pipe_bottom=load_image("resources/image/pipe1_2.png");
	Map[1].background=load_image("resources/image/background_day.png");
	Map[1].ground=load_image("resources/image/ground.png");
	Map[1].pipe_top=load_image("resources/image/pipe2_1.png");
	Map[1].pipe_bottom=load_image("resources/image/pipe2_2.png");
	 
	Bird[0].first=load_image("resources/image/bird1_1.png");
	Bird[0].second=load_image("resources/image/bird1_2.png");
	Bird[0].third=load_image("resources/image/bird1_3.png");
	Bird[1].first=load_image("resources/image/bird2_1.png");
	Bird[1].second=load_image("resources/image/bird2_2.png");
	Bird[1].third=load_image("resources/image/bird2_3.png");
	Bird[2].first=load_image("resources/image/bird3_1.png");
	Bird[2].second=load_image("resources/image/bird3_2.png");
	Bird[2].third=load_image("resources/image/bird3_3.png");

	Ui.caption=load_image("resources/image/caption.png");
	Ui.messagebox=load_image("resources/image/messagebox.png");
	Ui.button_start=load_image("resources/image/button_start.png");
	
	music=Mix_LoadMUS("resources/audio/music.mp3");
	Sound.point=Mix_LoadWAV("resources/audio/sfx_point.ogg");
	Sound.hit=Mix_LoadWAV("resources/audio/sfx_hit.ogg");
	Sound.wing=Mix_LoadWAV("resources/audio/sfx_wing.ogg");
	
	textfont=TTF_OpenFont("../../../Resources/Font/D2-coding.ttf",30);
}
//载入文件


void apply_surface(int x,int y,SDL_Surface *source,SDL_Surface *distination,SDL_Rect *clip=NULL){
	SDL_Rect offset;
	offset.x=x;
	offset.y=y;
	//偏移量
	
	SDL_BlitSurface(source,clip,distination,&offset);
}
//执行表面


void clean_up(void){
	for(int i=0;i<2;i++){
    	SDL_FreeSurface(Map[i].background);
    	SDL_FreeSurface(Map[i].ground);
    	SDL_FreeSurface(Map[i].pipe_top);
    	SDL_FreeSurface(Map[i].pipe_bottom);
	}
    for(int i=0;i<3;i++){
    	SDL_FreeSurface(Bird[i].first);
    	SDL_FreeSurface(Bird[i].second);
    	SDL_FreeSurface(Bird[i].third);
    }
	SDL_FreeSurface(Ui.caption);
	SDL_FreeSurface(Ui.messagebox);
	SDL_FreeSurface(Ui.button_start);
	SDL_FreeSurface(Ui.text);
	
	Mix_FreeMusic(music);
	Mix_FreeChunk(Sound.point);
	Mix_FreeChunk(Sound.hit);
	Mix_FreeChunk(Sound.wing);
	Mix_CloseAudio();
	
	TTF_CloseFont(textfont);
	
	TTF_Quit();
	SDL_Quit();
}
//释放内存


unsigned int rander(int a){
	srand(time(NULL));
	static unsigned short temp=rand()/35665;
	temp+=12345*a;
	long result=temp;
	
	while(result>=27){
		if(result==0){
			result+=rand()/65535;
		}
        result/=12;
	}
	
	if(result<=3){
		return rander(a+13);
	}
    else{
    	return result;
    }
}
//产生5-25之间的随机数


void map(unsigned short whatday,unsigned short background){
	static unsigned short x1=0,x2=0;
	if(background==1){
		x1+=3;
		//远景慢速移动
	}
    else{
    	x2+=5;
    	//近景快速移动
    }
	if(x1>=SW){
		x1=0;
	}
	if(x2>=SW){
		x2=0;
	}
	if(background==1){
	    apply_surface(0-x1,0,Map[whatday].background,screen);
	}
	else{
		apply_surface(0-x2,SH-181,Map[whatday].ground,screen);
	}
	if(background==1){
    	apply_surface(SW-x1,0,Map[whatday].background,screen);
	}
	else{
		apply_surface(SW-x2,SH-181,Map[whatday].ground,screen);
	}
}
//背景滚动


void pipe_move(unsigned short whatday){
	for(int i=0;i<3;i++){
    	if(move[i]>SW+60){
	    	move[i]=0;
		    length[i]=rander(rand())*10;
    	}
	}
	//重新为管道赋坐标与长度
	for(int i=0;i<3;i++){
    	apply_surface(SW-move[i],-length[i],Map[whatday].pipe_top,screen);
    	apply_surface(SW-move[i],SH/3*2+30-length[i],Map[whatday].pipe_bottom,screen);
    	move[i]+=5;
	}
	//绘制三个管道
}
//产生随机的管道障碍


short bird_control(){
	static int upordown=0,slower=0;
	slower--;
	if(go==0){
		upordown=0;
	}
	if(slower<0){
		slower=0;
	}
	if(SDL_PollEvent(&event)){
		if(event.type==SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_UP:{
					Mix_PlayChannel(-1,Sound.wing,0);
					go=1;
					upordown=-25;
					//向上
					slower=10;
					//上升状态持续时间
				}
				break;
			}
		}
	}
    else{
    	if(slower<=0&&go==1){
	    	upordown=25;
    	}
    	//如果没有按键事件产生且不在上升状态就开始下坠
    }
	if(slower>0){
		return upordown+=3;
		//变慢
	}
	else{
		return upordown;
	}
}
//控制小鸟


bool bird_fly(unsigned short whichone,short upordown){
	static short fly=0;
	height+=upordown;
	//上升或者下坠
    fly++;
	//动画控制
	
	switch(fly){
		case 1:
		apply_surface(SW/2-100,height,Bird[whichone].first,screen);
		break;
		case 2:
		apply_surface(SW/2-100,height,Bird[whichone].second,screen);
		break;
		case 3:
		apply_surface(SW/2-100,height,Bird[whichone].first,screen);
		break;
		case 4:
		apply_surface(SW/2-100,height,Bird[whichone].third,screen);
		break;
	}
		
	if(fly>=4){
		fly=0;
	}  
    if(height>=SH-200){
		return false;
		//坠地返回false
	}
	else{
		return true;
    }
}
//小鸟飞行并判断是否坠地


bool check_collition(unsigned int fps){
	for(int i=0;i<3;i++){
		if(SW-move[i]>=SW/2-180&&SW-move[i]<=SW/2-50){
			if(SW-move[i]==SW/2-100){
				//走完一个管道计分
		    	Mix_PlayChannel(-1,Sound.point,0);
		    	score++;
	    		}
			if(height<=480-length[i]||height>=SH/3*2-length[i]){
				Mix_PlayChannel(-1,Sound.hit,0);
				//判断是否在管道之间
				return false;
			}	
		}
	}
	return true;
}
//碰撞检测


void messagebox(void){
	static unsigned short keepscore;
	if(displaymessagebox==50){
		keepscore=score;
	}
	sprintf(scoredisplay,"YOU\'VE GOT:%d",keepscore);
	Ui.text=TTF_RenderText_Solid(textfont,scoredisplay,textcolor);
	
	if(displaymessagebox>0){
		apply_surface(SW/2-144,SH/2-200-displaymessagebox*10,Ui.messagebox,screen);
		apply_surface(SW/2-100,SH/2-110-displaymessagebox*10,Ui.text,screen);
	}
	
	if(displaymessagebox>-50){
    	displaymessagebox-=2;
	}
	
	if(displaymessagebox>-50&&displaymessagebox<=0){
		apply_surface(SW/2-144,SH/2-200,Ui.messagebox,screen);
		apply_surface(SW/2-100,SH/2-110,Ui.text,screen);
	}
}
//得分消息窗


void playing(void){
	map((whatday-8)/10,1);
    pipe_move((whatday-8)/10);
	map((whatday-8)/10,0);
	//绘制地图
	
	sprintf(scoredisplay,"SCORE:%d,TOTAL FRAMES:%d",score,fps/5);
	Ui.text=TTF_RenderText_Solid(textfont,scoredisplay,textcolor);
	apply_surface(0,0,Ui.text,screen);
	SDL_FreeSurface(Ui.text);
	//显示后释放Ui.text
	
	if(bird_fly((whichone)/12,bird_control())==false||check_collition(fps)==false){
	displaymessagebox=50;
	start=0;
	//如果碰撞或者坠地游戏结束
	}
}
//游戏中。。。
	

void welcome(void){
	map((whatday-8)/10,1);
	pipe_move((whatday-8)/10);
	map((whatday-8)/10,0);
	//绘制地图
	
	apply_surface(SW/2-105,SH/2-200,Ui.caption,screen);
	apply_surface(SW/2-150,SH-200,Ui.button_start,screen);
	//标题和开始按钮
	
	if(SDL_PollEvent(&event)){
		if(event.type==SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_UP:
				score=0;
				height=SH/2;
				move[0]=-200,move[1]=-400,move[2]=-600;
			    //开始游戏后将分数，飞行高度，管道位置重置
			    whatday=rander(rand());
				whichone=rander(rand());
				//更新地图
				
				start=1;
				go=0;
				//start=1调用playing()函数
				//go=0表示默认用户还未开始操作，这时小鸟保持平直飞行状态
				break;
				case SDLK_LEFT:
				clean_up();
				exit(1);
			}
		}
	}
}
//开始界面

	
	
	
	
	

	







	
	
	