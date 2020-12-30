/* 高精度除法 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* 全局变量，存储大数运算的结果 */
char c[1000];
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
  int lena,lenb,i,k,flag;
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

/* 高精度除以低精度 */
int BigNumDividSmall(char a1[],int b1)
{
  if(!b1)
    return 0;
  int i,j,k,flag=0,a[1000]={0};
  char b[2000];
  memset(b,0,sizeof(b));
  k=strlen(a1);
  for(i=0;i<k;i++)
    a[i]=a1[i]-'0';
  z=0;
  for(i=0;i<k;i++)
  {
    z=a[i]+z*10;
    b[i]=z/b1+'0';
    z=z%b1;
  }
  i=j=0;
  while(b[i++]=='0');
  for(i=i-1;i<k;i++)
    c[j++]=b[i];
  return 1;
}

/* 高精度除以高精度 */
void BigNumDividBig(char a1[],char b1[])
{
  char b[1000],time[1000];
  int lena1,lentime,i,j,k,flag=0;
  memset(arr,0,sizeof(arr));//若被除数小于除数，则商为，余数为被除数
  if(Compare(a1,b1)<0)
    strcpy(arr,a1);
  //若两数相等，则商为，余数为
  else
    if(!Compare(a1,b1))
      c[0]='1';
  //若被除数大于除数
    else
    {
      j=lentime=0;
      lena1=strlen(a1);
      memset(b,0,sizeof(b));
      memset(time,0,sizeof(time));
      for(i=0;i<lena1;i++)
      {//计算得到被除数的前几位,得到整型数组形式的商
        //time的一个元素表示一次相除的商
        b[j++]=a1[i];
        flag=0;
        while(Compare(b,b1)>=0)
        {
          BigNumberSub(b,b1);
          strcpy(b,c);
          memset(c,0,sizeof(c));
          time[lentime]++;
          flag=1;//控制time的元素的位置
        }
        if(flag)//将商转换为字符
          time[lentime]+='0';
        else//当被除数前几位小于除数，商补
          time[lentime]='0';
        if(!strcmp(b,"0"))//若b为‘’
          j=0;
        else//继续在b的后面加值
          j=strlen(b);
        lentime++;
      }
      k=0;
      for(i=0;i<lentime;i++)
        if(time[i]!='0')
          break;//找到time数组中第一个不为的位置
      for(j=i;j<lentime;j++)
        c[k++]=time[j];
      strcpy(arr,b);
    }
    if(Judge(c))
      strcpy(c,"0");
    if(Judge(arr))
      strcpy(arr,"0");
}  

int main(void)
{
  char a1[] = {'9','2','3','4','5','6','7','8','9','\0'};
  int b1 = 12345;
  char b2[] = {'2','3','4','5','6','6','7','8','9','\0'};
  char len = 0;

  BigNumDividSmall(a1,b1);

  len = strlen(c);

  printf("923456789/12345=");
  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");
  printf("余数：%ld\n",z);

  memset(c,0,2000);
  BigNumDividBig(a1,b2);

  len = strlen(c);

  printf("923456789/234566789=");
  for(int i=0;i<len;i++)
  {
    printf("%c",c[i]);
  }
  printf("\n");
  
  printf("余数：");
  for(int i=0;i<strlen(arr);i++)
  {
    printf("%c",arr[i]);
  }
  printf("\n");
  return 0;
}