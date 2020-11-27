/*
例程196. 字符串逆置
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/
#include <stdio.h>
#define N 81 

char* strrev(char* s)
{
    /* h指向s的头部 */
    char* h = s;    
    char* t = s;
    char ch;

    /* t指向s的尾部 */
    while(*t++){};
    t--;    /* 与t++抵消 */
    t--;    /* 回跳过结束符'\0' */
    
    /* 当h和t未重合时，交换它们所指向的字符 */
    while(h < t)
    {
        ch = *h;
        *h++ = *t;    /* h向尾部移动 */
        *t-- = ch;    /* t向头部移动 */
    }
    return s;
}

int main( ) 
{
	char a[N]; 
    printf ( "Enter a string : " ); gets ( a ); 
    printf ( "The original string is : " ); puts( a ); 
    strrev( a ); 
    printf("\n"); 
    printf ( "The string after modified : "); 
    puts ( a ); 
}