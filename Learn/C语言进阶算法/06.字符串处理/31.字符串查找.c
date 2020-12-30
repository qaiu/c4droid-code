#include <stdlib.h>
#include <string.h>

/*
 * str[]：在此源字符串进行查找操作
 * key[]：被查找的字符串，不能为空串
 * 返回值：如果查找成功，返回key在str中第一次出现的位置，否则返回-1
 */
int strfind(char str[],char key[])
{
  int l1,l2,i,j,flag;
  l1=strlen(str);
  l2=strlen(key);
  for(i=0;i<=l1-l2;i++)
  {
    flag=1;
    for(j=0;j<l2;j++)
      if(str[i+j]!=key[j])
      {
        flag=0;
        break;
      }
      if(flag)
        return i;
  }
  return -1;
}

int main(void)
{
  int pos;
  char str[20]="abcdfadabddfa";
  char key[3]="fa";
  pos = strfind(str,key);
  printf("pos:%d\n",pos);
  return 0;
}