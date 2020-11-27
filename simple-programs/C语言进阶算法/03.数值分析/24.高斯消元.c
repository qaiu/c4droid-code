/* 高斯消元 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 10
int main()
{
  int i=0,j=0,r=0,n=0,k=1;
  float a[N][N],x[N],max=-1,sum1=0,sum2=0;
  /* 输入矩阵维数 */
  printf("Please enter n(0<n<11):\n");
  scanf("%d",&n);
  /* 输入矩阵元素，最后一列为b[] */ 
  for(i=1;i<=n;i++)
  {
    for(j=1;j<=n+1;j++)
    {
      printf("Please enter a[%d][%d]:\n",i,j);
      scanf("%f",&a[i][j]);  
    }
  }
  do
  {
    for(i=k;i<=n;i++)
    {
      if(fabs(a[i][k])>max)
      {
        max=a[i][k];
        r=i;
      }
      if(a[r][k]==0)
      {
        printf("Cannot solve!\n");
        exit(1);
      }

    }

    /* 换行 */ 
    for(j=1;j<=n+1;j++)
    {
      sum1=a[k][j];
      a[k][j]=a[r][j];
      a[r][j]=sum1;
    }

    /* 将矩阵化为上三角矩阵 */ 
    for(i=k+1;i<=n;i++)
    {
      for(j=k+1;j<=n+1;j++)
      {
        a[i][j]=a[i][j]-(a[i][k])*(a[k][j])/(a[k][k]);
      }
    }
    k=k+1;
    max=-1;       
  }while(k<(n-1));

  /* 求x[] */ 
  x[n]=(a[n][n+1])/(a[n][n]);
  for(k=n-1;k>=1;k--)
  {
    for(j=k+1;j<=n;j++)
      sum2=sum2+(a[k][j])*(x[j]);
    x[k]=(a[k][n+1]-sum2)/(a[k][k]);
    sum2=0;
  }

  /* 输出结果 */ 
  for(i=1;i<=n;i++)
  {
    printf("%f\n",x[i]);
  }
  while(1);
  return 0;
}