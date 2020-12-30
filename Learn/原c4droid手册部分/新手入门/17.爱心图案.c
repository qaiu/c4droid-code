/*c4droid代码手册
 *爱心
 *啊o额iu鱼编写
 *TTHHR收集整理
 *转载请说明出处
*/
 # include <stdio.h>
# include <math.h>
#include <conio.h>
# define U 0.1
# define V 0.063
# define M 1.1
# define N 1.2
int main(void)
{
float x, y;
float m, n;
for (y = 1.3; y >= -1.3; y -= U)
{
for (x = -1.2; x <= 1.4; x += V)
{
if ((((x * x + y * y - 1) * (x * x + y * y - 1) * (x * x + y * y -
 1) -
 x * x * y * y * y) <= 0))
printf("*");
else
printf(" ");
fflush(stdout);
usleep(1000 * 5);
}
printf("\n");
}
return 0;
}
