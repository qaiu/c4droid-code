// 
// main.c
// 素数幻方
// 
// Created by Fred Liang on 2016/12/23.
// Copyright © 2016年 Fred Liang. All rights reserved.
// 

#include <stdio.h>
#include <math.h>

int triple(int a, int b, int c, int d);
void save_num(int *b);
int re(int k);
int whether(int i);
int turn1(int a);
void devide(int *devide, int a);
void none22(int *none2, int *b);

int main() {
  int a[4][4], b[204], none2[22], c[4], i, k, l, m, devide1[4], o = 0, num = 1;
  save_num(b);
  none22(none2, b);
  for (c[0] = 0; c[0] < 22; c[0]++) {
    for (c[1] = 0; c[1] < 204; c[1]++) {
      for (c[2] = 0; c[2] < 204; c[2]++) {
        for (c[3] = 0; c[3] < 22; c[3]++) {
          for (i = 0; i < 4; i++) {
            if (i == 1 || i == 2) {
              devide(devide1, b[c[i]]);
            } else {
              devide(devide1, none2[c[i]]);
            }
            a[i][0] = devide1[0];
            a[i][1] = devide1[1];
            a[i][2] = devide1[2];
            a[i][3] = devide1[3];
          }

          for (k = 0; k < 4; k++) {
            if ((turn1((triple(a[k][0], a[k][1], a[k][2], a[k][3]))) == 0)
                && (turn1((triple(a[0][k], a[1][k], a[2][k], a[3][k]))) == 0)
                && (turn1((triple(a[0][0], a[1][1], a[2][2], a[3][3]))) == 0)
                && (turn1((triple(a[3][0], a[2][1], a[1][2], a[0][3]))) ==
                    0)) {
              o = o + 1;
            }

          }
          if (o == 4) {
            printf("%d\n", num++);
            for (l = 0; l < 4; l++) {
              for (m = 0; m < 4; m++) {
                printf("%d ", a[l][m]);
              }
              printf("\n");
            }
            printf("\n\n");
          }
          o = 0;
        }
      }
    }

  }
  return 0;
}

int triple(int a, int b, int c, int d) {
  int sum;
  sum = a * 1000 + b * 100 + c * 10 + d;
  return sum;
}
void save_num(int *b) {
  int i, j = 0;
  for (i = 0; i < 204; i++) {
    b[i] = 0;
  }
  for (i = 1000; i < 10000; i++) {
    if (turn1(i) == 0) {
      b[j] = i;
      j++;
    }
  }
}
void devide(int *devide, int a) {
  int i = 0;
  for (i = 3; i > -1; i--) {
    devide[i] = a % 10;
    a = a / 10;
  }
}
int turn1(int a) {
  if (whether(a) == 1)
    if (whether(re(a)) == 1)
      return 0;

  return 1;
}
int re(int k) {
  int i, j, a[5];
  for (i = 0; i < 5; i++) {
    a[i] = k % 10;
    k = k / 10;
    if (k == 0)
      break;
  }
  k = 0;
  for (j = 0; j <= i; j++) {
    k = k * 10 + a[j];
  }
  return k;
}

int whether(int i) {
  int j, b;
  b = sqrt(i) + 1;
  for (j = 2; j <= b; j++) {
    if ((i % j) == 0)
      break;
    if (j == b)
      return 1;
  }
  return 0;
}
void none22(int *none2, int *b) {
  int i = 0, a[4], j = 0;
  for (i = 0; i < 204; i++) {
    devide(a, b[i]);
    if ((a[1] % 2 != 0) && (a[0] % 2 != 0) && (a[2] % 2 != 0)
        && (a[3] % 2 != 0) && (a[1] != 5) && (a[0] != 5) && (a[2] != 5)
        && (a[3] != 5)) {
      none2[j] = b[i];
      j++;
    }
  }
}