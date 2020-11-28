﻿/*c4droid代码手册
 *简单的统计
 *TTHHR编写
 *转载请说明出处
*/
//写于13年9月27日
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
 main()
{
	printf("a:简单的统计\n");
	printf("b:变量间相关关系\n");
	char xz;
	scanf("%c",&xz);
	if (xz=='a')
	{	printf("**********************\n数据统计。求平均数，方差，标准差。\n");	printf("请输入数据的个数:\n");
	int n,i;
	scanf("%d",&n);
	float x=0,s=0,a[100];
	for (i=0; i<n;i++)
	{		printf("请输入第%d个数据:",i+1);
		scanf("%f",&a[i]);
		x=x+a[i];
	}
	int p=0;
	x=x/n;
	printf("平均值为:%.3f",x);
	for (p;p<n;p++)
		s=s+(a[p]-x)*(a[p]-x);
s=s/n;	printf("\n方差为:%.3f\n",s);
	s=sqrt(s);
	printf("标准差为:%.3f",s);	printf("\n\nTIT制作,谢谢使用！");
	}
	else if (xz=='b')
	{
		int n,i;
float s1=0,x=0,y=0,p[20],q[20];
float a,b,s2=0,s3=0,r;	printf("******变量相关关系分析******\n");
	printf("输入数据组数:");
	scanf("%d",&n);
	for (i=0;i<n;i++)
	{
		printf("\n请输入第%d组数据:",i+1);
		scanf("%f'%f",&p[i],&q[i]);
		s1=s1+p[i]*q[i];
		s2=s2+p[i]*p[i];
		s3=s3+q[i]*q[i];
		x=x+p[i];y=y+q[i];
	}
	x=x/n;y=y/n;
	s1=s1-x*y*n;
	s2=s2-n*x*x;
	s3=s3-n*y*y;
	b=s1/s2;
	a=y-b*x;
	printf("\n相关关系方程:y=%.3fx+%.3f",b,a);
	r=s2*s3;
	r=s1/sqrt(r);
	if (r<-0.75)
	printf("\n负相关关系很强");
	else if (r>0.75)
	printf("\n正相关关系很强");
	else if (-0.75<r&&r<-0.3||0.3<r&&r<0.75)
	printf("\n相关性一般");
	else if (-0.25<r&&r<0.25)
	printf("\相关性较弱");
	else printf("\nsomething error!");
	printf("\n\n谢谢使用！made by:TTHHR");
	}
	else
	{
		printf("error");
	}
return 0;
}