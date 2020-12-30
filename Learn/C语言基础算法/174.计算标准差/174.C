/*
例程174. 计算标准差
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/

#include <stdio.h>
#include <math.h>
#include <conio.h>
double fun(double x[10])
{
	int i;
	double fc,avg=0.0,sum=0.0,abs=0.0;
	for (i=0;i<10;i++)
		sum+=x[i];
	avg=sum/10;
	for (i=0;i<10;i++)
		abs+=(x[i]-avg)*(x[i]-avg);
	fc=sqrt(abs/10) ;
	return fc;
}
int main()
{
	double s, x[10]={95.0,89.0,76.0,65.0,88.0,72.0,85.0,81.0,90.0,56.0};
	int i;
	clrscr();
	printf(" The original data is:\n");
	for(i=0;i<10;i++)
		printf(" %3.1f",x[i]);
	printf("\n The variance of the data is %lf.\n",fun(x));
	printf(" Press any key to quit...");
	getch();
	return 0;
}