/* 数字转换为字符 */
#include <stdlib.h>
#include <math.h>

/*
 * k：转换的数字
 * o[]：存储转换结果的字符串
 * 返回值：null
 */
void cstr(int k,char o[])
{
  int len,i,t;
  len=log10((float)k)+1;
  for(i=len;i>0;i--)
  {
    t=k%10;
    k-=t;k/=10;
    o[i-1]='0'+t;
  }
  o[len]='\0';
}

int main(void)
{
  int k=9870;
  char o[20]="\0";
  cstr(k,o);
  printf("o:%s\n",o);
  return 0;
}