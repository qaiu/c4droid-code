#include <stdio.h>  
#include <stdlib.h> 
/* 交换两个数值 */
void Swap(int *a,int *b)  //  
{  
  int temp = *a;  
  *a = *b;  
  *b = temp;  
}  
/* 冒泡排序算法 */
void BubbleSort(int *a,int len)
{  
  for(int i = 0;i < len - 1;++i)  
  {  
    for(int j = len - 2;j >= i;--j)  
    {  
      if(a[j] > a[j + 1])  
        Swap(&a[j],&a[j + 1]);  
    }  
  }  
}  
int main()  
{  
  int a[] = {20,15,22,30,34,14,19,1,48,5,9,10,88,17};  
  printf("排序前数组的元素为: \n");  
  int len = sizeof(a) / sizeof(*a);  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);  
  BubbleSort(a,len);  
  printf("\n排序后数组的元素为: \n");  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);
  return 0;  
}