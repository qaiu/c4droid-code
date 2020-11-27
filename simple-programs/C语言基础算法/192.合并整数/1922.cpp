/*
 合并两个整数2
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <stdio.h>

long long mergeinteger1(int a, int b)
{
	long long sum = 0;
	int mid = b;
	sum += a * 10;
	for (;mid/=10;sum*=10);
	sum += b;
	return sum;
}

long long mergeinteger2(int a, int b)
{
	char A[100] = {0};
	long long res = 0;
	sprintf(A,"%d%d",a,b);
	sscanf(A,"%lld",&res);
	return res;
}


int main(void)
{
	int a, b;
	scanf("%d %d", &a, &b);
	printf("%d%d\n", a, b);
	printf("merge:%lld\n", mergeinteger1(a, b));
	printf("merge:%lld\n", mergeinteger2(a, b));
	return 0;
}