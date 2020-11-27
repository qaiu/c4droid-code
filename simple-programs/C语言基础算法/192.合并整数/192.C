/*
例程192. 合并两个整数 (仅支持四位数)
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <stdio.h>

void fun(int a, int b, long *c) {
  *c = a / 10 * 10 + a % 10 * 1000 + b / 10 * 100 + b % 10;
}
int main(){
  int a, b;
  long c;
  printf("Input a, b:");
  scanf("%d%d", &a, &b);
  fun(a, b, &c);
  printf("The result is: %ld\n", c);
  return 0;
}