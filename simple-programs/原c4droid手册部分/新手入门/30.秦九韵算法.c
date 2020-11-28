﻿/*c4droid代码手册
 *秦九韶算法
 *TTHHR编写
 *转载请说明出处
*/
#include<stdio.h>
main()
{
	printf("*******秦九韶算法*******\n");
	printf("输入项数:");
	int i,n;float x,a,s=0;
	scanf("%d",&n);
	printf("\n输入第%d项系数:",n);
	scanf("%f",&a);
	printf("\n输入x值:");
	scanf("%f",&x);
	s=x+a;
	i=n-1;
	for (i;i>0;)
	{
		printf("\n输入第%d项系数:",i);
		scanf("%f",&a);
		s=s*x+a;
		i--;
	}
	printf("\n答案:%.3f",s);
	printf("\n谢谢使用！made by:TTHHR");
	return 0;
}
	
