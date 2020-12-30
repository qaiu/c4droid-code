/* 高精度乘法 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* 全局变量，存储大数运算的结果 */
char c[2000];

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

/* 高精度乘以低精度 
 * 将多位数存入数组，低位在前、高位在后，然后用一位数去乘数组的各位，考虑进位，最后按正常顺序输出*/
void BigNumMultiSmall(char a1[],int b1)
{
  int i,j,t;
  
  int a[2000]={0};
  t=strlen(a1);
  /* 将字符串转化为整型数组，并逆置 */
  for(i=0;i<t;i++)
    a[i]=a1[t-1-i]-'0';
  a[0]=a[0]*b1;
  /* 整型数组的每个元素乘以b1，然后对其进行求余，整除，使其只有一位数 */
  for(i=1;i<t;i++)
  {
    a[i]*=b1;
    a[i]+=a[i-1]/10;
    a[i-1]=a[i-1]%10;
  }
  /* 若最后一个元素大于9 */
  while(a[i-1]>9)
  {
    a[i]=a[i-1]/10;
    a[i-1]=a[i-1]%10;
    i++;
  }

  /* 将得到的整型数组逆置赋给字符串 */
  for(j=0;j<i;j++)
    c[j]=a[i-j-1]+'0';
  /* 若积全为0，则只输出一个0 */
  if(Judge(c))
    strcpy(c,"0");
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

int main()
{
  char a1[] = {'1','2','3','4','5','6','7','8','9','\0'};
  int b1 = 12345;
  char b2[] = {'2','3','4','5','6','6','7','8','9','\0'};
  char len = 0;

  BigNumMultiSmall(a1,b1);

  len = strlen(c);

  printf("123456789*12345=");
  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");

  memset(c,0,2000);
  BigNumMultiBig(a1,b2);

  len = strlen(c);

  printf("123456789*234566789=");
  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");

  return 0;
}