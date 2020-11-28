/*c4droid代码手册
 *控制声音大小//需要在控制台设置control键
 *TTHHR编写
 *转载请说明出处
*/
#include<stdlib.h>
int main()
{
	system("su -c 'input keyevent VOLUME_UP'");//DOWN是音量减
	return 0;
}
