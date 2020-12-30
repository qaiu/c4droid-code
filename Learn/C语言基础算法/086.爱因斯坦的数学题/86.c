/*
例程86. 爱因斯坦的数学题
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/23
*/
#include<stdio.h>
#include <conio.h>
int main()
{
    int i=1;                  /*i为所设的阶梯数*/
    clrscr();
    puts("**************************************************");
    puts("*        This program is to solve                *");
    puts("*     Einstein's interesting math question,      *");
    puts("*   which is presented by Albert Einstein,       *");
    puts("*       a famous theoretical physicist.          *");
    puts("* The Problem is as follows: there is a long     *");
    puts("* ladder, if one step strides 2 stages, 1 stages *");
    puts("* left, if one step strides 3 stages, 2 stages   *");
    puts("* left, if one step strides 5 stages, 4 stages   *");
    puts("* left, if one step strides 7 stages, 0 stages   *");
    puts("* left, the question is, how many stages has     *");
    puts("* the ladder?                                    *");
    puts("**************************************************");
    while(!((i%2==1)&&(i%3==2)&&(i%5==4)&&(i%6==5)&&(i%7==0)))
        ++i;                  /*满足一组同余式的判别*/
    printf("\n >> The ladder has %d stages.\n",i);
    puts("\n Press any key to quit...");
    getch();
    return 0;
}