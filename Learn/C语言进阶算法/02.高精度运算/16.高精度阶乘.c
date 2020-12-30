/* 整型常量阶乘 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* 全局变量，存储大数运算的结果 */
char c[1000];

/* 判断字符串ch是否全为0
* 若全为0，返回0，否则返回1
*/
int Judge(char ch[])
{
  int i,k;
  k=strlen(ch);
  for(i=0;i<k;i++)
    if(ch[i]!='0')
      return 0;
  return 1;
}

/* 整型常量阶乘 */
void BigNumFact(int x)
{
  int i,k,m=0,a[1000]={0};
  a[0]=1;
  for(;x;x--)
  {
    /* m为在求阶乘过程中a的元素个数 */
    for(k=i=0;i<=m;i++)
    {
      /* 数组各个元素均乘以x(x递减)，以完成阶乘的运算 */
      k=k+a[i]*x;
      a[i]=k%10;
      k/=10;
    }
    while(k)
    {
      a[++m]=k%10;
      k/=10;
    }
  }

  /* 阶乘的结果转化为字符型 */
  for(i=0;i<=m;i++)
    c[i]=a[m-i]+'0';
  /* 若结果全为0，则只输出一个 */
  if(Judge(c))
    strcpy(c,"0");
}
/* 计算100！ */
int main(void)
{
  int len = 0;
  BigNumFact(100);

  len = strlen(c);

  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");

  return 0;
}