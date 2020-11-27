#include <stdio.h>  
#include <stdlib.h> 
int Sort(int *a,int low,int high);
void Swap(int *a,int low,int high);
/* 快速排序算法 */
void QSort(int *a,int low,int high)
{  
  if(low < high)  
  {  
    /* 将a[low].......a[high]一分为二,算出枢轴值 */
    int pivot = Sort(a,low,high);
    /* 将前一半快速排序 */
    QSort(a,low,pivot-1);
    /* 将后一半快速排序 */
    QSort(a,pivot+1,high); 
  }  
} 
void QuickSort(int *a,int len)  
{  
  QSort(a,0,len-1);  
}  
int Sort(int *a,int low,int high)  
{  
  /* 选待排序列的第一个元素为枢轴值 */
  int pivotkey = a[low]; 
  /* 从序列的两端后中间扫描进行排序 */
  while(low < high)
  {  
    while(low < high && a[high] >= pivotkey)   
      high--;  
    /* 交换两值 */
    Swap(a,low,high);
    while(low < high && a[low] <= pivotkey)  
      low++;  
    Swap(a,low,high);  
  }  
  /* 返回区轴所在位置 */
  return low;
}  
/* 交换两个位置的元素 */
void Swap(int *a,int low,int high)
{  
  int temp = a[low];  
  a[low] = a[high];  
  a[high] = temp;  
}   
int main()  
{  
  int a[] = {20,87,12,67,23,10,17,66,81,24,13};  
  int len = sizeof(a) / sizeof(*a);  
  QuickSort(a,len);  
  printf("排序后的序列为: \n");  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);  
  printf("\n");  
  return 0;  
}