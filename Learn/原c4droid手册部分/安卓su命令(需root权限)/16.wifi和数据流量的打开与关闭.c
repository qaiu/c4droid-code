/*c4droid代码手册
 *wifi和数据流量的打开与关闭
 *TTHHR编写
 *转载请说明出处
*/
#include<stdio.h>
#include<stdlib.h>
int main()
{
	t:printf("a:打开wifi,关闭数据流量\n");
	printf("b:关闭wifi,打开数据流量\n");
	printf("c:关闭数据流量\n");
	printf("please input:\n");
	char ch;
	scanf("%c",&ch);
	printf("please wait.....\n");
	switch (ch)
	{
		case 'a':system("su -c 'svc wifi enable'");
		system("su -c 'svc data disable'");break;
		case 'b':system("su -c 'svc wifi disable'");
		system("su -c 'svc data enable'");break;
		case 'c':
		system("su -c 'svc data disable'");break;
		default:printf("error\n");goto t;
	}
	printf("success!\n");
	return 0;
}
	