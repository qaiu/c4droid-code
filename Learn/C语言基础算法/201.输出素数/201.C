/*
例程201. 输出素数
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <conio.h>
#include <stdio.h>
int isP(int m) {
  int i;
  for (i = 2; i < m; i++)
    if (m % i == 0)
      return 0;
  return 1;
}
void num(int m, int k, int xx[])
{
  int s = 0;

  for (m = m + 1; k > 0; m++)

    if (isP(m)) {
      xx[s++] = m;
      k--;
    }
}
int main() {
  int m, n, xx[9999];
  printf("\nPlease enter two integers:");
  scanf("%d%d", &m, &n);
  num(m, n, xx);
  for (m = 0; m < n; m++)
    printf("%d ", xx[m]);
  printf("\n");
  return 0;
}