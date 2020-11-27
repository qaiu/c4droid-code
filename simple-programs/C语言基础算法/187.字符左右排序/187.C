/*
例程187. 字符左右排序
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include<stdio.h>
#include<string.h>
#include<conio.h>
char xx[20][80];
void jsSort()
{
	int i,strl,half,j,k;
	char ch;
	for(i=0;i<20;i++) /*行循环*/
	{
		strl=strlen(xx[i]); /*每行长度*/ 
		half=strl/2;
		for(j=0;j<half-1;j++) /*每行的第j个位置*/
			for(k=j+1;k<half;k++) 
				if(xx[i][j]>xx[i][k])
				{
					ch=xx[i][j]; /*每次将最小数赋给xx[i][j]*/ 
					xx[i][j]=xx[i][k]; 
					xx[i][k]=ch;
				}
		for(j=half-1,k=strl-1;j>=0;j--,k--)
		{
			ch=xx[i][j];
			xx[i][j]=xx[i][k];
			xx[i][k]=ch;
		}
	}
}


void readDat()
{
	FILE *in;
	int i=0;
	char *p;
	in=fopen("in187.dat","r");
	while(i<20&&fgets(xx[i],80,in)!=NULL)
	{
		p=strchr(xx[i],'\n');
		if(p)
			*p=0;
		i++;
	}
	fclose(in);
}
void writeDat()
{
	FILE *out;
	int i;
	clrscr();
	out=fopen("out187.dat","w");
	for(i=0;i<20;i++)
	{
		printf("%s\n",xx[i]);
		fprintf(out,"%s\n",xx[i]);
	}
	fclose(out);
}

void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}

int main()
{
	readDat();
	jsSort();
	writeDat();
	PressKeyToQuit();
	return 0;
}