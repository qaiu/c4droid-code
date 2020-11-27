/* Least Common Multiple 最小公倍数
 * lcm(a,b) = a*b/gcd(a,b)
 */
#include <stdio.h>
#include <stdlib.h>

/* 最大公约数 */
int gcd(int x,int y)
{
  int t;
  while(y > 0)
  {
    t = x % y;
    x = y;
    y = t;
  }
  return x;
}

/* 最小公倍数 */
int lcm(int x,int y)
{
  int t;
  
  t = x*y/gcd(x,y);
  return t;
}

int main(void)
{
  int a,b;
  scanf("%d %d",&a,&b);
  printf("%d与%d最小公倍数为：",a,b);
  printf("%d\n",lcm(a,b));
  return 0;
}