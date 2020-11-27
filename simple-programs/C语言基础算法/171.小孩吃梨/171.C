/*
例程171. 小孩吃梨问题
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/

#include <stdio.h> /*小孩吃梨问题*/
#include <conio.h>
int main()
{
	long x,y; /* 定义两个变量*/
	x=1;
	y=18;
	clrscr();
	puts("       This program is to solve");
	puts(" The Problem of Child's Eating Pears.");
	while(y>0)
	{
		x=2*(x+1);
		y--;
	}
	printf(" >> In the first day, the child bought %ld pears.\n",x);
	printf(" Press any key to quit...");
	getch();
	return 0;
}