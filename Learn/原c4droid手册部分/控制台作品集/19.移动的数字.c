/*c4droid代码手册
 *移动的数字
 *冷雨轩258 编写
 *转载请说明出处
*/
#include<stdio.h>
#include<conio.h>
int main()
{
int a,b,i,j,m,n,x,y,p,q,k;
char c[20];
printf("输入要移动的字符;\n");
gets(c);
printf("输入长和宽:\n");
scanf("%d%d",&p,&q);
printf("输入速度(一万多就可以):\n");
scanf("%d",&k);
do
{
for(a=1;a<=q;a++)
{
for(i=0;i<a;i++)
{
printf(" ");
}
 printf("%s\033[?25l\n",c);
usleep(k);
clrscr();
if(a==q)
{
for(b=0;b<=p;b++)
{
for(j=0;j<=b;j++)
{
printf("\n");
for(m=0;m<=q;m++)
printf(" ");
}
printf("%s\033[?25l\n",c);
usleep(k);
clrscr();
if(b==p)
{
for(m=0;m<=p;m++)
{
for(n=0;n<=p;n++)
printf("\n");
for(n=0;n<=q-m;n++)
{
printf(" ");
}
printf("%s\033[?25l\n",c);
 usleep(k);
 clrscr();
 if(m==p)
 {
 for(x=0;x<p;x++)
 {
 for(y=0;y<p-x-1;y++)
 {
 printf("\n");
 }
 printf("%s\033[?25l\n",c);
 usleep(k);
 clrscr();
 }
 }
 }
 }
}
}
 }
 p--;
 q--;
 }
 while(1);
}