#include <stdio.h>  
#include <stdlib.h> 
/* 交换两个数值 */
void Swap(int *a,int *b)
{  
  int temp = *a;  
  *a = *b;  
  *b = temp;  
} 
/* 简单排序算法 */
void SelectSort(int *a,int len)
{  
  /* 用于记录最小元素的下标 */
  int min;
  for(int i = 0;i < len - 1;++i)  
  {  
    /* 设i为最小元素的下标 */
    min = i;
    for(int j = i + 1;j < len;++j)  
    {  
      /* 如果a[min] > a[j]则把j记录为最小值 */
      if(a[min] > a[j])
        min = j;  
    }  
    /* 如果i不为最小值的下标，进行交换 */
    if(i != min)
      Swap(&a[i],&a[min]);  
  }  
}   
int main(void)  
{  
  int a[] = {20,12,11,9,8,35,56,25,5,6};  
  int len = sizeof(a) / sizeof(*a);  
  printf("排序前数组的元素:\n");  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);  
  printf("\n");  
  SelectSort(a,len);  
  printf("排序后数组的元素:\n");  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);
  return 0;  
}