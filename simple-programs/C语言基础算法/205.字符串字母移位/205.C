/*
例程205. 字符串字母移位
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <stdio.h>
#define  N 81
void chg(char *s) {
  while (*s)
    if (*s == 'z' || *s == 'Z') {
      *s -= 25;
      s++;
    } else if (*s >= 'a' && *s <= 'y') {
      *s += 1;
      s++;
    } else if (*s >= 'A' && *s <= 'Y') {
      *s += 1;
      s++;
    } else
      s++;
}

int main() {
  char a[N];

  printf("Enter a string : ");
  gets(a);
  printf("The original string is : ");
  puts(a);
  chg(a);
  printf("The string after modified : ");
  puts(a);
}