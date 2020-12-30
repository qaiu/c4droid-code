﻿/*c4droid代码手册
 *数列
作者未知
 *转载请说明出处
*/#include <stdio.h>
#define N 120
#define M 120
int main()
{
int a[M][N],i,j,n;
printf("输入列数:");
scanf("%d",&n);
for (i=0;i<n;i++)
{
for (j=0;j<=i;j++)
{
if (j==0||j==i)
{
a[i][j]=1;
}
else
{
 a[i][j]=a[i-1][j-1]+a[i-1][j];
}
}
}
for (i=0;i<n;i++)
{
for (j=0;j<n-i;j++)
{
printf(" ");
}
for (j=0;j<=i;j++)
{
printf("%3d ",a[i][j]);
}
printf("\n");
}
}