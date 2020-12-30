﻿﻿/*c4droid代码手册
 *进recovery
 *TTHHR编写
 *转载请说明出处
*/
#include<stdlib.h>
#include<stdio.h>
int main()
{
	printf("即将重启：\n");
	system("su -c 'reboot recovery'");//关键一句，这句话是进rec
	return 0;
}
