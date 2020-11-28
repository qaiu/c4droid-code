﻿/*c4droid代码手册
 *写入文件
 *TTHHR编写
 *转载请说明出处
*/
#include<stdio.h>
int main()
{
	FILE *fp;
	char ch[]="c4droid代码手册";
	printf("已执行");
	fp=fopen("/sdcard/c4droid.txt","w+");
	fputs(ch,fp);
	fclose(fp);
printf("\n退出");
return 0;
}