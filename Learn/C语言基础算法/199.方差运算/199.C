/* 
   例程199. 
   方差标准差
   整理优化by:千百度QAIU 
   QQ:736226400
   编译环境:gcc/tcc 
   2017/10/25 
 */

#include "stdio.h"
#include "math.h"
#define N 100
int main() {
  int a[N], n, i;
  double aver, s;
  double sum = 0, e = 0;
  printf("请输入样本量：");
  scanf("%d", &n);
  printf("请输入%d个样本：", n);
  for (i = 0; i < n; i++) {
    scanf("%d", &a[i]);
    sum += a[i];
  }
  aver = sum / n;
  for (i = 0; i < n; i++)
    e += (a[i] - aver) * (a[i] - aver);
  e /= n - 1;
  s = sqrt(e);
  printf("平均数为：%.2f,方差为：%.2f,标准差为：%.2f\n", aver, e, s);
  return 0;
}