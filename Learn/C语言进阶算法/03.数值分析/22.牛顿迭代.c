/* 牛顿迭代 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 函数f(x)=1+x-x^3 */
double func(double x)
{
  return 1+x-x*x*x;
}
/* 导函数 */
double func1(double x)
{
  return 1-3*x*x;
}

/* 牛顿迭代 */
int Newton(double *x,double precision,int maxcyc)
{
  double x1,x0;
  int k;
  x0=*x;
  for(k=0;k<maxcyc;k++)
  {
    if(func1(x0)==0.0)
    {
      printf("迭代过程中导数为0!\n");
      return 0;
    }
    x1=x0-func(x0)/func1(x0);
    if(fabs(x1-x0)<precision || fabs(func(x1))<precision)
    {
      *x=x1;
      return 1;
    }
    else
      x0=x1;
  }
  printf("迭代次数超过预期！\n");
  return 0;
}
int main()
{
  double x = 2,precision = 1e-5;
  int maxcyc = 5;

  if(Newton(&x,precision,maxcyc)==1) //若函数返回值为1
    printf("该值附近的根为：%lf\n",x);
  else //若函数返回值为0
    printf("迭代失败！\n");
  return 0;
}