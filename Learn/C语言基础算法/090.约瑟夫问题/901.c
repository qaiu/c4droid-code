/*
例程90. 猴子选王(约瑟夫问题)
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/23
*/

#include <stdio.h>
#include <malloc.h>
#define LEN sizeof(struct monkey) // 定义structmonkey这个类型的长度
struct monkey {
  int num;
  struct monkey *next;
};
struct monkey *create(int m) {
  struct monkey *head, *p1, *p2;
  int i;
  p1 = p2 = (struct monkey *)malloc(LEN);
  head = p1;
  head->num = 1;
  for (i = 1, p1->num = 1; i < m; i++) {
    p1 = (struct monkey *)malloc(LEN);
    p1->num = i + 1;
    p2->next = p1;
    p2 = p1;
  }
  p2->next = head;
  return head;
}
struct monkey *findout(struct monkey *start, int n) {
  int i;
  struct monkey *p;
  i = n;
  p = start;
  for (i = 1; i < n - 1; i++)
    p = p->next;
  return p;
}
struct monkey *letout(struct monkey *last) {
  struct monkey *out, *next;
  out = last->next;
  last->next = out->next;
  next = out->next;
  free(out);
  return next;
}
int main() {
  int m, n, i, king;
  struct monkey *p1, *p2;
  printf("请输入猴子的个数m:\n");
  scanf("%d", &m);
  printf("每次数猴子的个数n:\n");
  scanf("%d", &n);
  if (n == 1) {
    king = m;
  } else {
    p1 = p2 = create(m);
    for (i = 1; i < m; i++) {
      p2 = findout(p1, n);
      p1 = p2;
      p2 = letout(p1) ;
      p1 = p2;
    }
    king = p2->num;
    free(p2);
  }
  printf("猴王的编号是：%d\n", king);
  return 0;
}