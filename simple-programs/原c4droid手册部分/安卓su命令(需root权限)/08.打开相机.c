/*c4droid代码手册
 *打开相机
 *TTHHR编写
 *转载请说明出处
 请以root身份运行！！
*/
#include<stdlib.h>
int main()
{
	system("am start -n com.android.camera/com.android.camera.Camera");
	//4.0及以上
//	system("am start -n com.android.gallery3d/com.android.camera.Camera");
	return 0;
}