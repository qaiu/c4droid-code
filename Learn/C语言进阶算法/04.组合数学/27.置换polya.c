/* 求置换的循环节,polya原理 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXN 1000

int gcd(int a,int b){
  return b?gcd(b,a%b):a;
}

/* perm[0..n-1]为0..n-1的一个置换(排列)
* 返回置换最小周期,num返回循环节个数
*/
int polya(int* perm,int n,int* num){
  int i,j,p=0,v[MAXN]={0},ret=1;
  for(*num=i=0;i<n;i++)
    if(!v[i]){
      for(*num++,j=0,p=i;!v[p=perm[p]];j++)
        v[p]=1;
      ret*=j/gcd(ret,j);
    }
  return ret;
}

int main(void)
{
  int perm[5]={1,2,3,4,5},num,ret;
  ret = polya(perm,5,&num);

  printf("ret:%d num:%d\n",ret,num);

  return 0;
}