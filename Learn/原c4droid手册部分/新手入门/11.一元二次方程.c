/*c4droid代码手册
 *一元二次方程
 *TTHHR编写
 *转载请说明出处
*/
#include<stdio.h>
#include<math.h>
int main()
{
	printf("请输入a,b,c值:\n");/*提示输入*/
	float a, b, c, d, x1, x2;/*定义变量*/
	scanf("%f %f %f", &a, &b, &c);/*因为3个%f之间有空格，所以输入时，a.b.c值之间要用空格隔开*/
	if (a != 0)
	{
		d = b * b - 4 * a * c;/*对德尔它进行计算*/
		if (d > 0)
		{
			x1 = (-b + sqrt(d)) / (2 * a);
			x2 = (-b - sqrt(d)) /( 2 * a);
			printf("方程根为:x1=%.3f\nx2=%.3f", x1, x2);
		}
		else if (d == 0)
		{
			x1 = -b / (2 * a);
			printf("方程根为:x1=x2=%.3f", x1);
		}
		else
		{
			printf("无实根!");
		}
	}
	else if (a == 0)
	{
		printf("非一元二次方程!");
	}
return 0;
}
