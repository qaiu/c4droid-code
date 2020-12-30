// 约瑟夫问题3: 用数组模拟链表
#include<stdio.h>
#include<malloc.h>
int main() {
  int *person, i, node, n, m;
  scanf("%d%d", &n, &m);
  person = (int *)malloc(sizeof(int) * (n + 1));
  for (i = 1; i <= n; i++)      // 初始化圈
  {
    person[i] = i + 1;          // i表示编号为i的人,person[i]的值表示编号为i的人的下一个人的编号
  }
  person[n] = 1;                // 编号为n的下一个人的编号是1
  node = 1;
  while (node != person[node])  // 如果某个人的下一个人不是自己,意味着人数超过1人
  {
    for (i = 1; i < m - 1; i++) // 这个循环终止于被杀的人的前一个人
    {
      node = person[node];      // 下一个人的编号为node,node的值来自于前一个人的person[node]
    }
    printf("%d ", person[node]); // 输出被杀的人编号
    person[node] = person[person[node]]; // 修改被杀的人的前一个人的person[node]为被杀的人的后一个人的编号
    node = person[node];        // 这句话中的node是被杀的人后一个人
  }
  printf("%d ", node);          // 输出最后幸存者的编号
  return 0;
}