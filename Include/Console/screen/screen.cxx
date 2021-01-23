/*
 *  screen.c[v2.0]
 *  获取终端屏幕的信息
 *  by 白狼·圣狂
 */

/*
 *  防重包含举措
 */
#ifndef _C4DROID_SCREEN_C
#define _C4DROID_SCREEN_C

/*
 *  与c++兼容
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 *  包含需要的头文件
 */
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <term.h>

#include "screen.hxx"

/*
 *  私有变量，不建议外部程序使用
 */
static uint8_t v_ScreeninfoUsable;
static const char *v_ScreeninfoErrMsg;
static void (*v_SignalCallBack)( int, int );

/*
 *  私有函数，不建议外部程序使用
 */
static void v_ScreeninfoSigFunc( int signo )
{
	int fd = STDOUT_FILENO;
	struct winsize ws;
	
	if ( v_ScreeninfoUsable && v_SignalCallBack && isatty( fd ) && ioctl( fd, TIOCGWINSZ, (char*)&ws ) >= 0)
		v_SignalCallBack( ws.ws_col, ws.ws_row );
}


static void v_SetScreeninfoError( const char *aMessage )
{
	v_ScreeninfoErrMsg = aMessage;
}

/*
 *  公有函数，可以被外部程序使用
 */
const char* C4S_GetError( void )
{
	const char* message;
	
	if ( v_ScreeninfoErrMsg == NULL )
		return "没有错误";
	
	message = v_ScreeninfoErrMsg;
	v_SetScreeninfoError( NULL );
	
	return message;
}

int C4S_Init( void )
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

int C4S_Quit( void )
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

int C4S_Sleep( int ms )
{
	char strtime[ 32 ];
	
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	sprintf( strtime, "sleep %d.%d", ms / 1000, ms % 1000 );
	system( strtime );
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_SetScreenSizeHook( void (*aCallBack)( int, int ) )
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

int C4S_GetScreenSize( int *cols, int *rows )
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

int C4S_ShowCursor( void )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	printf("\x1B[?25h");
	fflush(stdout);
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_HideCursor( void )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	printf("\x1B[?25l");
	fflush(stdout);
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_MoveCursor( int x, int y )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	if ( x < 0 || y < 0 )
	{
		v_SetScreeninfoError( "光标的位置不能是负数" );
		return -1;
	}
	
	printf("\x1B[%d;%dH", y, x);
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_OpenInputEcho( void )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	terminal_echo( 1 );
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_CloseInputEcho( void )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	terminal_echo( 0 );
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_SetTextColor( int r, int g, int b )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	if ( ( r < 0 || r > 5 ) || ( g < 0 || g > 5 ) || ( b < 0 || b > 5 ) )
	{
		v_SetScreeninfoError( "RGB的值只能指定0-5" );
		return -1;
	}
	
	printf("\x1B[38;2;%d", r * 36 + g * 6 + b );
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_SetBackColor( int r, int g, int b )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	if ( ( r < 0 || r > 5 ) || ( g < 0 || g > 5 ) || ( b < 0 || b > 5 ) )
	{
		v_SetScreeninfoError( "RGB的值只能指定0-5" );
		return -1;
	}
	
	printf("\x1B[48;2;%d", r * 36 + g * 6 + b );
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_OpenTouchEcho( void )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	printf("\x1B[?1000h");
	fflush(stdout);
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_CloseTouchEcho( void )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	printf("\x1B[?1000l");
	fflush(stdout);
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_Input( char* buf, int size, int ms, int mode )
{
	struct timeval time_wait;
	int todo_count = 0;
	int flag_free = 0;
	fd_set fds;
	
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	if ( size <= 0 || ms < 0 )
	{
		v_SetScreeninfoError( "参数不能小于0" );
		return -1;
	}
	
	if ( !buf )
	{
		if ( !( buf = ( char* )malloc( sizeof ( char ) * size ) ) )
		{
			v_SetScreeninfoError( "strsize值太大，空间不足" );
			return -1;
		}
		
		flag_free = 1;
	}
	
	FD_ZERO( &fds );
	FD_SET( STDIN_FILENO, &fds );
	
	time_wait.tv_sec = ms / 1000;
	time_wait.tv_usec = ms % 1000 * 1000;
	
	if ( mode )
		--size;
	
	terminal_lnbuf( 0 );
	
	while ( select( STDIN_FILENO + 1, &fds, NULL, NULL, &time_wait ) && todo_count < size )
	{
		todo_count += read( STDIN_FILENO, buf + todo_count, size - todo_count );
		
		if ( mode && buf[ todo_count - 1 ] == 127)
		{
			if ( todo_count > 1 )
				--todo_count;
			--todo_count;
		}
		
		FD_SET( STDIN_FILENO, &fds );
	}
	
	if ( mode )
		buf[ todo_count ] = '\0';
	
	terminal_lnbuf( 1 );
	
	if ( flag_free )
		free( buf );
		
	v_SetScreeninfoError( NULL );
	
	return todo_count;
}

int C4S_ClearInput( void )
{
	char* str;
	
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	if ( !( str = ( char* )malloc( sizeof ( char ) * 50 ) ) )
	{
		v_SetScreeninfoError( "刷新缓冲分配失败，内存不足" );
		return -1;
	}
	
	while ( C4S_Input( str, sizeof ( str ), 0, 0 ) );
	
	free( str );
	
	v_SetScreeninfoError( NULL );
	
	return 0;
}

int C4S_SplitSuite( const char* text, int size, C4S_Suite* suite )
{
	if ( v_ScreeninfoUsable == 0 )
	{
		v_SetScreeninfoError( "未初始化" );
		return -1;
	}
	
	if ( !text || size < 0 )
	{
		v_SetScreeninfoError( "text参数不能为NULL且size不能小于0" );
		return -1;
	}
	
	v_SetScreeninfoError( NULL );
	
	if ( size < 6 )
		return 0;
	
	if ( !memcmp( text, "\x1B[M", 3 ) )
		switch ( text[ 3 ] )
		{
			case 32:
				if ( suite )
				{
					suite->chuck.type = C4S_SUITE_CHUCK;
					suite->chuck.state = C4S_CHUCK_PRESSED;
					suite->chuck.x = text[ 5 ] - 33;
					suite->chuck.y = text[ 4 ] - 33;
				}
				return 6;
			
			case 35:
				if ( suite )
				{
					suite->chuck.type = C4S_SUITE_CHUCK;
					suite->chuck.state = C4S_CHUCK_RELEASED;
					suite->chuck.x = text[ 5 ] - 33;
					suite->chuck.y = text[ 4 ] - 33;
				}
				return 6;
				
			case 96:
				if ( suite )
				{
					suite->scroll.type = C4S_SUITE_SCROLL;
					suite->scroll.direction = C4S_SCROLL_UP;
					suite->scroll.x = text[ 5 ] - 33;
					suite->scroll.y = text[ 4 ] - 33;
				}
				return 6;
			
			case 97:
				if ( suite )
				{
					suite->scroll.type = C4S_SUITE_SCROLL;
					suite->scroll.direction = C4S_SCROLL_DOWN;
					suite->scroll.x = text[ 5 ] - 33;
					suite->scroll.y = text[ 4 ] - 33;
				}
				return 6;
				
			default:
				if ( suite )
					memset( suite, 0, sizeof ( C4S_Suite ) );
				break;
		}
	
	return 1;
}

#ifdef __cplusplus
}
#endif

#endif