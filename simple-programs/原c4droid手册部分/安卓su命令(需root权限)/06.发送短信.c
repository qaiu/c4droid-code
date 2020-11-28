/*c4droid代码手册
 *发送短信
 *TTHHR编写
 *转载请说明出处
 请以root身份运行！！
*/
#include<stdlib.h>
int main()
{
system("su -c 'am start -a android.intent.action.SENDTO -d sms:10010 --es sms_body 话费'");
	return 0;
}
