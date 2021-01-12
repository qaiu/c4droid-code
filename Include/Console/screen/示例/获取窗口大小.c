/*
 *  获取窗口大小.c[v1.0]
 *  测试获取窗口大小的两种方法
 *  by 白狼·圣狂
 */


#include <stdlib.h>
#include "../screen.h"


// 回调函数
void hook(int w, int h)
{
	printf("[hook]窗口大小被改变:[%d,%d]\n", w, h);
}

void GetSize()
{
	int w, h;
	
	// 获取屏幕大小
	C4S_GetScreenSize(&w, &h);
	
	printf("[GetSize]窗口大小为[%d,%d]\n", w, h);
}

int main()
{
	// 初始化
	C4S_Init();
	
	// 设置回调函数
	C4S_SetScreenSizeHook(hook);
	
	// 获取大小
	GetSize();
	
	// 暂停程序，避免进程被杀
	system("pause");
	printf("程序结束\n");
	
	// 退出
	C4S_Quit();
	
	return 0;
}