/* Sieve Prime 素数筛法 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 
 * 把从1开始的、某一范围内的正整数从小到大顺序排列，1不是素数，首先把它筛掉。
 * 剩下的数中选择最小的数是素数，然后去掉它的倍数。依次类推，直到筛子为空时结束。
*/
/* M:1000以内素数 */
const int M = 1000;
int mark[M]; //1:prime number
void sieve_prime()
{
  for(int i=0; i <= M; i++)
  {
    mark[i] = 1;
  }
  mark[0] = mark[1] = 0;
  for(int i=2; i <= sqrt((float)M); i++)
  {
    if(mark[i]) {
      for(int j=i*i; j < M; j+=i)
        mark[j] = 0;
    }
  }
}

int main(void)
{
  int i,j = 0;

  sieve_prime();
  printf("%d以内素数如下：\n",M);
  for(i=0;i<M;i++)
  {
    if(mark[i])
    {
      printf("%d  ",i);
      j++;

      if(j%10 == 0)
      {
        printf("\n");
      }
    }
  }

  return 0;
}