/*
例程12. 用一维数组统计学生成绩.c
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
*/
#include <stdio.h>

/**
 * 记录并统计一个班的学生的成绩来
 * 练习一维数组的使用
 */
int main(void)
{
    int count;

    printf("How many students are in your class?\n");
    scanf("%d",&count);  //获取班级中学生的数量

    /**
     * 我在书中看的说是不允许变量赋值来这，但是
     * 在我的尝试中是可以的，不知道怎么回事。
     */
    int numbers[count],chinese[count],maths[count],eng[count];

    puts("Please input the StudentID and three scores:\n");
    printf("      studentID Chinese Math English\n");

    int i = 0;
    /**
     * 接受用户的成绩输入
     */
    for(i = 0;i < count;i++){
        printf("No.%d>",i+1);
        scanf("%d %d %d %d",&numbers[i],&chinese[i],&maths[i],&eng[i]);
    }

    /**
     * 输入完毕之后，输出用户的成绩及其平均成绩
     */
    printf("ID      CH      MA      EN      AVE\n");
    printf("----------------------------------\n");

    int j;
    for(j = 0;j < count;j++){
        float ave = (chinese[j] + maths[j] + eng[j])/3;  //求出平均成绩
        printf("%d\t%d\t%d\t%d\t%f\n",numbers[j],chinese[j],maths[j],eng[j],ave);
    }

    /**
     * 循环求出各科的总成绩
     */
    float ave_chinese = 0,ave_math= 0 ,ave_english = 0;
    int m;
    for(m = 0;m < count;m++){
        ave_chinese += chinese[m];
        ave_math += maths[m];
        ave_english += eng[m];
    }

    //输出平均成绩
    printf("The Average of Chinese in this class is %f\n",ave_chinese/count);
    printf("The Average of math in this class is %f\n",ave_math/count);
    printf("The Average of english in this class is %f\n",ave_english/count);

    return 0;
}