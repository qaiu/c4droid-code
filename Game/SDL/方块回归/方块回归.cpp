/*=======================
           游戏: 方块回归
           
  作者: 傻傻_痴痴   版本:  1.0
  
  联系邮箱: huangcjgame@gmail.com
  
 请不要恶意使用此代码或进行商业行为!
 
     此代码属于原创,归本人所有
     
     有问题请反馈到邮箱
     
 ———————C4droid吧————————
 ————————欢迎加入———————


>>>>>>>>>>游戏说明<<<<<<<<<<<<<

 一款小小的益智游戏，躲避各种障碍物
 游戏共有八关(实在很少，本人太懒了)
 
            1、上下左右键移动
            2、确定键选择确定
            3、游戏过程确定键保存
            4、ESC键返回或者退出
                       
>>>>>>>>>>主要实现功能<<<<<<<<<<<
                  
保存游戏功能、声音设置功能、关卡上锁解锁、 重置游戏数据等
 
       *****************************
       *                                              *
       *              游戏愉快  !             *
       *                                              *
       * ****************************
       
===========================*/


//包含的头文件
#include <MAP.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

//主菜单
int menu()
{
	da.dy = 120;  //箭头坐标方向
	q = 1;      //判断是否退出
	
	while (q == 1)  // q==1时,循环
	{
		if (set_1 == 0)
	//当声音设置为0,播放音乐
	
		{
			if (Mix_PlayingMusic() == 0)
				Mix_PlayMusic(menu_ogg, -1);
		}
		
		apply(0, 0, menu_b1, screen, NULL);   //应用菜单背景
		
		m_n = choose();  //应用选择返回值
		if (m_n == 1)
			new_game();  //新的游戏
		if (m_n == 2)
			con_game();  //继续游戏
		if (m_n == 3)
			choose_level();  //关卡选择
		if (m_n == 4)
			setting();     // 设置
		if (m_n == 5)
		{
			help();      //帮助
		}
		if (m_n == 6)
			clean();    //退出游戏
		SDL_Flip(screen);
		Fpsm();  //菜单限帧
	}
}

int choose()  //选择
{
	input_d();  //按键
	move_d();  //移动
	apply(250, da.dy, da.Dot, screen, NULL);  //应用箭头
	if (ch_m == 1)
	{
		if (da.dy >= 115 && da.dy < 155)
			return 1;
		if (da.dy >= 165 && da.dy < 210)
			return 2;
		if (da.dy >= 215 && da.dy < 260)
			return 3;
		if (da.dy >= 295 && da.dy < 340)
			return 4;
		if (da.dy >= 355 && da.dy < 400)
			return 5;
		if (da.dy >= 415 && da.dy < 460)
			return 6;
	}
	//返回选择的值
}

void new_game()  //新的游戏
{
	a.x = 5;
	a.y = 5;
	b.x = W - BOX_W - 5;
	b.y = H - BOX_W - 5;
	l = 1;
	Timer.loadTicks = 0;
	game();
}

void con_game()  //继续游戏
{
	input_data();
	game();
}

void help()   //帮助
{
	q_h = 1;
	while (q_h == 1)
	{
		apply(0, 0, menu_help, screen, NULL);
		SDL_Flip(screen);
		if (help_input() == 1)
		{
			q_h = 0;
			q_ch = 1;
		}
	}
	m_n = 0;
	ch_m = 0;
}

