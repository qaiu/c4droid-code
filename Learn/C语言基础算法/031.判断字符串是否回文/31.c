/*
例程31. 判断字符串是否为回文字符串
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/21
*/
#include <stdio.h>
#include <conio.h>
#define MAX 50
int cycle(char *s)
{
	char *h,*t;

	for(h=s,t=s+strlen(s)-1;t>h;h++,t--)
		if(*h!=*t) break;
	return t<=h;
}

int main()
{
	char s[MAX];
	clrscr();
	while(1)
	{
		puts("Please input the string you want to judge (input # to quit):");
		scanf("%s",s);
		if(s[0]=='#')
			break;
		if(cycle(s))
			printf(" %s is a cycle string.\n",s);
		else
			printf(" %s is not a cycle string.\n",s);
	}
	puts("\nThank you for your using,bye bye!\n");
 return 0;
}