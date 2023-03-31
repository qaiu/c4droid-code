#include "./atom.h"
#include "./constant.h"
#include "./draw.h"
#include "SDL2/SDL.h"

Uint32 Atom::oldtime=0;
double Atom::interval=0.0;
Atom Atom::source;

void Atom::interact()
{
	static Vector_2D ST;
	static double r=0.0;// 距离
	static double force_1D=0.0;
	// 真正的合力
	static Vector_2D force;
	// 向量DT从场源粒子指向检验粒子
	ST=position-source.position;
	// 取模
	r=ST.mod();
	// 计算ST方向上的合力
	force_1D= (Constant::K*charge*source.charge)/(r*r);
	force=ST.unit_vector()*force_1D;
	// 正交分解，动能定理
	d_velocity=force*(interval/mass);
}

void Atom::copy()
{
	SDL_Rect screen_pos{0,0,2,2};
	screen_pos.x= position.x/Constant::Distance_per_pixel-1;
	screen_pos.y= position.y/Constant::Distance_per_pixel-1;
	Draw::rect(Draw::Fill,screen_pos);
}

bool Atom::out_of_horizon()
{
	return (position.x<0.0 or position.y<0.0 or position.x>Draw::get_w()*Constant::Distance_per_pixel or position.y>Draw::get_h()*Constant::Distance_per_pixel);
}