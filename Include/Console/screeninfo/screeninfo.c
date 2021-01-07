/*
 *  screeninfo.c[v1.0]
 *  获取终端屏幕的信息
 *  by 白狼·圣狂
 */

///
// 防重包含举措
//
#ifndef _C4DROID_SCREENINFO_C
#define _C4DROID_SCREENINFO_C

///
// 与c++兼容
//
#ifdef __cplusplus
extern "C" {
#endif

///
// 包含需要的头文件
//
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <term.h>

///
// 私有变量，不建议外部程序使用
//
static uint8_t v_ScreeninfoUsable;
static const char *v_ScreeninfoErrMsg;
static void (*v_SignalCallBack)( int, int );

///
// 私有方法，不建议外部程序使用
//
static void v_ScreeninfoSigFunc( int signo )
{
	int fd = fileno(stdout);
	struct winsize ws;
	
	if ( v_ScreeninfoUsable && v_SignalCallBack && isatty( fd ) && ioctl( fd, TIOCGWINSZ, (char*)&ws ) >= 0)
		v_SignalCallBack( ws.ws_col, ws.ws_row );
}

static void v_SetScreeninfoError( const char *aMessage )
{
	v_ScreeninfoErrMsg = aMessage;
}

///
// 公用方法，可以被外部程序使用
//
const char* C4Screeninfo_GetError( void )
{
	const char* message;
	
	if ( v_ScreeninfoErrMsg == NULL )
		return "没有错误";
	
	message = v_ScreeninfoErrMsg;
	v_SetScreeninfoError( NULL );
	
	return message;
}

int C4Screeninfo_Init( void )
{
	v_SignalCallBack = NULL;
	
	if ( v_ScreeninfoUsable )
	{
		v_SetScreeninfoError( "已被初始化" );
		return -1;
	}
	
	if ( signal( SIGWINCH, v_ScreeninfoSigFunc ) == SIG_ERR )
	{
		v_SetScreeninfoError( "无法挂载信号函数" );
		return -1;
	}
	
	v_ScreeninfoUsable = 1;
	v_SetScreeninfoError( NULL );
	return 0;
}

int C4Screeninfo_Quit( void )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	signal( SIGWINCH, NULL );
	
	v_ScreeninfoUsable = 0;
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4Screeninfo_SetHook( void (*aCallBack)( int, int ) )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	v_SignalCallBack = aCallBack;
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4Screeninfo_GetSize( int *cols, int *rows )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	setupterm( NULL, fileno( stdout ), NULL );
	
	if ( cols )
		*cols = tigetnum( "cols" );
	
	if ( rows )
		*rows = tigetnum( "lines" );
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif