/* Miller_Rabin 算法进行素数测试，可以判断 <2^63的数 */
#include <stdio.h>
#include <stdlib.h>

/* 随机算法判定次数，S越大，判错概率越小 */
const int S=20;

/* 计算 (a*b)%c  a,b,c <2^63 */
long long mult_mod(long long a,long long b,long long c)
{
  a%=c;
  b%=c;
  long long ret=0;
  while(b)
  {
    if(b&1){ret+=a;ret%=c;}
    a<<=1;
    if(a>=c)a%=c;
    b>>=1;
  }
  return ret;
}

/* 计算  x^n %c */
long long pow_mod(long long x,long long n,long long mod)//x^n%c
{
  if(n==1)return x%mod;
  x%=mod;
  long long tmp=x;
  long long ret=1;
  while(n)
  {
    if(n&1) ret=mult_mod(ret,tmp,mod);
    tmp=mult_mod(tmp,tmp,mod);
    n>>=1;
  }
  return ret;
}

/* 以a为基,n-1=x*2^t a^(n-1)=1(mod n)  验证n是不是合数
 * 一定是合数返回true,不一定返回false */
int check(long long a,long long n,long long x,long long t)
{
  long long ret=pow_mod(a,x,n);
  long long last=ret;
  for(int i=1;i<=t;i++)
  {
    ret=mult_mod(ret,ret,n);
    if(ret==1&&last!=1&&last!=n-1) return 1;
    last=ret;
  }
  if(ret!=1) return 1;
  return 0;
}

/* Miller_Rabin()算法素数判定
 * 是素数返回true，合数返回false */
int Miller_Rabin(long long n)
{
  if(n<2)return 0;
  if(n==2)return 1;
  /* 偶数 */
  if((n&1)==0) return 0;
  long long x=n-1;
  long long t=0;
  while((x&1)==0){x>>=1;t++;}
  for(int i=0;i<S;i++)
  {
    long long a=rand()%(n-1)+1;
    if(check(a,n,x,t))
      return 0;
  }
  return 1;
}

int main(void)
{
  long long a;
  int flag;
  scanf("%lld",&a);
  flag = Miller_Rabin(a);
  if(flag)
  {
    printf("%ld是素数\n",a);
  }
  else
  {
    printf("%ld不是素数\n",a);
  }

  return 0;
}