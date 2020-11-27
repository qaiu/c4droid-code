/* Extended Euclid 扩展欧几里德算法 */
#include <stdio.h>
#include <stdlib.h>

/* 如果GCD(a,b) = d,则存在x,y,使d=ax+by
 * extended_euclid(a,b)=ax+by
 * 即接受两个整数a,b，输出三个整数d,x,y。d,x,y满足等式ax+by=d，且d是a,b的最大公约数。
 * 常用在求解模线性方程及方程组中
 */
int extended_euclid(int a,int b,int *x,int *y)
{
  int d;
  if(b == 0) {
    *x = 1;
    *y = 0;
    return a;
  }
  d = extended_euclid(b,a%b,y,x);
  *y-=a/b*(*x);
  return d;
}

int main(void)
{
  int i,x = 0,y = 0;

  i = extended_euclid(4,6,&x,&y);
  printf("4*%d+6*%d = %d\n",x,y,i);

  return 0;
}