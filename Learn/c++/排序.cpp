#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

#define MAX 10000000 //一千万int，大约需要30多m堆空间

int main()
{
	using namespace std;

	array<int,MAX> *a = new array<int,MAX>;//栈的大小只有8m，不new的话会溢出报段错误
	
	default_random_engine random(time(NULL));//初始化随机数引擎
    uniform_int_distribution<int> dis1(0, 999999999);//定义一个分布区间

    for (auto &i : *a)
        i = dis1(random);
    
	sort(a->begin(),a->end(),[](int a,int b){return a>b;});//定义排序规则(从大到小)
	
	for (int i = 0; i < 1000; i++) { //输出前1000个验证，全部输出会卡，可以考虑输出到文件
		cout << (*a)[i]<<endl;
	}
}