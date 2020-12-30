#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
int Bin_Search(char *s,int value,int n)  
{  
  /* 设置最小值为0 */
  int min = 0;
  /* 设置最大值为n - 1 */
  int max = n - 1;
  int mid;  
  /* 位置初始化为-1 */
  int pos = -1;
  while(min < max)  
  {  
    /* 将字符串折半 */
    mid = min + (max - min)/ 2;
    /* 如果中间的值大于要查找的值，则在小的一半中查找 */
    if(s[mid] > value) 
      max = mid - 1;
    /* 如果中间的值小于要查找的值,则在大的一半中查找 */
    else if(s[mid] < value)
      min = mid + 1;
    /* 如果找到记录下位置 */
    else
    {  
      pos = mid; 
      /* 再在小的一半中继续查找 */
      max = mid - 1;
    }  
  }  
  /* 如果是第一个字符等于要查找的值，记下位置 */
  if(s[min] == value)
    pos = min;  
  return pos;
}
int main(void)
{
  char s[] = "aaaaabcccccddddeeeefff";
  char value = 'd';
  int pos;
  pos = Bin_Search(s,value,strlen(s));
  printf("pos is: %d\n",pos);
}