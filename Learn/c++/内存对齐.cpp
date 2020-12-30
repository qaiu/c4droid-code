#include <iostream>

using std::cout;
using std::endl;
/*对内存对齐的深入研究*/

/*
1.#pragma pack(n)设置编译器内存对齐值为n
n只能取1,2,4,8,16如果n取到了其他值一律按16处理
#pragma pack()取消对齐设置，恢复默认对齐值8
*/

/*
对齐包含两部分
结构体成员自身对齐，和结构体整体对齐

数据成员对齐规则：
结构(struct)(或联合(union))的数据成员，
第一个数据成员放在offset为0的地方，
以后每个数据成员的对齐按照
#pragma pack指定的数值和这个数据成员自身长度中，比较小的那个进行。

结构(或联合)的整体对齐规则：
在数据成员完成各自对齐之后，
结构(或联合)本身也要进行对齐，
对齐将按照
#pragma pack指定的数值和结构(或联合)最大数据成员长度中，
比较小的那个进行。

对齐过程:
n是设定的对齐值

整体对齐系数 = min((max(type1,type2,...), n);
整体大小(size)=$(成员总大小(ps:成员自身对齐后相加的大小) ) 按 $(整体对齐系数) 圆整

圆整

什么是“圆整”？
举例说明：如8字节对齐中的“整体对齐”，整体大小=9 按 4 圆整 = 12
圆整的过程：从9开始每次加一，
看是否能被4整除，这里9，10，11均不能被4整除，
到12时可以，则圆整结束。

假设成员大小均小于设定对齐值
1)结构体变量的首地址是其最长基本类型成员的整数倍；

备注：编译器在给结构体开辟空间时，首先找到结构体
中最宽的基本数据类型，然后寻找内存地址能是该基本
数据类型的整倍的位置，作为结构体的首地址。将这个
最宽的基本数据类型的大小作为上面介绍的对齐模数。

2)结构体每个成员相对于结构体首地址的偏移量
（offset）都是成员大小的整数倍，如有需要编译器
会在成员之间加上填充字节（internal adding）；
备注:为结构体的一个成员开辟空间之前，编译器首
先检查预开辟空间的首地址相对于结构体首地址
的偏移是否是本成员的整数倍，若是，则存放本成员，
反之，则在本成员和上一个成员之间填充一定的字节，
以达到整数倍的要求，也就是将预开辟空间的首地址
后移几个字节。

3)结构体的总大小为结构体最宽基本类型成员
大小的整数倍，如有需要，编译器会在最末一个成
员之后加上填充字节（trailing padding）。

备注：
a、结构体总大小是包括填充字节，最后一个成
员满足上面两条以外，还必须满足第三条，否则
就必须在最后填充几个字节以达到本条要求。

b、如果结构体内存在长度大于处理器位数的元素，
那么就以处理器的倍数为对齐单位；否则，如果
结构体内的元素的长度都小于处理器的倍数的时候，
便以结构体里面最长的数据元素为对齐单位。

4) 结构体内类型相同的连续元素将在连续的空间内，
和数组一样。
*/
#pragma pack(1)
struct Test {
    char c;     // 1 byte
    double d;   // 8 byte
    int i;      // 4 byte
    short s;    // 2 byte
    float f;    // 4 byte
};
#pragma pack()

#pragma pack(2)
struct Test2 {
    char c;     // 1 byte
    double d;   // 8 byte
    int i;      // 4 byte
    short s;    // 2 byte
    float f;    // 4 byte
};
#pragma pack()


#pragma pack(4)
struct Test4 {
    char c;     // 1 byte
    double d;   // 8 byte
    int i;      // 4 byte
    short s;    // 2 byte
    float f;    // 4 byte
};
#pragma pack()

#pragma pack(8)
struct Test8 {
    char c;     // 1 byte
    long double d;   // 8 byte
    int i;      // 4 byte
    short s;    // 2 byte
    float f;    // 4 byte
};
#pragma pack()


#pragma pack(16)
struct Test16 {
    char c;     // 1 byte
    double d;   // 8 byte
    int i;      // 4 byte
    short s;    // 2 byte
    float f;    // 4 byte
};
#pragma pack()

int main() 
{
    //Test结构体以1字节对齐,总大小为1的倍数 因此为所有成员大小之和即可无需填充
    Test test;

    printf("Test 以%ld byte 对齐\n",alignof(Test));
    printf("Test结构体大小: %ld\n",sizeof(Test));
    printf("Test.c address:  %p\n",&test.c);
    printf("Test.d address:  %p\n",&test.d);
    printf("Test.i address:  %p\n",&test.i);
    printf("Test.s address:  %p\n",&test.s);
    printf("Test.f address:  %p\n",&test.f);
    //Test结构体以2字节对齐,总大小必须为2的倍数,成员与成员填充之和为(可以通过地址相减加尾元素大小) 20为2的倍数无需填充
    Test2 test2;    
    printf("Test2 以%ld byte 对齐\n",alignof(Test2));
    printf("Test2结构体大小: %ld\n",sizeof(Test2));
    printf("Test2.c address:  %p\n",&test2.c);
    printf("Test2.d address:  %p\n",&test2.d);
    printf("Test2.i address:  %p\n",&test2.i);
    printf("Test2.s address:  %p\n",&test2.s);
    printf("Test2.f address:  %p\n",&test2.f);
    //Test结构体以8字节对齐,总大小必须为8的倍数,成员与成员填充之和为 27不为8的倍数,继续填充直到32
    Test8 test8;    
    printf("Test8 以%ld byte 对齐\n",alignof(Test8));
    printf("Test8结构体大小: %ld\n",sizeof(Test8));
    printf("Test8.c address:  %p\n",&test8.c);
    printf("Test8.d address:  %p\n",&test8.d);
    printf("Test8.i address:  %p\n",&test8.i);
    printf("Test8.s address:  %p\n",&test8.s);
    printf("Test8.f address:  %p\n",&test8.f);
    return 0;
}