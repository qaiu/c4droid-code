/*c4droid代码手册
 *暂停和清屏
 *TTHHR编写
 *转载请说明出处
*/
#include<conio.h>/*clrscr(),sleep()所属头文件*/
#include<stdio.h>
int main()
{
printf("我来过\n");
sleep(1);/*暂停一秒*/
clrscr();/*清屏*/
sleep(1);
printf("我又来了，马上就走\n");
sleep(1);
clrscr();
return 0;
}
