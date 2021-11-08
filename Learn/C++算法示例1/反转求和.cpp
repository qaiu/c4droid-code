#include<iostream>
#include<cmath>
using namespace std;
int reverse(int n)
{
	int c=0;
	while(n)
	{
		c+=n%10;
		c*=10;
		n/=10;
	}
	return c/10;
	
}
int Add(int a, int b)
{
  return(reverse(a) + reverse(b));
}
main() {
  int a, b;
  cin >> a >> b;
  cout << Add(a, b) << endl;
  return 0;
}