/*
例程175. 求取符合特定要求的素数
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/

#include <conio.h>
#include <stdio.h>
int isP(int m)
{ 
	int i ;
	for(i = 2 ; i < m ; i++)
	if(m % i == 0) return 0 ;
	return 1 ;
}
void num(int m,int k,int xx[])
{
	int i=0;
	for(m=m+1;k>0;m++)
	if(isP(m))
	{ 
		xx[i++]=m;
		k--; 
	}
}

int main()
{
	int m, n, xx[1000] ;
	clrscr() ;
	puts(" This program is to get k prime numbers which are larger than m.");
	printf(" >> Please input two integers to m and k : ") ;
	scanf("%d%d", &m, &n ) ;
	num(m, n, xx) ;
	printf(" >> The %d prime numbers which are larger than %d are:\n ",n,m);
	for(m = 0 ; m < n ; m++)
		printf(" %d ", xx[m]) ;
	printf("\n Press any key to quit...") ;
	getch();
	return 0;
}