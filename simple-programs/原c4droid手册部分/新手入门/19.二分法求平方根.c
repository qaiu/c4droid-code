/*c4droid代码手册
*二分法求平方根
*作者未知
*转载请说明出处
*/

#include <stdio.h> 
#define eps 0.0001
double sqrt(double n)
{
    /*处理异常*/
    if(n<0)
    return n;
    
    /*定义参数*/
    double ans;
    double low,up;
    
    /*二分*/
    low=0,up=n; 
    ans=(low+up)/2; 
    
    /*条件*/
    while(ans*ans-n>eps || ans*ans-n<-eps) 
    {
        /*数字过大*/
        if(ans*ans>n) 
        up=ans; 
        /*数字过小*/
        else 
        low=ans;
        /*再来一次*/
        ans=(up+low)/2; 
    } 
    return ans; 
} 

int main() 
{
    double num ;
    scanf("%lf",&num);
    printf("%f",sqrt(num));
    return 0;
}