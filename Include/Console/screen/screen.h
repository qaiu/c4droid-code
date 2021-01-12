/*
 *  screen.h[v2.0]
 *  获取终端屏幕的信息
 *  by 白狼·圣狂
 */

/*
 *  防重包含举措
 */
#ifndef _C4DROID_SCREEN_H
#define _C4DROID_SCREEN_H


#ifndef __ANDROID__
#warning 为C4droid量身定制的头文件，建议仅在C4droid编译与运行
#endif


/*
 *  与c++兼容
 */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  点击和滚动转义序列类型
 */
typedef enum C4S_TOUCHSUITETYPE
{
	C4S_SUITE_UNKNOWN,  // 未知
	C4S_SUITE_CHUCK,    // 触控点击
	C4S_SUITE_SCROLL    // 触控滚动
}C4S_TouchSuiteType;


/*
 *  按压状态类型
 */
typedef enum C4S_TOUCHCHUCKSTATETYPE
{
	C4S_CHUCK_UNKNOWN,  // 未知
	C4S_CHUCK_RELEASED, // 松开
	C4S_CHUCK_PRESSED   // 按下
}C4S_TouchChuckStateType;


/*
 *  滚动方向类型
 */
typedef enum C4S_SCROLLDIRECTIONTYPE
{
	C4S_SCROLL_UNKNOWN, // 位置
	C4S_SCROLL_UP,      // 向上滚动
	C4S_SCROLL_DOWN     // 向下滚动
}C4S_ScrollDirectionType;


/*
 *  序列联合体
 */
typedef union C4S_SUITE
{
	// 序列类型
	C4S_TouchSuiteType type;
	
	// 点击序列
	struct C4S_CHUCK
	{
		// 序列类型
		C4S_TouchSuiteType type;
		
		// 点击坐标(以字为单位)
		int x, y;
		
		// 事件状态
		C4S_TouchChuckStateType state;
	}chuck;
	
	// 滚动序列
	struct C4S_SCROLL
	{
		// 序列类型
		C4S_TouchSuiteType type;
		
		// 拖动开始坐标(以字为单位)
		int x, y;
		
		// 滚动方向
		C4S_ScrollDirectionType direction;
	}scroll;
}C4S_Suite;


/*
 *  包含需要的头文件
 */
#include "screen.c"


/*
 *  初始化，使用前请调用
 *  失败返回-1
 */
extern int C4S_Init( void );


/* 
 *  与初始化相反，不再使用时调用
 *  失败返回-1
 */
extern int C4S_Quit( void );


/*
 *  获取错误提示字符串
 */
extern const char* C4S_GetError( void );


/*
 *  延时一段时间(毫秒)
 *  注意:延时操作必须使用此函数，否则
 *  延时失效
 *  失败返回-1
 */
extern int C4S_Sleep( int ms );


/*
 *  获取终端可视的宽和长(按字来算)
 *  失败返回-1
 */
extern int C4S_GetScreenSize( int* w, int* h );


/*
 *  挂载监听，当可视大小发生改变后调用
 *  传入的函数
 *  失败返回-1
 */
extern int C4S_SetScreenSizeHook( void (*)( int w, int h ) );


/*
 *  显示光标
 *  失败返回-1
 */
extern int C4S_ShowCursor( void );


/*
 *  隐藏光标
 *  失败返回-1
 */
extern int C4S_HideCursor( void );


/*
 *  移动光标到指定位置(以字为单位)
 *  失败返回-1
 */
extern int C4S_MoveCursor( int x, int y );


/*
 *  设置输出文字颜色
 *  终端仅支持256色，RGB的范围为0-5
 *  失败返回-1
 */
extern int C4S_SetTextColor( int r, int g, int b );


/*
 *  设置输出背景颜色
 *  终端仅支持256色，RGB的范围为0-5
 *  失败返回-1
 */
extern int C4S_SetBackColor( int r, int g, int b );


/*
 *  开启输入回显
 *  注意:conio.h的getch、getche等
 *  输入函数将会改变回显状态，需要在
 *  使用之后立即使用该函数
 *  失败返回-1
 */
extern int C4S_OpenInputEcho( void );


/*
 *  关闭输入回显
 *  注意:conio.h的getch、getche等
 *  输入函数将会改变回显状态，需要在
 *  使用之后立即使用该函数
 *  失败返回-1
 */
extern int C4S_CloseInputEcho( void );


/*
 *  开启点击和滚动反馈
 *  当点击和滚动产生时，终端会向输入发送
 *  转义序列，序列内容为点击或滚动，并且
 *  抑制输入法的弹出
 *  注意:
 *    1. 在C4droid必须在终端打开发送鼠
 *       标事件才可产生序列，具体操作:
 *       终端模拟器->设置->发送鼠标事件
 *       -->√勾选
 *    2. 终端只能发送X和Y值在224以内的
 *       点击转义序列，超出部分不会产生
 *       转义序列
 *    3. 无论屏幕是否滚动，都不影响滚动
 *       序列的产生
 *  失败返回-1
 */
extern int C4S_OpenTouchEcho( void );


/*
 *  关闭点击和滚动反馈
 *  失败返回-1
 */
extern int C4S_CloseTouchEcho( void );


/*
 *  清空输入缓存区
 *  失败返回-1
 */
extern int C4s_ClearInput( void );


/*
 *  在给定的时间内获取输入
 *  strbuf为NULL时不不记录此处输入，
 *  但strsize请勿设置太大的值
 *  当mode为1时为字符串模式(兼容不好，
 *  不建议使用)，处理退格且自动在末尾
 *  加\0结束符号；当mode为0时为字符模
 *  式，原样填充若输入大于缓冲则提起结
 *  束输入
 *  成功返回成功处理的数量，失败返回-1
 */
extern int C4S_Input( char* strbuf, int strsize, int ms, int mode );


/*
 *  解析一组点击滚动转序列
 *  当suite为NULL时仅测试text是否
 *  为点击滚动转义序列
 *  成功返回解析完成的字符数量，解析
 *  非法序列返回1，解析到末尾返回0，
 *  失败返回-1
 */
extern int C4S_SplitSuite( const char* text, int size, C4S_Suite* suite );


#ifdef __cplusplus
}
#endif


#endif