/*
例程172. 删除字符串中的特定字符
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/

#include <stdio.h>
#include <conio.h>
int func(char s[],int c)
{
    char *q=s;
    for(; *q; q++)
    if(*q != c) *(s++)=*q;
    *s='\0';
}
int main()
{
    static char str[]="  this is C4droid and qeditor for gcc or g++";
    char ch;
    clrscr() ;	printf(" 原字符串为 \n%s.\n",str);
    printf(" 请输入要删除的字符: ");	
    scanf("%c",&ch);
    func(str,ch);
    printf(" 删除后的字符串为 \n%s.\n",str);
    printf(" 任意键退出...");
    getch();
    return 0;
}