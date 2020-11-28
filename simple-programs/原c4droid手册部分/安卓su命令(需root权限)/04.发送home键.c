/*c4droid代码手册
 *发送home键，后台运行
 *TTHHR编写
 *转载请说明出处
*/
#include<stdlib.h>
int main()
{
	system("su -c 'input keyevent 3'");
	system("su -c 'input swipe 540 0 540 540'");
//	system("su -c 'input keyevent 3'");
//	system("su -c 'input keyevent 3'");
	
	return 0;
}