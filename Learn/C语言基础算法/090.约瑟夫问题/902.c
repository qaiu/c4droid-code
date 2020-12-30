// 猴子选王2（约瑟夫问题）

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <conio.h>
int fre(char mok[], int k) {
  int i;
  printf("\n猴子编号：\n");
  for (i = 0; mok[i] != '\0'; i++)
    printf("%d\t", mok[i]);     // 输出为踢出之前的编号，测试用
  for (i = k; mok[i] != '\0'; i++) {
    mok[i] = mok[i + 1];
  }                             // 一个循环，将k以后的元素前移
  putchar('\n');
  setbuf(stdin,NULL);
  for (i = 0; mok[i] != '\0'; i++)
    printf("%d\t", mok[i]);     // 输出踢出之后的编号，测试用
  printf("\n按回车继续下一轮:\n");
  getch();                      // 暂停，测试用
  return 0;
}
int main() {
  char mok[50];
  int i;
  int n, s, b;                  // n表示猴子总数；s表示步进；b表示元素个数及大王编号
  int j, k;                     // j,k都是计数器
  mok[0] = 1;                   // 初始化mok[0],让后面编号更简单的进行
  printf("请输入猴子的总数：\n");
  scanf("%d", &n);              // 输入猴子的总数
  for (i = 1; i < n; i++) {
    mok[i] = i + 1;
  }                             // 对猴子进行编号
  mok[n] = '\0';                // 用0来表示数组的结尾
  printf("请输入循环单位：\n");
  scanf("%d", &s);              // 单位长度
  b = n;                        // 统计猴子的个数
  for (j = 1, k = 0;; j++, k++) {
    if (b == 1) {
      b = mok[0];
      break;
    }                           // 如果元素只剩下一个，那么退出循环
    if (j == s) {
      printf("\n它出列了：%d\n", mok[k]);
      fre(mok, k);              // 用于元素前移的函数
      b--;
      j = 1;
    }                           // 将猴子从数组中踢出，并重置计数器J。
    if (mok[k + 1] == '\0')
      k = -1;                   // 重置计数器k，因为后面有k++所以k要在重置基础上-1.
  }                             // 判断是否为数组最后元素，重置计数器k。
  printf("\n最终大王是他：%d\n", b);
  return 0;
}