#include <stdio.h>
int main() {
  int fun(unsigned long long);
  unsigned long long n;
  printf("输入n的值\n");
  scanf("%ld", &n);
  fun(n);
}
int fun(unsigned long long n) {
  int i = 0, j = 0, k = 0;      // i表示偶数个数，j表示奇数个数，k代表总个数
  unsigned long long p;
  while (n > 0) {
    p = n % 10;
    if (p % 2 == 0)
      i++;
    else
      j++;
    n = n / 10;
    k++;
  }
  printf("输出新形成的数:\n偶 奇 总\n");
  printf("%d  %d  %d\n", i, j, k);
  n = i * 100 + j * 10 + k;
  while (n != 123)
    return fun(n);
}