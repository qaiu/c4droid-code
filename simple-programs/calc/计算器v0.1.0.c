/* 
   简易计算器v1.0
    by:千百度 */
#include <stdio.h>

int main(void)
{
	char c[2];
	double a, b, d;

  while (1)
  {
    printf("请输入第一个数字：\n");
    scanf("%lf", &a);
    printf("请输入运算符号：\n");
    scanf("%c", c);
    scanf("%c", &c);
    printf("请输入第二个数字：\n");
    scanf("%lf", &b);
    switch (c[0])
    {
    case '+':
      d = a + b;
      break;

    case '-':
      d = a - b;
      break;

    case '*':
      d = a * b;
      break;

    case '/':
      d = a / b;
      break;

    default:
      printf("输入有误\n");
      break;
    }

    if (c[0] == '+' || c[0] == '-' || c[0] == '*' || c[0] == '/')
    printf("%g %c %g = %g\n", a, c[0], b, d);
    }
  return 0;
  }