int choose_level()   //关卡选择
{
	ch_set = 2;
	da.dx = 150;
	da.dy = 60;
	q_level = 0;
	while (q_level == 0)
	{
		input_level();
		int ch_l = 0;
		int temp = lock_level;
		apply(0, 0, menu_b2, screen, NULL);
		apply(300, 100, num_1, screen, NULL);
		if (lock_level < 2)
			apply(300, 180, menu_lock, screen, NULL);
		if (lock_level < 3)
			apply(300, 260, menu_lock, screen, NULL);
		if (lock_level < 4)
			apply(300, 340, menu_lock, screen, NULL);
		if (lock_level < 5)
			apply(550, 100, menu_lock, screen, NULL);
		if (lock_level < 6)
			apply(550, 180, menu_lock, screen, NULL);
		if (lock_level < 7)
			apply(550, 260, menu_lock, screen, NULL);
		if (lock_level < 8)
			apply(550, 340, menu_lock, screen, NULL);
		if (lock_level > 1)
			apply(300, 180, num_2, screen, NULL);
		if (lock_level > 2)
			apply(300, 260, num_3, screen, NULL);
		if (lock_level > 3)
			apply(300, 340, num_4, screen, NULL);
		if (lock_level > 4)
			apply(550, 100, num_5, screen, NULL);
		if (lock_level > 5)
			apply(550, 180, num_6, screen, NULL);
		if (lock_level > 6)
			apply(550, 260, num_7, screen, NULL);
		if (lock_level > 7)
			apply(550, 340, num_8, screen, NULL);
		apply(da.dx, da.dy, da.Dot, screen, NULL);
		input_d();
		move_d();
		if (ch_level == 1)
		{
			if ((da.dx == 150) && (da.dy >= 50) && (da.dy < 130))
				ch_l = 1;
			if (da.dx == 150 && da.dy >= 130 && da.dy < 210 && lock_level > 1)
				ch_l = 2;
			if (da.dx == 150 && da.dy >= 210 && da.dy < 280 && lock_level > 2)
				ch_l = 3;
			if (da.dx == 150 && da.dy >= 280 && da.dy <= 350 && lock_level > 3)
				ch_l = 4;
			if (da.dx == 400 && da.dy >= 50 && da.dy < 130 && lock_level > 4)
				ch_l = 5;
			if (da.dx == 400 && da.dy >= 130 && da.dy < 210 && lock_level > 5)
				ch_l = 6;
			if (da.dx == 400 && da.dy >= 210 && da.dy < 280 && lock_level > 6)
				ch_l = 7;
			if (da.dx == 400 && da.dy >= 280 && da.dy <= 350 && lock_level > 7)
				ch_l = 8;
			l = ch_l;
			a.x = 5;
			a.y = 5;
			b.x = W - BOX_W - 5;
			b.y = H - BOX_W - 5;
			if (ch_l != 0)
			{
				if (set_1 == 0)
				{
					Mix_HaltMusic();
					Mix_PlayMusic(game_ogg, -1);
				}
				game();
			}
			if (set_1 == 0)
			{
				if (Mix_PlayingMusic() == 0)
					Mix_PlayMusic(menu_ogg, -1);
			}
			l = temp;
			ch_level = 0;
		}
		SDL_Flip(screen);
	}
}

void setting()  //设置
{
	da.dy = 220;   //箭头坐标位置
	ch_set = 1;
	q_set = 0;
	while (q_set == 0)
	{
		apply(0, 0, menu_b2, screen, NULL);
		apply(400, 150, m_5, screen, NULL);
		apply(480, 250, m_6, screen, NULL);
		apply(480, 300, m_7, screen, NULL);
		apply(430, 350, m_8, screen, NULL);
		input_d();
		move_d();
		apply(270, da.dy, da.Dot, screen, NULL);
		SDL_Flip(screen);
		Fpsm();
	}
}

void again()   //死亡重新游戏
{
	a.x = 5;
	a.y = 5;
	b.x = W - BOX_W - 5;
	b.y = H - BOX_W - 5;
	box_a = load_image("data/res/box_a.png");
	box_b = load_image("data/res/box_b.png");
	n = 0;
}

bool check(SDL_Rect a, SDL_Rect b)
//判断碰撞函数

{
	int l_a, r_a, top_a, bot_a;
	int l_b, r_b, top_b, bot_b;
	l_a = a.x;
	r_a = a.x + a.w;
	top_a = a.y;
	bot_a = a.y + a.h;
	l_b = b.x;
	r_b = b.x + b.w;
	top_b = b.y;
	bot_b = b.y + b.w;
	if (top_a >= bot_b)
	{
		return false;
	}
	if (bot_a <= top_b)
	{
		return false;
	}
	if (l_a >= r_b)
	{
		return false;
	}
	if (r_a <= l_b)
	{
		return false;
	}
	return true;
}

void over()  //游戏死亡
{
	if (life == 0)
	{
		if (set_1 == 0)
		{
			Mix_PlayChannel(-1, die_ogg, 0);
		}
		SDL_Delay(100);
		if (n == 0)
		{
			box_a = IMG_Load("data/res/b_1.png");
			box_b = IMG_Load("data/res/r_1.png");
		}
		if (n == 1)
		{
			box_a = IMG_Load("data/res/b_2.png");
			box_b = IMG_Load("data/res/r_2.png");
		}
		if (n == 2)
		{
			box_a = IMG_Load("data/res/b_3.png");
			box_b = IMG_Load("data/res/r_3.png");
		}
		if (n >= 3)
		{
			again();
			life = 1;
		}
	}
	n++;
}


void apply(int x, int y, SDL_Surface * a, SDL_Surface * b, SDL_Rect * c = NULL)
//应用图片函数
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(a, c, b, &offset);
}

