#include <iostream>
using namespace std;

/*
//js闭包示例
function foo(x) {
    var tmp = 3;
    return function (y) {
        alert(x + y + (++tmp));
    }
}
var bar = foo(2); // bar 现在是一个闭包
bar(10); //输出16
*/

auto foo(int x)//auto返回值，仅限于C++14
{
	auto tmp = 3;
	return[=] (int y) mutable//lambda函数，c++11
	{
		cout << x + y + (++tmp) << endl;
	};
}

/*
lambda函数(匿名函数)一般形式:
[扑捉块](参数表<可选>)mutable<可选>->返回值类型<可选>{函数体}

*/

int main()
{
	auto c = foo(2);
	c(10);
	c(10);

	//一般形式
	auto foo2=[](int x)
	{
		return[=] ()mutable
		{
			return x++;
		};
	};
	
	auto f=foo2(2);
	cout << f() << endl;
	cout << f() << endl;
	
	auto d=[]{cout<<"Hello"<<endl;};
	d();
}