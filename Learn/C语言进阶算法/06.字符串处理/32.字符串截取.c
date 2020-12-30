#include <stdlib.h>
#include <string.h>

/*
 * 字符串截取
 * str[]：操作的目标字符串
 * start：从第start个字符串开始，截取长度为len的字符
 * len：从第start个字符串开始，截取长度为len的字符
 * strback[]：截取的到的字符
 * 返回值：0：超出字符串长度，截取失败；1：截取成功
 */
int mid(char str[],int start,int len,char strback[])
{
  int l,i,k=0;
  l=strlen(str);
  if(start+len>l)
    return 0;
  for(i=start;i<start+len;i++)
    strback[k++]=str[i];
  strback[k]='\0';
  return 1;
}

int main(void)
{
  int start=3,len=5;
  char str[20]="abcdfadabddfa";
  char strback[10]="\0";
  mid(str,start,len,strback);
  printf("strback:%s\n",strback);
  return 0;
}