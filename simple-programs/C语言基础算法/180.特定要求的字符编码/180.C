/*
例程180. 特定要求的字符编码
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/

#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>
char xx[1000][80];
int maxline=0;/*文章的总行数*/
int ReadDat(void);
void WriteDat(void);
void encryptChar()
{
	int I;
	char *pf;
	for(I=0;I<maxline;I++)
	{pf=xx[I];
	while(*pf!=0)
	{if(*pf*11%256>130||*pf*11%256<=32);
	else
	*pf=*pf*11%256;
	pf++; }
	}
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
int main()
{
	clrscr();
	if(ReadDat())
	{
		printf(" Can't open file IN180.DAT!\n");
		return 0;
	}
	encryptChar();
	WriteDat();
	PressKeyToQuit();
	return 0;
}
int ReadDat(void)
{
	FILE *fp;
	int i=0;
	char *p;
	if((fp=fopen("in180.dat","r"))==NULL) return 1;
	while(fgets(xx[i],80,fp)!=NULL)
	{
		p=strchr(xx[i],'\n');
		if(p)*p=0;
		i++;
	}
	maxline=i;
	fclose(fp);
	return 0;
}
void WriteDat(void)
{
	FILE *fp;
	int i;
	fp=fopen("out180.dat","w");
	for(i=0;i<maxline;i++)
	{
		printf("%s\n",xx[i]);
		fprintf(fp,"%s\n",xx[i]);
	}
	fclose(fp);
}