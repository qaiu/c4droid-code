/*c4droid代码手册
 *定时关机
 *作者 iamhuyulong1
 *转载请说明出处
*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
int
main ()
{
pid_t pid;
if ((pid = fork ()) < 0)
{
return -1;
}
else if (pid != 0)
{
{
int a, b, c, d, e, f, f1, f2;
printf
("请分别输入小时、分钟、秒，并以回车键确认。\n");
printf ("小时:");
scanf ("%d", &a);
printf ("分钟:");
scanf ("%d", &b);
printf ("秒:");
scanf ("%d", &c);
system("su -c 'input keyevent 6'");
d = a * 3600 + b * 60 + c;
for (e = 0; e < d; e++)
{
sleep (1);
clrscr ();
f = (d - e) / 3600;
f1 = ((d - e) % 3600) / 60;
f2 = ((d - e) % 3600) % 60;
printf
("距离关机还有%02d小时%02d分钟%02d秒。\n",
 f, f1, f2);
}
system ("su -c 'reboot -p'");
}
 exit (0);
}
setsid ();
if ((pid = fork ()) != 0)
{
exit (0);
}
else if (pid < 0)
{
return -1;
}
close (0);
close (1);
close (2);
return (0);
}
//话说最后那几个close我也不知道什么用
