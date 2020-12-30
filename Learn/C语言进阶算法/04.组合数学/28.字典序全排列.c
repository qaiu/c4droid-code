/* 字典序全排列与序号的转换 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 字典序全排列转到序号 */
int perm2num(int n,int *p){
  int i,j,ret=0,k=1;
  for(i=n-2;i>=0;k*=n-(i--))
    for(j=i+1;j<n;j++)
      if(p[j]<p[i])
        ret+=k;
  return ret;
}

/* 序号转到字典序全排列 */
void num2perm(int n,int *p,int t){
  int i,j;
  for(i=n-1;i>=0;i--)
    p[i]=t%(n-i),t/=n-i;
  for(i=n-1;i;i--)
    for(j=i-1;j>=0;j--)
      if(p[j]<=p[i])
        p[i]++;
}


int main(void)
{
  int a[5] = {1,3,2,4,5};
  int b[5] = {0};

  printf("%d\n",perm2num(5,a)+1);

  num2perm(5,b,3);
  for(int i=0;i<5;i++)
  {
    printf("%d",b[i]);
  }
  printf("\n");
  
  return 0;
}