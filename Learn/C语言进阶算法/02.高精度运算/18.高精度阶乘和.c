/* 整型常量阶乘和 */
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

/* 高精度加法 */
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

/* 整型常量阶乘和 */
void BigNumFactAdd(int t)
{
  int i;
  char sum[2000],d[2000];
  memset(d,0,sizeof(d));
  memset(sum,0,sizeof(sum));

  /* 分别求出相应i的阶乘然后相加 */
  for(i=t;i>0;i--)
  {
    BigNumFact(i);
    strcpy(d,c);
    memset(c,0,sizeof(c));
    BigNumberAdd(d,sum);
    strcpy(sum,c);
    memset(c,0,sizeof(c));
  }
  strcpy(c,sum);
}

int main(void)
{
  int len = 0;
  BigNumFactAdd(10);

  len = strlen(c);

  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");

  return 0;
}