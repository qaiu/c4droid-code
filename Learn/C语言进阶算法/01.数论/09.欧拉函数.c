/* Euler Function欧拉函数 */
#include <stdio.h>
#include <stdlib.h>

/* 求2..n-1中与n互质数的个数 */
int euler(int n)
{
  int ans = 1;
  int i;
  for(i=2;i*i<=n;i++)
  {
    if(n%i==0)
    {
      n/=i;
      ans*=i-1;

      while(n%i==0)
      {
        n/=i;
        ans*=i;
      }
    }
  }
  if(n>1)
  {
    ans *= n-1;
  }
  return ans;
}

int main(void)
{
  int a,num;
  scanf("%d",&a);
  num = euler(a);
  printf("2...%d中与%d互质数的个数是：%d\n",a-1,a,num);
  return 0;
}