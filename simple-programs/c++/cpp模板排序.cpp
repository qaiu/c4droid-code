#include<iostream>
#include<string>

using namespace std;

template <class T> 
void pop_sort(T &a1)
{
	int length = sizeof(a1) / sizeof(a1[0]);	// 计算数组长度
	int i, j;
//	cout<<length<<endl;
	for (i = 0; i < length - 1; i++)		// 通过冒泡排序进行排序
	{
		for (j = 0; j < length - 1 - i; j++)
		{
			if (a1[j] > a1[j + 1])
			{
				auto form = a1[j];
				a1[j] = a1[j + 1];
				a1[j + 1] = form;
			}
		}
	}

}

int main()
{
	string a1[5];
	cout<<"请输入5个字符串"<<endl;
	cin >> a1[0] >> a1[1] >> a1[2] >> a1[3] >> a1[4];
	cout<<"排序后"<<endl;
	pop_sort(a1);
	
	for (int i = 0; i < 5; i++)
	{
		cout << a1[i] << endl;
	}
	
	int a[6];
	cout<<"请输入5个数字"<<endl;
	cin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4];
	cout<<"排序后"<<endl;
	pop_sort(a);
	
	for (int i = 0; i < 5; i++)
	{
		cout << a[i] << endl;
	}
	
	system("pause");
	return 0;
}