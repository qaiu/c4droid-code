/*c4droid代码手册
 *鸡兔同笼
 *TTHHR编写
 *转载请说明出处
*/
#include <stdio.h>
int main()
{
int chicken,rabbit;//鸡，兔子
int M,N;//头，脚
printf("M=,N=");
scanf("%d %d",&M,&N);
for(chicken=0;chicken<=M;chicken++)//鸡的数量从0到总头数M尝试
{
if(chicken*2+rabbit*4==N)//如果总脚数与输入的总脚数相同
{
printf("%d %d",chicken,rabbit);//分别输出鸡、兔
return 0;//停止程序
}
}
//for执行完表示找不到
printf("0");
return 0;
}
