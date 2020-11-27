/*
例程20. 求数组前n元素之和.c
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
*/
#include <conio.h>

int sum(int a[],int n)
{
	if(n<=0)
		return 0;
	return a[n-1]+sum(a,n-1);
}
int a[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int main()
{
	int i;
	clrscr();
	printf("\n The arry is:\n");
	for(i=0;i<sizeof(a)/sizeof(a[0]);i++)
		printf(" %d ",a[i]);
	printf("\n SUM of the arry is: %d\n",sum(a,sizeof(a)/sizeof(a[0])));
	printf("\n Press any key to quit...\n");
	getch();
}