/* 高精度加法 */
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

/* 先确定a和b中的最大位数k，然后依照由低至高位的顺序进行加法运算。
 * 注意进位，若高位有进位，则c的长度为k+1。
 */
void BigNumberAdd(char a1[],char b1[])
{
  int i,j,k,lena,lenb;
  int a[1000]={0},b[1000]={0},d[1000]={0};
  lena=strlen(a1);
  lenb=strlen(b1);
  /* 将加数与被加数化为整型数组 */
  for(i=0;i<lena;i++)
    a[i]=a1[lena-i-1]-'0';
  for(i=0;i<lenb;i++)
    b[i]=b1[lenb-1-i]-'0';

  k=lena>lenb?lena:lenb;
  for(i=0;i<k;i++)
  {
    d[i]=a[i]+b[i]+d[i];
    d[i+1]=d[i+1]+d[i]/10;
    d[i]=d[i]%10;
  }
  /* 若高位进 */
  while(d[k])
    k++;
  while(!d[k-1])
    k--;
  /* 将整型数组逆着转变并赋值给c字符型数组 */
  for(j=0;j<k;j++)
    c[j]=d[k-j-1]+'0';
  /* 若全为0，则只输出一个 */
  if(Judge(c))
    strcpy(c,"0");
}

int main()
{
  char a1[] = {'1','2','3','4','5','6','7','8','9','\0'};
  char b1[] = {'2','3','4','5','6','6','7','8','9','\0'};
  char len = 0;

  BigNumberAdd(a1,b1);

  len = strlen(c);

  printf("len:%d result:\n",len);
  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");
  
  system("pause");
  return 0;
}/* 高精度减法 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* 全局变量，存储大数运算的结果 */
char c[2000];
/* 高精度除以高精度的余数 */
char arr[1000];
/* 高精度除以低精度的余数 */
long z=0;

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

/* 比较字符串的大小 */
int Compare(char a[],char b[])
{
  int lena,lenb,i; 
  lena=strlen(a);
  lenb=strlen(b);
  if(lena<lenb)
    return -1;
  else
    if(lena>lenb)
      return 1;
    else
    {
      if(strcmp(a,b)==0)
        return 0;
      else
      {
        for(i=0;i<lena;i++)
        {
          if(a[i]>b[i])
            return 1;
          if(a[i]<b[i])
            return -1;
        }
        return 0;
      }
    }
}


/* 依照由低位至高位的顺序进行减法运算。在每次位运算中，若出现不够减的情况，
* 则向高位借位。在进行了la的减法后，若最高位为0，则a的长度减。
* 若A、B大小未知，则需先判断大小。
* a1为被减数，b1为减数
*/
void BigNumberSub(char a1[],char b1[])
{
  int lena,lenb,i,j,k,flag;
  int a[1000]={0},b[1000]={0},d[1000]={0};
  lena=strlen(a1);
  lenb=strlen(b1);
  /* 若被减数大于等于减数 */
  if(Compare(a1,b1)>=0)
  {
    for(i=0;i<lena;i++)
      a[i]=a1[lena-1-i]-'0';
    for(i=0;i<lenb;i++)
      b[i]=b1[lenb-1-i]-'0';
    /* 结果正的标志 */
    flag=0;
  }
  else
  {
    /* 若被减数小于减数 */
    for(i=0;i<lenb;i++)
      a[i]=b1[lenb-1-i]-'0';
    for(i=0;i<lena;i++)
      b[i]=a1[lena-1-i]-'0';
    /* 结果负的标志 */
    flag=1;
  }
  k=lena>lenb?lena:lenb;
  /* 大数减小数 */
  for(i=0;i<k;i++)
  {
    if(a[i]<b[i])
    {
      /* 若被减数不够减，向高位借一位 */
      a[i+1]--;
      d[i]=a[i]-b[i]+10;
    }
    else
      d[i]=a[i]-b[i];
  }

  /* 若较高位已为0，并且不止1位时 */
  while(!d[i-1])
  {
    k--;
    i--;
  }

  /* 根据flag，输出有无"-" */
  if(!flag)
  {
    /* 将结果转化为字符逆着赋给数组c */
    for(i=0;i<k;i++)
    {
      /* 若差的第一个字母为0，则马上跳过 */
      if(!i&&!d[k-i-1])
        continue;
      c[i]=d[k-i-1]+'0';
    }
  }
  else
  {
    c[0]='-';
    /* 将结果转化为字符逆着赋给数组c */
    for(i=1;i<=k;i++)
    {
      /* 若差的第一个字母为0，则马上跳过 */
      if(i==1&&!d[k-i])
        continue;
      /* 注意d的下标，不是k-i-1 */
      c[i]=d[k-i]+'0';
    }
  }
  /* 若差全为0，则只输出一个 */
  if(Judge(c))
    strcpy(c,"0");
}

int main()
{
  char a1[] = {'1','2','3','4','5','6','7','8','9','\0'};
  char b1[] = {'2','3','4','5','6','6','7','8','9','\0'};
  char len = 0;

  BigNumberSub(a1,b1);

  len = strlen(c);

  printf("result:\n");
  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");

  return 0;
}