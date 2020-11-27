/* 高精度的乘方 */
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

/* 高精度乘以高精度 */
void BigNumMultiBig(char a1[],char b1[])
{
  int i,j,k,lena,lenb;
  int a[1000]={0},b[1000]={0},d[2000]={0};
  lena=strlen(a1);
  lenb=strlen(b1);
  /* 将字符串转化为整型数组，并逆置 */
  for(i=0;i<lena;i++)
    a[i]=a1[lena-i-1]-'0';
  for(i=0;i<lenb;i++)
    b[i]=b1[lenb-i-1]-'0';
  /* 计算乘数从低位到高位以此乘以被乘数的低位到高位 */
  for(i=0;i<lena;i++)
    for(j=0;j<lenb;j++)
    {
      d[i+j]=d[i+j]+a[i]*b[j];
      d[i+j+1]+=d[i+j]/10;
      d[i+j]=d[i+j]%10;
    }
    /* 根据高位是否为判断整型数组的位数 */
    k=lena+lenb;
    while(!d[k-1])
      k--;
    /* 积转化为字符型 */
    for(i=0;i<k;i++)
      c[i]=d[k-1-i]+'0';
    /* 若积全为0，则只输出一个1 */
    if(Judge(c))
      strcpy(c,"0");
}

/* 高精度的乘方 */
void BigNumInvol(char a1[],int b1)
{
  int i;
  char temp[1000];
  strcpy(temp,a1);
  for(i=2;i<b1;i++)
  {
    BigNumMultiBig(a1,temp);
    strcpy(temp,c);
    memset(c,0,sizeof(c));
  }
  
  BigNumMultiBig(a1,temp);
  if(Judge(c))
    strcpy(c,"0");
}

int main(void)
{
  char a1[] = {'1','2','3','4','5','6','7','8','9','\0'};
  int len = 0,b1=2;
  BigNumInvol(a1,b1);

  len = strlen(c);

  printf("123456789^%d=",b1);
  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");

  return 0;
}