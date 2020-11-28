/*c4droid代码手册
 *获取当前时间
 *TTHHR编写
 *转载请说明出处
*/
#include<stdio.h>
#include<time.h>
int main() {
 time_t t=time(0);
 printf("%s\n", ctime(&t));
 return 0;
}
/*伪动态时间
*天空上的河编写
#include<stdio.h>
#include<time.h>
#include<conio.h>
int main() {
 while (1) {
 time_t t = time(0);
 printf("%s\n", ctime(&t));
 sleep(1);
 clrscr();
 }
 return 0;
}*/
