// 酷狗歌词解密转换 krc2lrc v1.1 
// 设置输入目录，输出目录，运行即可

// date: 初版大概是2018年 2022-03-04优化
// author: QAIU(主程序), 枫羽(入口函数+文件遍历)

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <dirent.h>
#include "krc2lrc.h"

// krc歌词路径
#define KRC_PATH "./krc/"

// 解码krc歌词文件的输出目录
#define DECODE_PATH "./krc/Decode/"

// 转码lrc歌词文件输出目录
#define CONVERT_PATH "./krc/Convert/"

int CreateDir();
int Search(const char *path);
int main(int argc,char **argv)
{
	CreateDir();
	Search(KRC_PATH);
	return 0;
}

int CreateDir(){
	DIR *ddir=opendir(DECODE_PATH),
	*cdir=opendir(CONVERT_PATH);
	
	if(ddir==NULL){
		//PUT_ERROR;
		mkdir(DECODE_PATH,0);
	}else{
		closedir(ddir);
	}
	
	if(cdir==NULL){
		mkdir(CONVERT_PATH,0);
	}else{
		closedir(cdir);
	}
	return 0;
}

int Search(const char *path){
	DIR *dp=opendir(path);
	if(dp==NULL)
	{
		PUT_ERROR(path);
	}
	else
	{
		struct dirent *fp;
		
		while((fp=readdir(dp))!=NULL)
		{
			if(fp->d_type==DT_DIR)
				continue;
			if(fp->d_type==DT_REG)
			{
				if(strcmp(KRC_END, &fp->d_name[strlen(fp->d_name) - 4]) == 0)
				{
					//如果后缀名是.krc，就执行krc2lrc函数
					printf("开始解析：%s\n",fp->d_name);
					krc2lrc(fp->d_name, path, DECODE_PATH, CONVERT_PATH);
					printf("\n");
				}else{
					continue;
				}
			}else{
				continue;
			}
		}
	}
	return 0;
}