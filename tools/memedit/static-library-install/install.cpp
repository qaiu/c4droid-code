/* 
  memedit00安装程序 支持32位和64位手机，暂时不支持电脑模拟器
  1.运行即可安装成功
  2.修改g++参数(设置->G++参数)如下 (直接复制粘贴)
cd (c4droid:GCCROOT)tmpdir
export TEMP=(c4droid:GCCROOT)tmpdir
(c4droid:GCCROOT)bin/(c4droid:PREFIX)-g++ (c4droid:SRC) -lm -ldl -llog -lz -lmemedit00 -I(c4droid:CURSRCDIR) -std=c++14 -Wfatal-errors (c4droid:MODEARGS) -o (c4droid:BIN)
(c4droid:ERROUT)

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
//返回子串位置
int strpos(const char *haystack,const char *needle)  
{
	register unsigned char c, needc;
	unsigned char const *from, *end;
	int len = strlen(haystack);
	int needlen = strlen(needle);
	from = (unsigned char *)haystack;
	end = (unsigned char *)haystack + len;
	const char *findreset = needle;
	for (int i = 0; from < end; ++i) {
		c = *from++;
		needc = *needle;
		if(c == needc) {
			++needle;
			if(*needle == '\0') {
				if (len == needlen) 
					return 0;
				else
					return i - needlen+1;
			}
		}  
		else {  
			if(*needle == '\0' && needlen > 0)
				return i - needlen +1;
			needle = findreset;  
		}
	}  
	return  -1;  
} 

//aarch64-linux-android
//arm-linux-androideabi 
int main()
{
	
	if (getuid() == 0)
	{
		puts("请在非root环境下运行: 软件右上角>设置>以root身份运行>取消勾选");
		exit(1);
	}
	
	char path[64]={};
	char path_inc[256]={};
	char path_lib[256]={};
	
	const char*env=getenv("PATH");	
	int n=strpos(env,"/bintools");	
	strncpy(path,env,n);
	strcpy(path_inc,"cp include/memedit00.h ");
	strcat(path_inc,path);
	
	#if defined (__arm64__)|| defined (__aarch64__)
    printf("你的系统是arm64位: 64-bit [__arm64__ arch]\n");
    strcat(path_inc,"/gcc/aarch64-linux-android/include/");
    strcpy(path_lib,"cp lib64/libmemedit00.a ");
    strcat(path_lib,path);
    strcat(path_lib,"/gcc/aarch64-linux-android/lib/");
    #elif defined(__arm__)
    printf("你的系统是arm32位: [__arm__ arch]\n");
    strcat(path_inc,"/gcc/arm-linux-androideabi/include/");
    strcpy(path_lib,"cp lib32/libmemedit00.a ");
    strcat(path_lib,path);
    strcat(path_lib,"/gcc/arm-linux-androideabi/lib/");
    #else
    printf("暂不支持\n");
    return 0;
    #endif
    puts("开始安装");
    puts(path_lib);
    system(path_lib);
    puts(path_inc);
    system(path_inc);
    puts("安装成功");
    return 0;
}