﻿/*c4droid代码手册
 *关机
 *TTHHR编写
 *转载请说明出处
*/
#include<stdlib.h>
#include<stdio.h>
int main()
{
	printf("即将关机：\n");
	system("su -c 'reboot -p'");//关键一句，这句话是关机
	return 0;
}
