/* Modular Linear Equation 模线性方程(同余方程) */
#include <stdio.h>
#include <stdlib.h>

/* 求最大公约数 */
int gcd(int x,int y)
{
  int t;
  while(y > 0)
  {
    t = x % y;
    x = y;
    y = t;
  }
  return x;
}

/* 扩展欧几里德算法 */
int extended_euclid(int a,int b,int *x,int *y)
{
  int d;
  if(b == 0) {
    *x = 1;
    *y = 0;
    return a;
  }
  d = extended_euclid(b,a%b,y,x);
  *y-=a/b*(*x);
  return d;
}
 
/* 如果GCD(a,b)不能整除c,则ax+by=c没有整数解
 * ax≡b (mod n) n > 0
 * 上式等价于二元一次方程ax–ny=b 
 */
void modular_linear_equation(int a,int b,int n)
{
  int d,x,y,x0,g;
  g = gcd(a,n);
  if(b%g!=0){
    printf("无解\n");
    return;
  }

  a /= g;
  b /= g;
  n /= g;
  d = extended_euclid(a,n,&x,&y);
  if(b%d==0)
  {
    /* x0:basic solution */
    x0 = (x*(b/d))%n;
    int ans = n;
    for(int i=0;i<d;i++){
      ans = (x0+i*(n/d))%n;
      printf("%d\n",ans);
    }
  }
  else
    printf("无解\n");
}

int main(void)
{
  int a,b,n;

  scanf("%d%d%d",&a,&b,&n);

  modular_linear_equation(a,b,n);

  return 0;
}