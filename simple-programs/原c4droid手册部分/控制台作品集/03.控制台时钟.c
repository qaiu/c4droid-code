/*c4droid代码手册
 *控制台时钟
 *作者 不一样的井猜
 *转载请说明出处
*/
#include <iostream>
#include <sstream>
#include <ctime>
#include <conio.h>
using namespace std;
const string week[7] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
string gettime();
string gettime_eng();
int main()
{
while (true)
{
clrscr();
cout << "控制台时钟终结者" << endl
 << "Console-Clock-inator" << endl << endl
 << "当前日期时间：" << gettime() << endl
 << "Current date & time: " << gettime_eng() << flush;
usleep(10000);
}
return 0;
}
string gettime()
{
ostringstream sst;
time_t t;
time(&t);
tm * tn = localtime(&t);
sst << tn->tm_year + 1900 << " 年 "
 << tn->tm_mon + 1 << " 月 "
 << tn->tm_mday << " 日 "
 << week[tn->tm_wday] << ' '
 << ((tn->tm_hour >= 10) ? "" : "0") << tn->tm_hour << ':'
 << ((tn->tm_min >= 10) ? "" : "0") << tn->tm_min << ':'
 << ((tn->tm_sec >= 10) ? "" : "0") << tn->tm_sec;
return sst.str();
}
string gettime_eng()
{
time_t t;
time(&t);
tm * tn = localtime(&t);
return string(asctime(tn));
}