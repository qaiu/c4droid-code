/*
例程191. 比较字符串大小
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <stdio.h>

char *fun ( char *s, char *t) 
{
	char *ss=s, *tt=t; 
	while((*ss)&&(*tt)) 
	{
		ss++;
		tt++;
	} 
	if (*tt) 
	return t; 
	else
	return s; 
} 

int main( ) 
{
	char a[20],b[10],*p,*q; 
    int i; 
    printf("Input 1th string:") ; 
    gets( a); 
    printf("Input 2th string:") ; 
    gets( b); 
    printf("%s\n",fun (a, b )); 
    return 0;
}