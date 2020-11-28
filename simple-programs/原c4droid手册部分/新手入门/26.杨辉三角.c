/*c4droid代码手册
*杨辉三角
*作者未知
*转载请说明出处
*/
#include<stdio.h>
int main()
{
    int  b , i , j , t , m , k , d , n ;
    printf("输入层数:");
    scanf("%d",&n);
    
    int a[n][1+n];
    
    for(i=0;i<n;i++)
          for(j=0;j<n+1;j++)
                a[i][j]=0;
                
     a[0][1]=1;
     
     for(i=1;i<n;i++)
     {
         for(j=1;j<n+1;j++)
         {
             m=a[i-1][j-1];
             t=a[i-1][j];    
             a[i][j]=m+t;
         }
     }
     
     for(i=0;i<n;i++)
     {
         for(k=n-i+1;k>0;k--)
         printf("  ");
         
         for(j=0;j<n+1;j++)
         if(a[i][j]==0)
         printf("  ");
         
         else
         printf("%3d ",a[i][j]);
         
         printf("\n");
     }
    return 0;
}