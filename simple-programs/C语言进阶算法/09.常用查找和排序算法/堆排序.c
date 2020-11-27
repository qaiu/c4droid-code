#include <stdio.h>  
#include <stdlib.h> 
/* 建立大顶堆 */
void HeapAdjust(int *a,int n,int len)
{  
  int temp = a[n];  
  for(int j = n * 2;j < len;j *= 2)  
  {  
    /* 如果结点的左孩子小于右孩子增加j的值 */
    if(j < len - 1 && a[j] < a[j + 1])
      /* 用于记录较大的结点的下标 */
      ++j;
    /* 如果父结点大于等于两个孩子,则满足大顶堆的定义，跳出循环 */
    if(temp >= a[j])
      break;  
    /* 否则用较大的结点替换父结点 */
    a[n] = a[j];
    /* 记录下替换父结点的结点下标 */
    n = j;
  } 
  /* 把原来的父结点移动到替换父结点的结点位置 */
  a[n] = temp;
} 
/* 交换两个数值 */
void Swap(int *a,int *b)
{  
  int temp = *a;  
  *a = *b;  
  *b = temp;  
}  
/* 堆排序算法 */
void HeapSort(int *a,int len)
{  
  /* 建立大顶堆 */
  for(int i = len / 2;i >= 0;--i)
    HeapAdjust(a,i,len);  
  for(int i = len - 1;i > 0;--i)  
  {  
    /* 第个元素和最后一个元素进行交换 */
    Swap(&a[i],&a[0]);
    /* 建立大顶堆 */
    HeapAdjust(a,0,i);
  }  
}  
int main(void)  
{  
  int a[] = {12,45,34,37,66,15,6,4,17,19,25,35,3,2,1};  
  int len = sizeof(a) / sizeof(*a);  
  HeapSort(a,len);  
  printf("排序后的数组序列为: \n");  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);  
  printf("\n");  
  return 0;  
}