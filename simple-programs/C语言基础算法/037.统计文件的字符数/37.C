/*
例程37. 统计文件字符个数
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/21
*/

#include <conio.h>
#include <stdio.h>
int main()
{
	char fname[80];/*存贮文件名*/
	FILE *rfp;
	long count;/*文件字符计数器*/

	clrscr();
	printf("Please input the file's name:\n");
	scanf("%s",fname);
	if((rfp=fopen(fname,"r"))==NULL)
	{
		printf("Can't open file %s.\n",fname);
		return 0;
	}
	count=0;
	while(fgetc(rfp)!=EOF)
		count++;
	fclose(rfp);/*关闭文件*/
	printf("There are %ld characters in file %s.\n",count,fname);
	puts("\n Press any key to quit...");
	getch();
	return 0;
}