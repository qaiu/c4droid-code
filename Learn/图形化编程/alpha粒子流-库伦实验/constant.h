#ifndef _CONSTANT_H_
#define _CONSTANT_H_
/* 常数类 */
class Constant
{
private:
	Constant();
public:
	// 每像素代表的实际距离(米)
	static double Distance_per_pixel;
	// 每毫秒代表的实际时间(秒)
	static double Time_per_millisecond;
	// 场源粒子电荷
	static double Origin_charge;
	// 场源粒子质量
	static double Origin_mass;
	// 检验粒子电荷下界
	static double Test_charge_lower_bound;
	// 检验粒子电荷上界
	static double Test_charge_upper_bound;
	// 检验粒子质量下界
	static double Test_mass_lower_bound;
	// 检验粒子质量上界
	static double Test_mass_upper_bound;
	// 检验粒子初始速度下界
	static double Init_speed_lower_bound;
	// 检验粒子初始速度上界
	static double Init_speed_upper_bound;
	// 检验粒子初始位置x轴偏移量下界
	static double Init_x_offset_lower_bound;
	// 检验粒子初始位置x轴偏移量上界
	static double Init_x_offset_upper_bound;
	// 检验粒子初始位置y轴偏移量下界
	static double Init_y_offset_lower_bound;
	// 检验粒子初始位置y轴偏移量上界
	static double Init_y_offset_upper_bound;
	// 静电力恒量
	static double K;
	// 模拟负载(检验粒子数量)
	static int Simulating_load;
};

#endif