/*
 *  触控.c[v1.1]
 *  测试终端点击滚动反馈
 *  运行前请打开终端的鼠标事件反馈
 *  by 白狼·圣狂
 */


#include "../screen.h"


int main()
{
	C4S_Suite suite;
	char str[80] = {};
	int count;
	int value;
	int num;
	
	// 初始化
	C4S_Init();
	
	// 关闭输入回显
	// 打开触摸反馈
	C4S_CloseInputEcho();
	C4S_OpenTouchEcho();
	
	while (1)
	{
		// 获取一次输入
		if (count = C4S_Input(str, 80, 0, 0))
		{
			value = 0;
			
			// 解析反馈信息
			while (num = C4S_SplitSuite(str + value, count - value, &suite))
			{
				value += num;
				
				// 输出信息
				switch (suite.type)
				{
					case C4S_SUITE_CHUCK:
						if (suite.chuck.state == C4S_CHUCK_PRESSED)
							printf("按下:");
						else
							printf("弹起:");
						printf("(%d,%d)\n", suite.chuck.x, suite.chuck.y);
						break;
						
					case C4S_SUITE_SCROLL:
						if (suite.scroll.direction == C4S_SCROLL_UP)
							printf("上滚:");
						else
							printf("下滚:");
						printf("(%d,%d)\n", suite.scroll.x, suite.scroll.y);
						break;
						
					default:
						break;
				}
			}
		}
	}
	
	// 关闭点击滚动反馈
	// 打开输入回显
	C4S_CloseTouchEcho();
	C4S_OpenInputEcho();
	
	// 退出
	C4S_Quit();
	
	return 0;
}