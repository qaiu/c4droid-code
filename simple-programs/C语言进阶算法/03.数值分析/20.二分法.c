/* 二分法 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(double x)
{
  return 1+x-x*x*x;
}

/* 1+x-x^3 = 0 */
int main(void)
{
  double a = 1, b = 2, e = 1e-5;

  e = fabs(e);
  if (fabs(f(a)) <= e)
  {
    printf("solution: %lg\n", a);
  }
  else if (fabs(f(b)) <= e)
  {
    printf("solution: %lg\n", b);
  }
  else if (f(a)*f(b) > 0)
  {
    printf("f(%lg)*f(%lg) > 0 ! need <= 0 !\n", a, b);
  }
  else
  {
    while (fabs(b-a) > e)
    {
      double c = (a+b)/2.0;
      if (f(a)* f ( c ) < 0)
        b = c;
      else
        a = c;
    }
    printf("solution: %lg\n", (a+b)/2.0);
  }

  return 0;
}