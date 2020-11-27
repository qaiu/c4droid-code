/* pollard_rho 算法进行质因数分解 */
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

/* 质因数分解结果 */
long long factor[100];
/* 质因数的个数 */
int tol;

/* 最大公约数 */
long long gcd(long long a,long long b)
{
  if(a==0)return 1;
  if(a<0) return gcd(-a,b);
  while(b)
  {
    long long t=a%b;
    a=b;
    b=t;
  }
  return a;
}

/* 质因数分解 */
long long Pollard_rho(long long x,long long c)
{
  long long i=1,k=2;
  long long x0=rand()%x;
  long long y=x0;
  while(1)
  {
    i++;
    x0=(mult_mod(x0,x0,x)+c)%x;
    long long d=gcd(y-x0,x);
    if(d!=1&&d!=x) return d;
    if(y==x0) return x;
    if(i==k){y=x0;k+=k;}
  }
}

/* 对n进行素因子分解 */
void findfac(long long n)
{
  /* 素数 */
  if(Miller_Rabin(n))
  {
    factor[tol++]=n;
    return;
  }
  long long p=n;
  while(p>=n)p=Pollard_rho(p,rand()%(n-1)+1);
  findfac(p);
  findfac(n/p);
}

int main()
{
  long long n;
  while(scanf("%lld",&n)!=EOF)
  {
    tol=0;
    findfac(n);
    for(int i=0;i<tol;i++)
      printf("%lld ",factor[i]);
    printf("\n");
    if(Miller_Rabin(n))
      printf("Yes\n");
    else 
      printf("No\n");
  }

  return 0;
}