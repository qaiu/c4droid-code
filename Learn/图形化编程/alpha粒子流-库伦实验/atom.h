#ifndef _ATOM_H_
#define _ATOM_H_
#include "./vector_2d.h"
#include "./constant.h"
#include "SDL2/SDL.h"
/* 粒子类 */
class Atom
{
private:
// 粒子的全局属性
	static Uint32 oldtime;// 上一次相互作用的时刻
	static double interval;// 相互作用的实际时间
public:
	static Atom source;// 场源粒子
public:
	static void start()
	{
		/* 根据两次相互作用的时间间隔计算这一次相互作用的
		 * 实际时间 */
		interval=(SDL_GetTicks()-oldtime)* Constant::Time_per_millisecond;
		// 更新这一次相互作用的时刻
		oldtime=SDL_GetTicks();
	}
private:
// 粒子的真实属性
	Vector_2D position;// 位置
	Vector_2D velocity;// 速度
	double mass;// 质量
	double charge;// 电荷
private:
// 粒子的虚拟属性
	Vector_2D d_position;// 位置变化量
	Vector_2D d_velocity;// 速度变化量
public:
	Atom():mass(0.0),charge(0.0){}
	void set(Vector_2D& p,Vector_2D& v,double m,double c)
	{
		position=p;
		velocity=v;
		mass=m;
		charge=c;
	}
	void move()
	{
		/* 根据当前速度计算在本次相互作用的时间内粒子应该
		 * 移动的位移 */
		d_position=velocity*interval;
	}
	void interact();// 同上，但是计算的是速度变化量
	void practice()
	{
		position+=d_position;
		velocity+=d_velocity;
	}
	void copy();// 打印
	// 判断粒子是否超出显示范围
	bool out_of_horizon();
};

#endif