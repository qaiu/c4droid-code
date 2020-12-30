/* 设m1，m2，…，mi为两两互质的正整数，a1，a2，…，ak为任意整数，则同余方程组
 * x≡a1(mod m1)；
 * x≡a2(mod m2)；
 * ……
 * x≡ai(mod mi)；
 * 总有整数解
 */
#include <stdio.h>
#include <stdlib.h>

/* 扩展欧几里德算法 */
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

/* 中国余数定理 */
int chinese_remainder(int b[],int w[],int len)
{
  int i,d,x,y,m,n,ret;
  ret = 0;
  n = 1;
  for(i=0;i<len;i++)
    n*=w[i];
  for(i=0;i<len;i++)
  {
    m = n/w[i];
    d = extended_euclid(w[i],m,&x,&y);
    ret = (ret+y*m*b[i])%n;
  }
  return (n+ret%n)%n;
}

/* 求正整数N，使N除以3余2，除以5余3，除以7余2 */
int main(void)
{
  int b[]={2,3,2},w[]={3,5,7},N;
  N = chinese_remainder(b,w,3);
  printf("正整数N：%d",N);
  return 0;
}