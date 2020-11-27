/*
例程181. 求解三角方程
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include<stdio.h>
#include <conio.h>
char xx[100][11];
int yy[10];
int ReadDat(void);
void WriteDat(void);
void CountRs(void)
{ 
	int i,j,k;
	for(i=0;i<100;i++)
	{ 
		k=0;
		for(j=0;j<10;j++)
			if(xx[i][j]=='1')
				k++;
		if(k==0||k==10) 
			continue;
		for(j=0;j<10;j++)
			if(xx[i][j]=='1')
				yy[j]++;
	}
}
int main()
{
	int i;
	for(i=0;i<10;i++) yy[i]=0;
	if(ReadDat())
	{
		printf("Can't open file IN183.DAT!\n\007" );
		return 0;
	}
	CountRs();
	WriteDat();
	return 0;
}
int ReadDat(void)
{
	FILE *fp;
	int i;
	char tt[13];
	clrscr();
	if((fp=fopen("in181.dat","r" ))==NULL) return 1;
	for(i=0;i<100;i++)
	{
		if(fgets(tt,13,fp)==NULL)return 1;
		memcpy(xx[i],tt,10);
		xx[i][10]=0;
	}
	fclose(fp);
	return 0;
}
void WriteDat(void)
{
	FILE *fp;
	int i;
	fp=fopen("out181.dat","w" );
	for(i=0;i<10;i++)
	{
		fprintf(fp," %d\n",yy[i]);
		printf(" NO.%2d notes=%d\n" ,i+1,yy[i]);
	}
	fclose(fp);
}