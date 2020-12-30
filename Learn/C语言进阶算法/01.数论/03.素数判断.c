/* Prime 素数判断 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 
* 判断一个整数m是否是素数，只需被2~√m之间的每一个整数去除就可以了。
* 如果m不能被2~√m间任一整数整除，m必定是素数。
*/
int is_prime(int u)
{
  if(u == 0 || u == 1)
    return 0;
  if(u == 2)
    return 1;
  if(u%2 == 0)
    return 0;
  for(int i=3;i <= sqrt((float)u);i+=2)
    if(u%i == 0)
      return 0;
  return 1;
}

int main(void)
{
  int a;
  scanf("%d",&a);

  if(is_prime(a))
  {
    printf("%d是素数\n",a);
  }
  else
  {
    printf("%d不是素数\n",a);
  }

  return 0;
}