/*
例程193. 矩阵就地逆置
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <stdio.h>

void fun(int array[][3]) 
{
	int i,j,t; 
	for(i=0; i < 3; i++) 
	{
		for(j=0; j < i; j++) 
		{
			t=array[i][j]; 
    		array[i][j]=array[j][i]; 
    		array[j][i]=t;
		} 
	}		
} 
int main() 
{ 
	int i,j; 
	int array[3][3]={
	{100,200,300}, 
	{400,500,600}, 
	{700,800,900}}; 
	
	for (i=0; i < 3; i++) 
	{
		for (j=0; j < 3; j++) 
			printf("%7d",array[i][j]); 
		printf("\n"); 
	} 
	fun(array); 
	printf("Converted array:\n"); 
	for (i=0; i < 3; i++) 
	{
		for (j=0; j < 3; j++) 
		printf("%7d",array[i][j]); 
		printf("\n"); 
	} 
}