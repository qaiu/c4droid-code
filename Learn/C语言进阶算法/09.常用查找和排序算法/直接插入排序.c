#include <stdio.h>  
#include <stdlib.h> 
/* 直插排序算法 */
void InsertSort(int *a,int len)
{  
  int temp;  
  int j;  
  for(int i = 1;i < len;++i)  
  {  
    if(a[i] < a[i - 1])  
    {  
      /* 记录较小的值 */
      temp = a[i];
      for(j = i - 1;j >= 0 && a[j] > temp;--j)
        /* 将值向后移 */
        a[j + 1] = a[j];
      /* 将较小的值插入正确位置 */
      a[j + 1] = temp;
    }  
  }  
}
int main()  
{  
  int a[] = {13,10,11,34,23,17,29,31};  
  InsertSort(a,sizeof(a)/sizeof(*a));  
  for(int i = 0;i < sizeof(a) / sizeof(*a);++i)  
    printf("%d ",a[i]);  
  printf("\n"); 
  return 0;  
}