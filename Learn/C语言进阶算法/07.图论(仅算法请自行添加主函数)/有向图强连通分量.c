/* 有向图强连通分量 */
#include <string.h>
#include <stdio.h>

const int MAXINT = 2000000000;
const int N = 10000;
const int M = 50000;
int head[N], set[N], post[N], head2[N];
int no[M], next[M], no2[M], next2[M];
/* 可多次重复利用 */
bool vst[N];
/* cnt 也是这样 */
int n, m, cnt;

inline void read(int &data) {
  char ch = getchar();
  while (ch < '0' || ch > '9')
    ch = getchar();
  data = 0;
  do{
    data = data*10 + ch-'0';
    ch = getchar();  
  }while (ch >= '0' && ch <= '9');
}

void init() {
  int i, a, b;
  scanf("%d%d", &n, &m);
  memset(head, -1, sizeof(head));
  memset(head2, -1, sizeof(head2));
  for(i = 0; i < m; i++) {
    read(a); read(b);
    a--; b--;
    no[i] = b;
    next[i] = head[a];
    head[a] = i;
  }
}

/* 有向图的DFS 要分成多步进行dfs 这里没有判连通 因为不连通一定有2个以上的极大强连通子图 */
void DFS(int x) {
  vst[x] = 1;
  int p = head[x];
  for(; p >= 0; p = next[p])
    if(!vst[no[p]])
      DFS(no[p]);
  /* 我们只需要按照完成时间排序即可 并不需要严格按照结束时间 */
  post[cnt] = x; 
  cnt++;
}

void reverse() {
  int i, t = 0, p, j = 0;
  for(i = 0; i<n; i++) {
    for(p = head[i]; p >= 0; p = next[p]) { //i->no[p]
      no2[j] = i;
      next2[j] = head2[no[p] ];
      head2[no[p] ] = j++;
    }
  }
}

void DFS2(int x) {
  vst[x] = 1;
  set[x] = cnt;
  int p;
  for(p = head2[x]; p >= 0; p = next2[p]) 
    if(!vst[no2[p] ])
      DFS2(no2[p]);
}

void work() {
  int i, j;
  memset(vst, 0, sizeof(vst));
  /* cnt充当了记录时间的作用 */
  cnt = 0; 
  for(i = 0; i<n; i++) 
    if(!vst[i])
      DFS(i);
  reverse();
  memset(vst, 0, sizeof(vst));
  cnt = 0;
  for(i = n-1; i>=0; i--)
    if(!vst[post[i]]) { 
      DFS2(post[i]);
      /* 此处作为scc的id值使用 */
      cnt++;
    }

    /* 去除错误的scc */
    memset(vst, 0, sizeof(vst));
    int p;
    for(i = 0; i<n; i++) 
      if(!vst[set[i]]) 
        for(p = head[i]; p >= 0; p = next[p]) 
          if(set[no[p] ] != set[i]) {
            vst[set[i] ] = true;
            /* 有引申出去的点 则当前scc错误！ */
            break;
          }

          j = -1;
          for(i = 0; i<cnt; i++) 
            if(!vst[i]) {
              if(j >= 0) {
                /* 2连通分量 */
                printf("0\n");
                return;
              }
              j = i;
            }
            cnt = 0;
            for(i = 0; i<n; i++) 
              if(set[i] == j)
                cnt++;
            printf("%d\n", cnt);
}

int main() {
  init();
  work();
  return 0;
}