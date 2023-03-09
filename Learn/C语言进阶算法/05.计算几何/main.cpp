/*
计算几何简单示例
by 千百度QAIU
QQ736226400
建议将头文件编译成静态.a或者动态.so链接库，然后直接引用头文件即可
具体方法自行百度

函数说明在GEOMETRY.hpp
函数实现在GEOMETRY.Cpp
结合定义输入合法参数

c4droid 长按编译，在编译选项选择编译多个源文件:geometry.cpp main.cpp
快写代码直接创建命令行工程，将.cpp和.hpp复制进去
*/

#include <stdio.h>

#include "geometry.h"
int main()
{
/*测试代码*/
	int n = 6;
	POINT maxcic;
	double radius;
	POINT point[] = { {1, 1}, {1, 5}, {5, 1}, {5, 5}, {4, 3}, {2, 4} };
	/*get包含点集的最小圆圈*/
	getMinAreaCir(point, n, maxcic, radius);
	printf("%lf %lf %lf\n", maxcic.x, maxcic.y, radius);
	/*计算球面距离,球半径为9.0后面是起点和终点经纬度*/
	printf("%.9lf\n",sphere_dist(9,0,90,0,-90));
	/*计算球面欧氏几何直线距离,参数同上*/
	printf("%f\n",line_dist(9,0,-90,0,90));
	/*判断圆是否在矩形内*/
	printf("%d\n",point_in_circle({5,5}, 5, {0.9,4.9999}));
	// 两圆关系
	printf("%d\n",CircleRelation({0,5},5, {0,7}, 3));
	return 0;
}