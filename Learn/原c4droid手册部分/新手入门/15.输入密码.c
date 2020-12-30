/*
*c4droid代码手册
[* 密码输入程序（C 版本） *]
[* @不一样的井猜 出品 *]
[* 保留所有权利 *]
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>

#define LEN 26
#define wait() puts("请按任意键继续……");\
        getch();\
        clrscr();

const char * password = "123456";

int cryptget(char *, size_t);

int main()
{
	char enter[LEN];
	int num;
	while (1)
	{
		puts("请输入密码：");
		num = cryptget(enter, LEN);
		printf("已输入 %d 个字符。\n", num);
		if (strcmp(enter, "123456") == 0)
		{
			puts("密码正确！");
			wait();
			break;
		}
		else
		{
			fputs("密码错误！请重试！\n", stderr);
			wait();
			continue;
		}
	}
	puts("欢迎跳出 while 循环！");
	return 0;
}

int cryptget(char * p, size_t s)
{
	int i;
	for (i = 0; i < s - 1; i++)
	{
		p[i] = getch();
		if (p[i] == 127)
		{
			(i > 0) ? i -= 2 : i--;
			printf("\b \b");
			continue;
		}
		else if (p[i] == '\n')
			break;
		else if (p[i] == ' ' || p[i] == '\t' || p[i] == '\v' || p[i] == '\r')
		{
			i--;
			continue;
		}
		else
			putchar('*');
	}
	while (p[i] != '\n')
		p[i] = getch();
	p[i] = '\0';
	putchar('\n');
	return i;
}