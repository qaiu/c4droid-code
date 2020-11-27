/*
例程9. 乘法口诀表
by:千百度QAIU 
QQ:736226400
编译环境:gcc/tcc
2017/10/20
*/
#include <stdio.h>
int main()
{
	for (int i=1;i<=9;i++)
	{
		for (int j=1;j<=i;j++)
			printf("%dx%d=%-2d ",j,i,i*j);
		printf("\n");
	}
	return 0;
}