/*
c4droid代码手册
使用前说明
！！！！！！！！！！！！！！！
首先进入设置，找到G++参数设置（英文版G++ arguments），在(c4droid:SRC)后面加个空格，再加上-lOpenSLES，同时要与后面的参数留个空格，l是小写字母L，不是符号，也不是大写 i 。
修改后的参考：
cd (c4droid:GCCROOT)tmpdir
export TEMP=(c4droid:GCCROOT)tmpdir
(c4droid:GCCROOT)bin/(c4droid:PREFIX)-g++ (c4droid:SRC) -lOpenSLES -lm -ldl -llog -lz -I(c4droid:CURSRCDIR) -Wfatal-errors (c4droid:MODEARGS) -o (c4droid:BIN)
(c4droid:ERROUT)
！！！！！！！！！！！！！
*/
/*
           
     源码：c4droid控制台音乐播放器
     作者：傻傻_痴痴
     来源：c4droid吧（欢迎加入交流）
     
      Version：1.1

   更新说明：
   1、修复播放歌曲完毕歌曲名称不改变bug

2、优化部分内存，释放垃圾内存

   更新日期：2014.4.20 若要转载源码，请注明作者出处，禁止修改为自己名义发布源码
 
 
*/


/* 定义为终端模拟器 */
#ifndef _TERMINAL_
#define _TERMINAL_
#endif
#include <PlayMusic.c>
#include <conio.h>

static bool q = false;

/* 绘制菜单 */
void draw_menu()
{
	int i;
	printf("\n");
	for (i = 0; i < 10; i++)
		printf("\033[33;5m***");
	printf("\n*");
	for (i = 0; i < 9; i++)
		printf("   ");
	printf(" *\n*");
	for (i = 0; i < 11; i++)
		printf(" ");
	printf("Welcome！");
	for (i = 0; i < 8; i++)
		printf(" ");
	printf("*");
	printf("\n*");
	for (i = 0; i < 9; i++)
		printf("   ");
	printf(" *\n");
	for (i = 0; i < 10; i++)
		printf("***");
	printf("\n输入歌曲序列号，直接播放对应歌曲\n");
	printf
		("输入G/g播放上一曲\n输入H/h播放下一曲\n输入P/p暂停或者播放\n输入S/s进行搜索\n输入C/c清除记录\n输入E/e退出\n");

}

/* 功能选择 */
void Choose(char *c)
{
	if (strcmp(c, "g") == 0 || strcmp(c, "G") == 0)
	{
		PlayUpper(&ListNum);	// 上一曲
	}
	else if (strcmp(c, "h") == 0 || strcmp(c, "H") == 0)
	{
		PlayNext(&ListNum);		// 下一曲
	}
	else if (strcmp(c, "p") == 0 || strcmp(c, "P") == 0)
	{
		PauseMUS();				// 暂停或者播放
	}
	else if (strcmp(c, "s") == 0 || strcmp(c, "S") == 0)
	{
		char key[128];
		printf("请输入搜索关键词：");
		scanf("%s", key);
		struct song *result = search(key);
		printf("搜索结果：\n");
		if (result == NULL)
		{
			printf("搜索结果为空！\n");
		}
		else
		{
			while (result != NULL)
			{
				printf("%s\n", result->name);
				result = result->next;
			}
			printf("输入C/c清除记录\n");
		}
	}
	else if (strcmp(c, "c") == 0 || strcmp(c, "C") == 0)
	{
		/* 清除屏幕，重新绘制歌曲列表和菜单 */
		clrscr();
		struct song *ts = MySong;
		while (ts != NULL)
		{
			printf("%s\n", ts->name);
			ts = ts->next;
		}
		draw_menu();
	}
	else if (strcmp(c, "e") == 0 || strcmp(c, "E") == 0)
	{
		q = true;
		PlayShutdown();			// 关闭
	}
	else
	{
		printf("输入不合法，请重新输入\n");
	}
}

/* 判断输入的字符串是否可以转换成数字，如果可以，则播放对应歌曲 
 */
bool inputNum(char *str)
{
	int n = strlen(str);		// 获取长度
	int i;
	for (i = 0; i < n; i++)
	{
		if (str[i] < '0' || str[i] > '9')	// 如果有一个不符合数字，则直接返回false
			return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	InitMusic();				// 启动音乐并且扫描
	draw_menu();				// 绘制菜单
	char ch[128];
	while (!q)
	{
		scanf("%s", ch);
		if (inputNum(ch))
		{
			int m = atoi(ch);
			if (m <= 0 || m > MusicNum)
			{
				printf("输入不合法，请输入正确序号\n");
			}
			else
			{
				ListNum = m;
				struct song *ps = indexSong(MySong, ListNum);
				if (ps!=NULL)
				{
				PlayMUS(ps->path);
				printf("播放： %s\n", ps->name);
				}
			}
		}
		else
		{
			Choose(ch);
		}
	}
	printf("退出完毕！\n");
	return 0;
}