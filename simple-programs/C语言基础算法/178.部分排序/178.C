/*
例程178. 部分排序
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/

#include<stdio.h>
#include<string.h>
#include<conio.h>
int aa[200],bb[10];
void jsSort()
{
	int I,j,data;
	for(I=0;I<199;I++)
	for(j=I+1;j<200;j++)
	{if (aa[I]%1000>aa[j]%1000)
	{data=aa[I];aa[I]=aa[j];aa[j]=data;}
	else if(aa[I]%1000==aa[j]%1000)
	if(aa[I]<aa[j])
	{data=aa[I];aa[I]=aa[j];aa[j]=data;}
	}
	for(I=0;I<10;I++)
	bb[I]=aa[I];
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}

void readDat()
{
	FILE *in;
	int i;
	in=fopen("in178.dat","r");
	for(i=0; i<200; i++) fscanf(in,"%d",&aa[i]);
	fclose(in);
}
void writeDat()
{
	FILE *out;
	int i;
	clrscr();
	out=fopen("out178.dat","w");
	for(i=0; i<10; i++){
	printf(" No.%2d: %d\n",i+1,bb[i]);
	fprintf(out,"%d\n",bb[i]);
	}
	fclose(out);
}

int main()
{
	readDat();
	jsSort();
	writeDat();
	PressKeyToQuit();
	return 0;
}