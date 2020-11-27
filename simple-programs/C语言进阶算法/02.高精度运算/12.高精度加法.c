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
  
  return 0;
}