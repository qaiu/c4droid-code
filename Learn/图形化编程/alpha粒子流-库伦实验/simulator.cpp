#include "./simulator.h"
#include "./atom.h"
#include "./constant.h"
#include "./vector_2d.h"
#include <cmath>
#include <ctime>
#include "SDL2/SDL.h"

Simulator::Simulator():fps_index(0), dre(std::clock()),
urd_charge(Constant::Test_charge_lower_bound, Constant::Test_charge_upper_bound),
urd_mass(Constant::Test_mass_lower_bound, Constant::Test_mass_upper_bound),
urd_speed(Constant::Init_speed_lower_bound, Constant::Init_speed_upper_bound),
urd_x_offset(Constant::Init_x_offset_lower_bound,Constant::Init_x_offset_upper_bound),
urd_y_offset(Constant::Init_y_offset_lower_bound,Constant::Init_y_offset_upper_bound),
atoms(Constant::Simulating_load)
{
	for (int i=0;i<10;i++)
	{
		fps_number_index[i]=0;
	}
}

void Simulator::init()
{
	Draw::init();
	// 初始化画笔颜色
	SDL_Color color{255,255,255,0};
	Draw::set_color(color);
	// 初始化fps字符串
	char ch_fps_number[]="0";
	SDL_Color fps_color{255,255,255,0};
	for (int i=0;i<10;i++)
	{
		fps_number_index[i]= Draw::add_word(ch_fps_number,fps_color,18);
		ch_fps_number[0]++;
	}
	char fps[]="fps:";
	fps_index=Draw::add_word(fps,fps_color,18);
	// 初始化场源粒子
	static Vector_2D centre
	{
		(Draw::get_w()/2)* Constant::Distance_per_pixel,
		(Draw::get_h()/2)* Constant::Distance_per_pixel
	};
	static Vector_2D zero_vel{0.0,0.0};
	Atom::source.set(centre,zero_vel, Constant::Origin_mass, Constant::Origin_charge);
	// 初始化检验粒子
	for (int i=0;i<atoms.size();i++)
	{
		init_atom(atoms[i]);
	}
}

void Simulator::quit()
{
	Draw::quit();
	atoms.clear();
}

void Simulator::init_atom(Atom& obj)
{
	using std::cos;
	using std::sin;
	// 原点
	static Vector_2D origin
	{
		0.0,
		Draw::get_h()*Constant::Distance_per_pixel
	};
	static Vector_2D init_pos;// 真实初位置
	static Vector_2D init_vel// 初速度方向
	{
		Draw::get_w()* Constant::Distance_per_pixel,
		-Draw::get_h()* Constant::Distance_per_pixel
	};
	static double x_offset=0.0;// x轴偏移量
	static double y_offset=0.0;// y轴偏移量
	static double mass=0.0;// 质量
	static double charge=0.0;// 电荷
	static double speed=0.0;// 速度大小
	// 计算初始位置
	mass=urd_mass(dre);
	charge=urd_charge(dre);
	speed=urd_speed(dre);
	x_offset=urd_x_offset(dre);
	y_offset=urd_y_offset(dre);
	/* 注：关于x_offset和y_offset
	 * 粒子在屏幕左下角的一小块区域生成
	 * 由于现实中不可能所有的粒子都在同一个位置出发，因此
	 * 我们需要一个随机化的偏移量来使模拟更加接近事实
	 */
	init_pos.x=x_offset+origin.x;
	init_pos.y=y_offset+origin.y;
	// 获取真实速度
	init_vel=init_vel.unit_vector()*speed;
	obj.set(init_pos,init_vel,mass,charge);
}

void Simulator::calc_fps()
{
	// 上一次调用calc_fps()的时刻
	static Uint32 oldtime=0;
	// frame计数
	static Uint32 frames=0;
	// fps
	static Uint32 fps=0;
	if (oldtime+250>SDL_GetTicks())
	{
		frames++;
		draw_fps(fps);
	}
	else
	{
		fps=frames*4;
		frames=0;
		draw_fps(fps);
		oldtime=SDL_GetTicks();
	}
}

void Simulator::draw_fps(Uint32 fps)
{
	int digits=1,x=0,y=0,ww=0,wh=0;
	int temp=fps;
	while (temp/=10)
	{
		digits++;
	}
	ww= Draw::get_word_width(fps_number_index[0]);
	wh= Draw::get_word_height(fps_number_index[0]);
	x=Draw::get_w()-ww;
	y=Draw::get_h()-wh;
	for (int i=0;i<digits;i++)
	{
		temp=fps%10;
		x-=ww;
		Draw::copy_word (fps_number_index[temp],x,y);
		fps/=10;
	}
	x-=Draw::get_word_width(fps_index);
	Draw::copy_word(fps_index,x,y);
}

int Simulator::run()
{
	init();
	bool running=true;
	SDL_Event event;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type==SDL_FINGERUP)
			{
				running=false;
			}
		}
		Atom::start();// 相互作用开始
		for (int i=0;i<atoms.size();i++)
		{
			atoms[i].move();
			atoms[i].interact();
		}
		for (int i=0;i<atoms.size();i++)
		{
			atoms[i].practice();
			if (!atoms[i].out_of_horizon())
			{
				atoms[i].copy();
			}
			else
			{
				// 重新设置屏幕外的粒子
				init_atom(atoms[i]);
			}
		}
		calc_fps();
		Draw::show();
		Draw::clear();
	}
	quit();
	return 0;
}