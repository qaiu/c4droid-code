/* 生成gray码 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 每次调用gray取得下一个码
 * 000...000是第一个码,100...000是最后一个码 */
void gray(int n,int *code){
  int t=0,i;
  for (i=0;i<n;t+=code[i++]);
  if (t&1)
    for (n--;!code[n];n--);
  code[n-1]=1-code[n-1];
}

int main(void)
{
  int n=5,code[5],i=0;
  memset(code,0,5*sizeof(int));

  printf("5 bits gray code:\n");
  gray(n,code);
  for(i=0;i<5;i++)
  {
    printf("%d",code[i]);
  }
  printf("\n");

  while(1)
  {
    gray(n,code);
    for(i=0;i<5;i++)
    {
      printf("%d",code[i]);
    }
    printf("\n");

    if(code[0]==1&&code[1]==0&&code[2]==0&&code[3]==0&&code[4]==0)
    {
      break;
    }
  }

  return 0;
}