#include <stdio.h>  
#include <stdlib.h> 
/* 希尔排序算法 */
void ShellSort(int *a,int len)
{  
  int gap,j;  
  for(gap = len / 2;gap > 0;gap /= 2)  
  {  
    /* j从gap个元素开始 */
    for(j = gap;j < len;++j)
    {  
      /* 每个元素与自己组内的数据进行直接插入排序 */
      if(a[j] < a[j - gap])  
      {  
        int temp = a[j];  
        int k = j - gap;  
        while(k >= 0 && a[k] > temp)  
        {  
          a[k + gap] = a[k];  
          k -= gap;  
        }  
        a[k + gap] = temp;  
      }  
    }  
  }  
}  
int main(void)  
{  
  int a[] = {12,5,35,42,37,15,4,3,2,1,11,9,23};  
  int len = sizeof(a) / sizeof(*a);  
  ShellSort(a,len);  
  printf("经过希尔排序后的数组序列:\n");  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);  
  printf("\n");  
  return 0;  
}