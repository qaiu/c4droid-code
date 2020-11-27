/*
例程106. 随机数
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/

#include <stdio.h>
#include <sys/timeb.h>
#include <conio.h>
#define Alpha 3.6

double initvalue()/*返回随机数序列初值*/
{
    double f0;
    struct timeb pr;
    while(1)
    {
		ftime(&pr);
		f0=pr.millitm*0.9876543*0.001;
		if(f0<0.001) continue;
		break;
    }
    return f0;
}
double random(void)/*返回一个(0,1)之间的随机数*/
{
	static double f=-1.0;
    double initvlaue();
    if(f==-1.0) f=initvalue();
    else f=Alpha*f*(1.0-f);
    return f;
}
int main()
{
	double test;
	int i;
	clrscr();
	puts("This is a random number generator.");
	puts("\n The random number are: ");
	for ( i = 0; i < 9; i++ )
	{
		test = random();
		printf ( " >> rand%d:%f\n", i, test );
	}
	puts("\n Press any key to quit...");
	getch();
	return 0;
}