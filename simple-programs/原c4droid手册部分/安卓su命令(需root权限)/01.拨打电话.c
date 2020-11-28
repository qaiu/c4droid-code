/*c4droid代码手册
 *拨打电话
 *TTHHR编写
 *转载请说明出处
 请以root身份运行！！
*/
#include<stdlib.h>
int main()
{
	system("su -c 'service call phone 2 s16 10010'");
	return 0;
}