void input_d()  //箭头获取按键信息
{
	while (SDL_PollEvent(&event) && q_ch == 1)
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				da.dv -= 5;
				break;
			case SDLK_DOWN:
				da.dv += 5;
				break;
			case SDLK_RETURN:
				if (ch_set == 0)
					ch_m = 1;
				if (ch_set == 1)
				{
					q_set = 1;
					ch_set = 0;
					if (da.dy > 200 && da.dy < 250)
					{
						set_1 = 0;
						g_voice.off_v = 0;
						save_voice(g_voice);
					}
					if (da.dy > 250 && da.dy < 300)
					{
						if (Mix_PlayingMusic() == 1)
							Mix_HaltMusic();
						set_1 = 1;
						g_voice.off_v = 1;
						save_voice(g_voice);
					}
					if (da.dy > 300 && da.dy < 350)
					{
						frame_s = 0;
						g_data.g_x1 = 5;
						g_data.g_y1 = 5;
						g_data.g_x2 = W - BOX_W - 5;
						g_data.g_y2 = H - BOX_W - 5;
						g_data.g_l = 1;
						g_data.g_time = 0;
						game_save(g_data);
						g_lock.ol = 1;
						l = 1;
						save_level(g_lock);
						set_1 = 0;
						g_voice.off_v = 0;
						save_voice(g_voice);
					}
					da.dy = 300;
				}
				if (ch_set == 2)
				{
					ch_level = 1;
				}
				break;
			case SDLK_ESCAPE:
				if (ch_set == 2)
				{
					ch_set = 0;
					da.dy = 250;
					q_level = 1;
				}
				if (ch_set == 1)
				{
					q_set = 1;
					ch_set = 0;
					da.dy = 300;
				}
				break;
			default:
				break;
			}
		}
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				da.dv += 5;
				break;
			case SDLK_DOWN:
				da.dv -= 5;
				break;
			case SDLK_RETURN:
				ch_m = 0;
				ch_level = 0;
				break;
			default:
				break;
			}
		}
	}
}

void input()   //方块获取按键信息
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				vx -= F;
				break;
			case SDLK_RIGHT:
				vx += F;
				break;
			case SDLK_UP:
				vy -= F;
				break;
			case SDLK_DOWN:
				vy += F;
				break;
			case SDLK_RETURN:
				ch_save = 1;
				break;
			case SDLK_ESCAPE:
				q_r = 1;
				break;
			default:
				break;
			}
		}
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				vx += F;
				break;
			case SDLK_RIGHT:
				vx -= F;
				break;
			case SDLK_UP:
				vy += F;
				break;
			case SDLK_DOWN:
				vy -= F;
				break;
			case SDLK_RETURN:
				ch_save = 0;
				break;
			case SDLK_ESCAPE:
				q_r = 0;
				break;
			default:
				break;
			}
		}
	}
}

int help_input()  //帮助获取按键信息
{
	q_ch = 0;
	while (SDL_PollEvent(&event_h))
	{
		if (event_h.type == SDL_KEYDOWN)
		{
			switch (event_h.key.keysym.sym)
			{
			case SDLK_UP:
				da.dv -= 5;
				return 1;
				break;
			case SDLK_DOWN:
				da.dv += 5;
				return 1;
				break;
			case SDLK_LEFT:
				return 1;
				break;
			case SDLK_RIGHT:
				return 1;
				break;
			case SDLK_RETURN:
				return 1;
				break;
			default:
				return 1;
				break;
			}
		}
		if (event_h.type == SDL_KEYUP)
		{
			switch (event_h.key.keysym.sym)
			{
			case SDLK_UP:
				da.dv += 5;
				break;
			case SDLK_DOWN:
				da.dv -= 5;
				break;
			case SDLK_RETURN:
				break;
			default:
				break;
			}
		}
	}
}

void move_d()   //移动按键
{
	da.dy += da.dv;
	if (ch_set == 0)
	{
		if (da.dy < 110)
			da.dy = 440;
		if (da.dy > 440)
			da.dy = 110;
	}
	if (ch_set == 1)
	{
		if (da.dy < 200)
			da.dy = 350;
		if (da.dy > 350)
			da.dy = 200;
	}
	if (ch_set == 2)
	{
		if (da.dx == 150 && da.dy < 50)
		{
			da.dy = 340;
			da.dx = 400;
		}
		if (da.dx == 150 && da.dy > 350)
		{
			da.dy = 60;
			da.dx = 400;
		}
		if (da.dx == 400 && da.dy < 50)
		{
			da.dy = 340;
			da.dx = 150;
		}
		if (da.dx == 400 && da.dy > 350)
		{
			da.dy = 60;
			da.dx = 150;
		}
	}
}

