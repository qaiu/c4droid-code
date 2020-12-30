/* 字典序组合与序号的转换 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* comb为组合数C(n,m) */
int comb(int n,int m){
  int ret=1,i;
  m=m<(n-m)?m:(n-m);
  for (i=n-m+1;i<=n;ret*=(i++));
  for (i=1;i<=m;ret/=(i++));
  return m<0?0:ret;
}

/* 字典序组合转至序号 */
int comb2num(int n,int m,int *c){
  int ret=comb(n,m),i;
  for (i=0;i<m;i++)
    ret-=comb(n-c[i],m-i);
  return ret;
}

/* 序号转至字典序组合 */
void num2comb(int n,int m,int* c,int t){
  int i,j=1,k;
  for (i=0;i<m;c[i++]=j++)
    for (;t>(k=comb(n-j,m-i-1));t-=k,j++);
}

int main(void)
{
  int a[5] = {1,3,2,4,5};
  int b[5] = {0};

  printf("%d\n",comb2num(5,2,a)+1);

  num2comb(5,2,b,3);
  for(int i=0;i<5;i++)
  {
    printf("%d",b[i]);
  }
  printf("\n");
  
  return 0;
}