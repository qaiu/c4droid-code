#include <iostream>
#include <chrono>
 
int main()
{
            // 记录开始时间
        auto start = std::chrono::system_clock::now();
        for (int i=0;i<1000000000;i++);
        auto end = std::chrono::system_clock::now();
 
        std::cout <<  std::chrono::duration<double> (end-start).count();
}