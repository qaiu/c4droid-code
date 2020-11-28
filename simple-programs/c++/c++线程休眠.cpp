// this_thread::sleep_for example
#include <iostream> // std::cout, std::endl
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
int main()
{
	using namespace std;
std::cout << "countdown:\n";
for (int i=10; i>0; --i) {
std::cout << i << std::endl;
std::this_thread::sleep_for (1s);
}
std::cout << "Lift off!\n";
return 0;
}