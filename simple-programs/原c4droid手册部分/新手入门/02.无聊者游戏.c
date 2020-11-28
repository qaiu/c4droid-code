/*c4droid代码手册
 *无聊者游戏
 *TTHHR编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include"stdlib.h"
#include<unistd.h>
#include"stdio.h"
#include"time.h"
int main()
{
	int a, b, t, c, n = 10;
	char s;
	printf("开始:\n");
	while (1)//让程序循环执行
	{
		srand(time(0));
		a = rand() % 101;
		sleep(1);
		srand(time(0));
		b = rand() % 100;
		//对运算符进行随机
		if (a < 10)
			s = '*';
		else if (a >= 10 && a < 45)
			s = '/';
		else if (a >= 45 && a < 85)
			s = '-';
		else
			s = '+';
		printf("%d%c%d=", a, s, b);
		scanf("%d", &c);
		switch (s)
		{
		case '*':
			t = a * b;
			break;
		case '/':
			t = a / b;
			break;
		case '+':
			t = a + b;
			break;
		case '-':
			t = a - b;
			break;
		default:
			printf("error");
			exit(1);
		}
		if (c == t)
		{
			printf("\n正确!\n");
			n += 10;//n当做得分
		}
		else
		{
			printf("\n错误，正确答案:%d\n", t);
			sleep(2);
			n -= 10;
			if (n <= 0)//对得分进行判断，如果小于0就结束程序
				exit(0);
		}
		printf("目前得分:%d\n", n);
	}
	return 0;
}