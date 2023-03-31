#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include "./atom.h"
#include "./constant.h"
#include "./draw.h"
#include "SDL2/SDL.h"
#include <random>
#include <vector>
/* 模拟器类 */
class Simulator
{
private:
// 随机数发生器组
	std::default_random_engine dre;
	std::uniform_real_distribution<double> urd_charge;
	std::uniform_real_distribution<double> urd_mass;
	std::uniform_real_distribution<double> urd_speed;
	std::uniform_real_distribution<double> urd_x_offset;
	std::uniform_real_distribution<double> urd_y_offset;
private:
// fps
	int fps_index;
	int fps_number_index[10];
private:
// 检验粒子
	std::vector<Atom> atoms;
private:
	void init();
	void quit();
	// 初始化检验粒子
	void init_atom(Atom& obj);
	// 计算和显示fps
	void calc_fps();
	void draw_fps(Uint32 fps);
public:
	Simulator();
	int run();
};

#endif