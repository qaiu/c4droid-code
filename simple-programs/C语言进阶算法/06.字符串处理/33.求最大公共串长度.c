#include <stdlib.h>
#include <string.h>

/*
 * 最大公共子串长度
 * a,b[]：根据a,b生成最大公共子串
 * 返回值：最大公共子串的长度
 * M、N是a,b数组的最大可能长度
 */
#define M 20
#define N 20
int c[M][N];
int lcs_len(char *a,char *b)
{
  int m=strlen(a),n=strlen(b),i,j;
  for(i=0;i<=m;i++)
    c[i][0]=0;
  for(j=0;j<=n;j++)
    c[0][j]=0;
  for(i=1;i<=m;i++)
    for(j=1;j<=n;j++)
    {
      if(a[i-1]==b[j-1])
        c[i][j]=c[i-1][j-1]+1;
      else if(c[i-1][j]>c[i][j-1])
        c[i][j]=c[i-1][j];
      else
        c[i][j]=c[i][j-1];
    }
  return c[m][n];
}

int main(void)
{
  int len;
  char a[20]="abcdfadabddfa";
  char b[20]="abddfa";
  len=lcs_len(a,b);
  printf("len:%d\n",len);
  return 0;
}