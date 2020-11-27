/*
例程5. 字符的输出
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/20
*/
#include <stdio.h>
int main()
{
	char ch,nch;
	printf("Please input a string with a # in the end.\n");
	scanf("%c",&ch);	/* 输入字符*/
	while(ch != '#')	/*终止条件 */
	{
		printf("%c",ch);	/* 循环打印*/
		printf(" ");
		scanf("%c",&ch);//从缓冲区读取字符
	}
	printf("#\n");
	return 0;
}