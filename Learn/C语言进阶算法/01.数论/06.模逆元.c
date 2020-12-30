/* Module Inverse 模逆元 */
#include <stdio.h>
#include <stdlib.h>

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
 
/* ax ≡ 1 (mod n) 如540*1674 mod 1769=1*/
int Inv(int a,int n)
{
  int d,x,y;
  d = extended_euclid(a, n,&x,&y);
  if(d == 1)
    return (x%n+n)%n;
  else
    /* 无解 */
    return -1;
}

int main(void)
{
  int a,n,x;

  scanf("%d%d",&a,&n);

  x = Inv(a,n);
  if(x!=-1)
  {
    printf("%d*%d ≡ 1 (mod %d)\n",a,x,n);
  }
  else
  {
    printf("无解\n");
  }
  
  return 0;
}