void move()  //方块移动
{
	a.x += vx;
	b.x -= vx;
	a.y += vy;
	b.y -= vy;
	if ((a.x < 0) || (a.x > (960 - BOX_W)))
	{
		a.x -= vx;
		b.x += vx;
	}
	if ((a.y < 0) || (a.y > (540 - BOX_W)))
	{
		a.y -= vy;
		b.y += vy;
	}
}

void box()  //初始化方块
{
	boxa.x = a.x;
	boxa.y = a.y;
	boxa.w = BOX_W;
	boxa.h = BOX_W;
	boxb.x = b.x;
	boxb.y = b.y;
	boxb.w = BOX_W;
	boxb.h = BOX_W;
}

void game_start()  //游戏开始
{
	if (life == 1)
	{
		move();
		input();
	}
	if (ch_save == 1)
	{
		q_save = 0;
		frame_s = 0;
		g_data.g_x1 = a.x;
		g_data.g_y1 = a.y;
		g_data.g_x2 = b.x;
		g_data.g_y2 = b.y;
		g_data.g_l = l;
		g_data.g_time = Timer.ticks;
		game_save(g_data);
	}
	box();
	Timer.startTicks = SDL_GetTicks();
	Timer.ticks = Timer.loadTicks + Timer.startTicks - Timer.pausedTicks;
	sprintf(t, "%d", Timer.ticks / 1000);
	sprintf(le, "%d", l);
	m_1 = TTF_RenderUTF8_Solid(font, t, color);
	m_3 = TTF_RenderUTF8_Solid(font, le, color);
	apply(0, 0, background, screen, NULL);
	apply(890, 470, blue[i], screen, NULL);
	apply(0, 0, red[i], screen, NULL);
	apply(a.x, a.y, box_a, screen, NULL);
	apply(b.x, b.y, box_b, screen, NULL);
	apply(800, 0, m_0, screen, NULL);
	apply(870, 0, m_1, screen, NULL);
	apply(800, 40, m_2, screen, NULL);
	apply(870, 40, m_3, screen, NULL);
	if (frame_s > 30)
		q_save = 1;
	if (q_save == 0)
	{
		apply(400, 100, m_4, screen, NULL);
		frame_s++;
	}
	if (frame % 7 == 0)
		i++;
	if (i > 4)
		i = 0;
	frame++;
	if (frame > 1000)
		frame = 1;
	if (a.x > 880 && a.y > 460)
		next = 1;
	if (next == 1)
		win();
}

void game()   //游戏
{
	Timer.pausedTicks = SDL_GetTicks();
	if (set_1 == 0)
	{
		Mix_HaltMusic();
		Mix_PlayChannel(-1, start_ogg, 0);
	}
	SDL_Delay(1000);
	if (set_1 == 0)
	{
		Mix_PlayMusic(game_ogg, -1);
	}
	ch_m = 0;
	if (l == 1)
		level_1();
	if (l == 2)
		level_2();
	if (l == 3)
		level_3();
	if (l == 4)
		level_4();
	if (l == 5)
		level_5();
	if (l == 6)
		level_6();
	if (l == 7)
		level_7();
	if (l == 8)
		level_8();
	if (set_1 == 0)
	{
		Mix_HaltMusic();
	}
}

void win()  //胜利
{
	if (set_1 == 0)
	{
		Mix_PlayChannel(-1, win_ogg, 0);
	}
	SDL_Delay(500);
	Timer.pausedTicks = Timer.startTicks;
	Timer.loadTicks = 0;
	if (l >= g_lock.ol && l < 8)
	{
		g_lock.ol =( l + 1);
		save_level(g_lock);
	}
	a.x = 5;
	a.y = 5;
	b.x = W - BOX_W - 5;
	b.y = H - BOX_W - 5;
	++l;
	if (l > 8)
	{
		l = 1;
	}
	next = 0;
	if (l == 1&&q_r==0)
		level_1();
	if (l == 2&&q_r==0)
		level_2();
	if (l == 3&&q_r==0)
		level_3();
	if (l == 4&&q_r==0)
		level_4();
	if (l == 5&&q_r==0)
		level_5();
	if (l == 6&&q_r==0)
		level_6();
	if (l == 7&&q_r==0)
		level_7();
	if (l == 8&&q_r==0)
		level_8();
}

int main(int argc, char *argv[])
{
	init_1();
	init_2();
	input_voice();
	menu();
	clean();
	return 0;
}