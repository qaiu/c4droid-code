#include <stdio.h>  
#include <stdlib.h> 
void MSort(int a[],int temp[],int low,int high);
/* 归并排序算法 */
void MergeSort(int *a,int len)
{  
  /* 动态分配一个额外的存储空间 */
  int *TR = (int *)malloc(sizeof(int) * len);
  /* 分裂然后调用归并排序 */
  MSort(a,TR,0,len-1); 
  /* 释放内存 */
  free(TR);
}
/* 归并排序 */
void Merge(int a[],int TR[],int low,int mid,int high)
{  
  int i = low;  
  int j = mid + 1;  
  int k = 0;  
  while(i <= mid && j <= high)  
  {  
    /* 进行排序存入动态分配的数组中 */
    if(a[i] < a[j])
      TR[k++] = a[i++];  
    else  
      TR[k++] = a[j++];  
  }  
  /* 如果前一半中还有未处理完的数据,按顺序移入动态分配的数组内 */
  while(i <= mid)
    TR[k++] = a[i++];
  /* 如果后一半中还有未处理完的数据，按顺序移入动态分配的数组内 */
  while(j <= high)
    TR[k++] = a[j++]; 
  /* 将排序好的数据移回到原序列中 */
  for(int v = 0,i = low;i <= high;++v,++i)
    a[i] = TR[v];  
}  
void MSort(int a[],int temp[],int low,int high)  
{  
  int mid;  
  if(low < high)  
  {  
    /* 进行分裂 */
    mid = (low + high) / 2;
    /* 将前一半继续分裂 */
    MSort(a,temp,low,mid);
    /* 将后一半继续分裂 */
    MSort(a,temp,mid + 1,high);
    /* 进行归并排序 */
    Merge(a,temp,low,mid,high);
  }  
} 
int main(void)  
{  
  int a[] = {20,19,35,33,17,42,15,55,9,5,8,3,1};  
  int len = sizeof(a) / sizeof(*a);  
  MergeSort(a,len);  
  printf("排序后的数组序列:\n");  
  for(int i = 0;i < len;++i)  
    printf("%d ",a[i]);  
  printf("\n");
  return 0;  
}