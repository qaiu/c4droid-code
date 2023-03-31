/*c4droid代码手册
*需要c4droid v4.98及以上
*暮日血穹 编写
*/
#include "./simulator.h"

/* 库仑定律-alpha粒子散射实验
 * 原理：库仑定律，动能定理
 * 
 * 注意：
 * 过低的fps会导致计算精度降低，进入constant.cpp文件
 * 减少Simulating_load的数值大小可以降低模拟的难度，
 * 提高计算精度
 */

int main()
{
	Simulator sim;
	return sim.run();
}