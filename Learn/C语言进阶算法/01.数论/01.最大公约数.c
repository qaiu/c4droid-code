/* Greatest Common Divisor 最大公约数
 * 几个整数，公有的约数，叫做这几个数的公约数；
 * 其中最大的一个，叫做这几个数的最大公约数。
 * 例如：12、16的公约数有±1、±2、±4，其中最大的一个是4，
 * 4是12与16的最大公约数，一般记为（12、16）=4。
 */
#include <stdio.h>
#include <stdlib.h>

/* 欧几里德算法（辗转相除法）求最大公约数
 * 原理： 已知a,b,c为正整数，若a除以b余c，则（a,b）=(b,c)
 *    即gcd(a,b)=gcd(b,a mod b)，当b为0时，两数的最大公约数即为a
 */
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

int main(void)
{
  int a,b;
  scanf("%d %d",&a,&b);
  printf("%d与%d最大公约数为：",a,b);
  printf("%d\n",gcd(a,b));
  return 0;
}