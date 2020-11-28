﻿/*c4droid代码手册
 *产生随机数
 *TTHHR编写
 *转载请说明出处
*/
#include"conio.h"/*sleep()所属头文件*/
#include"stdio.h"/*printf()所属头文件*/
#include"stdlib.h"/*srand()所属头文件*/
#include"time.h"/*time()所属头文件*/
int main()
{
	int t;
		printf("开始");
	while(1)
{
	srand(time(0));//设置时间种子
	t=rand()%100;//产生0-99的随机数
	printf("\n%d",t);
	sleep(1);//暂停一秒
}
return 0;
}
