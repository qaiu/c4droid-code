/*
 *  screeninfo.h[v1.0]
 *  获取终端屏幕的信息
 *  by 白狼·圣狂
 */

///
// 防重包含举措
//
#ifndef _C4DROID_SCREENINFO_H
#define _C4DROID_SCREENINFO_H

///
// 与c++兼容
//
#ifdef __cplusplus
extern "C" {
#endif

///
// 包含需要的头文件
//
#include "screeninfo.c"

///
// 初始化，使用前请调用
// 返回-1为失败
//
extern int C4Screeninfo_Init( void );

/// 
// 与初始化相反，不再使用时调用
// 返回-1为失败
//
extern int C4Screeninfo_Quit( void );

///
// 获取终端可视的宽和长(按字来算)
// 返回-1为失败
//
extern int C4Screeninfo_GetSize( int* w, int* h );

///
// 挂载监听，当窗口大小被改变后调用
// 传入函数
// 返回-1为失败
//
extern int C4Screeninfo_SetHook( void (*)( int w, int h ) );

///
// 获取错误提示字符串
//
extern const char* C4Screeninfo_GetError( void );


#ifdef __cplusplus
}
#endif

#endif