/*//////////////////////////////////////////////////////////////*/
/*                          汉诺塔问题                          */
/*//////////////////////////////////////////////////////////////*/
/*
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/22
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
/* hanoil 子程序，实现将n个盘子从a移动到c */
void hanoil(int n,char a,char b, char c)
{
	if(n==1) /* 递归调用的出口，n=1 */
    	printf("  >>  Move Plate No.%d from Stick %c to Stick %c.\n",n,a,c);
	else
    {
    	hanoil(n-1,a,c,b);  /* 递归调用 */
		printf("  >>  Move Plate No.%d from Stick %c to Stick %c.\n",n,a,c);
		hanoil(n-1,b,a,c);
	}
}

/****************************** 主程序******************************/

int main()
{
   	int n;
   	char a='A';
   	char b='B';
   	char c='C';
   	clrscr();
   	printf("This is a hanoil program.\nPlease input number of the plates:\n");
   	scanf("%d",&n);
   	if(n<=0)
   	{
   		puts("n must no less than 1!");
   		exit(1);
   	}
   	puts("The steps of moving plates are:");
   	hanoil(n,a,b,c);
   	puts("\n Press any key to quit...");
   	getch();
   	return 0;
}