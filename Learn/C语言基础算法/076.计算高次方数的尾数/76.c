/*
例程76. 计算高次方的尾数
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/22
*/

#include<stdio.h>
#include <conio.h>
int main()
{
    int i,x,y,last=1;    /*变量last保存求X的Y次方过程中的部分乘积的后三位*/
    clrscr();
    puts("**********************************************************");
    puts("*  This is a program to calculate the last 3 digits of   *");
    puts("*              high order value, e.g., 13^15.            *");
    puts("**********************************************************");
    printf("\n >> Input X and Y(X^Y): ");
    scanf("%d%d",&x,&y);
    for(i=1;i<=y;i++)                /*X自乘Y次*/
        last=last*x%1000;     /*将last乘X后对1000取模，即求积的后三位*/
    printf("\n >> The last 3 digits of %d^%d is: %d\n",x,y,last%1000); /*打印结果*/
    puts("\n >> Press any key to quit...");
    getch();
    return 0;
}