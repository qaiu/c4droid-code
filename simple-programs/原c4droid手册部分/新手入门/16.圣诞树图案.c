/*
c4droid代码手册
圣诞树
天空上的河 编写
*/

#include<stdio.h>
#include<conio.h>

void  delay() {
   int  i = 0, j = 0;
   for (i = 0; i < 1000; i++)
     for (j = 0; j < 15000; j++);
}
int  main() {
  int  i = 1, j = 0, y = 1, x = 25;
  while  (i < 6) {
    gotoxy(x--, 3 + y++);
     for  (j = 0; j <= i; j++)
      printf("\33[42;20m  \33[40;37m%");
     printf("\n");
     delay();
     i++;
  }
  {
     int  i = 0, j = 0;
     for (i = 0; i < 1000; i++)
       for (j = 0; j < 15000; j++);
  }
  {
    int  i = 1, j = 0, y = 1, x = 25;
    while  (i < 10) {
      gotoxy(x--, 6 + y++);
       for  (j = 0; j <= i; j++)
        printf("\33[42;20m  \33[40;37m%");
       printf("\n");
       delay();
       i++;
    }
  }
  {
     int  i = 0, j = 0;
     for (i = 0; i < 1000; i++)
       for (j = 0; j < 15000; j++);
  }
  {
    int  i = 1, j = 0, y = 1, x = 25;
    while  (i < 15) {
      gotoxy(x--, 10 + y++);
       for  (j = 0; j <= i; j++)
        printf("\33[42;20m  \33[40;37m%");
       printf("\n");
       delay();
       i++;
    }
  }
  {
     int  i = 0, j = 0;
     for (i = 0; i < 1000; i++)
       for (j = 0; j < 15000; j++);
  }
  {
    int  i = 1, j = 0, y = 1, x = 25;
    while  (i < 10) {
      gotoxy(x, 23 + y++);
       for  (j = i; j <= i; j++)
        printf("\33[42;41m  \33[42;41m%");
       printf("\n");
       delay();
       i++;
    }
  }
  {
     int  i = 0, j = 0;
     for (i = 0; i < 1000; i++)
       for (j = 0; j < 15000; j++);
  }
  {
    int  i = 1, j = 0, y = 1, x = 27;
    while  (i < 10) {
      gotoxy(x, 23 + y++);
       for  (j = i; j <= i; j++)
        printf("\33[42;41m  \33[42;41m%");
       printf("\n");
       delay();
       i++;
    }
  }
  gotoxy(22, 20);
   printf("\33[42;41m圣诞节快乐\n");
  gotoxy(17, 21);
  printf("来自天空上的河的祝福\n\33[42;41m%");

}