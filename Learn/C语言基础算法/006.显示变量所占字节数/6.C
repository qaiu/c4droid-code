/*
例程6. 显示变量所占字节数
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
最后修改时间:2017/10/20
*/
/* 输出不同类型所占的字节数*/
#include <stdio.h>
int main()
{
    /* sizeof()是保留字，它的作用是求某类型或某变量类型的字节数, */
    /* 括号中可以是类型保留字或变量。*/
    /*int型在不同的机器，不同的编译器中的字节数不一样,*/
    /*一般来说在TC2.0编译器中字节数为2,在VC编译器中字节数为4 */
    printf("The bytes of the variables are:\n");
    printf("int:%d bytes\n",sizeof(int));
    /* char型的字节数为1 */
    printf("char:%d byte\n",sizeof(char));
    /* short型的字节数为2 */
    printf("short:%d bytes\n",sizeof(short));
    /* long型的字节数为4 */
    printf("long:%d bytes\n",sizeof(long));
    /* float型的字节数为4 */
    printf("float:%d bytes\n",sizeof(float));
    /* double型的字节数为8 */
    printf("double:%d bytes\n",sizeof(double));
    /* long double型的字节数为8或10或12 ，在64位的GCC7.2中为16*/
    printf("long double:%d bytes\n",sizeof(long double));
    /*任意指针类型在64位的GCC7.2中为8*/
    printf("void* pointer:%d bytes\n",sizeof(void *));
    /*utf-8字符一个汉字的字节数为3+1 \0占用1byte*/
    printf("utf-8汉字'度':%d bytes\n",sizeof("度"));
    getchar();
}