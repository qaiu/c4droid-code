/*
           
     源码：文件查找
     作者：傻傻_痴痴
     来源：c4droid吧（欢迎加入交流）
     
     日期：2014.3.30
     
        若要转载源码，请注明作者出处，禁止修改为自己名义发布源码
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

/*歌曲总数*/
int MusicNum;

/*歌曲链表，我们不知道歌曲的总数，所以使用链表来实现*/
struct song
{
	char *name; /*文件名*/
	char *path; /*路径名*/
	struct song *next;
} *MySong=NULL, *sp1, *sp2;

/*判断是否符合音乐文件，你可以加入或者过滤不同类型文件*/
bool isMusic(char *filename)
{
	int len = strlen(filename);
	char *ch = &filename[len - 4];
	if (strcmp(".mp3", ch) == 0)
	{
		return true;
	}
	else if (strcmp(".ape", ch) == 0)
	{
		return true;
	}
	else if (strcmp(".wav", ch) == 0)
	{
		return true;
	}
	else if (strcmp(".m4a", ch) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*扫描文件夹*/
void findMusic(const char *dirname)
{
	DIR *dir; /*文件夹指针*/
	struct dirent *ptr;
	char *ch, *music;
	/*如果打不开，直接返回*/
	if ((dir = opendir(dirname)) == NULL)
	{
		return;
	}
	/*开始读取*/
	while ((ptr = readdir(dir)) != NULL)
	{
		/*如果遇到文件夹*/
		if (ptr->d_type & DT_DIR)
		{
			if (strcmp(".", ptr->d_name) == 0
				|| strcmp("..", ptr->d_name) == 0)
			{
				continue;
			}
			ch = (char *)malloc(sizeof(char *) *(strlen(dirname) +strlen(ptr->d_name)));
			sprintf(ch, "%s/%s", dirname, ptr->d_name);
			/*遇到文件夹，继续利用跌送法打开*/
			findMusic(ch); 
		}
		else /*否则遇到文件*/
		{
			/*如果是音乐文件，开始创建链表*/
			if (isMusic(ptr->d_name))
			{
				MusicNum++;
				sp2 = (struct song *)malloc(sizeof(struct song ));
				music = (char *)malloc(sizeof(ptr->d_name) + 8);
				sprintf(music, "%d.%s", MusicNum, ptr->d_name);
				sp2->name = (char *)malloc(sizeof(music));
				sp2->name = music;
				ch = (char *)malloc(sizeof(dirname) + sizeof(ptr->d_name));
				sprintf(ch, "%s/%s", dirname, ptr->d_name);
				sp2->path = (char *)malloc(sizeof(ch));
				sp2->path = ch;
#ifdef _TERMINAL_
				printf("\033[33;5m%s\n", music);
#endif
				if (MySong == NULL)
				{
					MySong = sp2;
				}
				else
				{
					sp1->next = sp2;
				}
				sp1 = sp2;
				sp2->next=NULL;
			}
		}
	}
	closedir(dir);
}

/*是否完成扫描*/
bool findFinish()
{
#ifdef _TERMINAL_
	printf("正在扫描888音乐，请稍等…\n");
#endif
	findMusic("/sdcard/");
	return true;
}

/*获取链表指定位置的歌曲*/
struct song *indexSong(struct song *s, int index)
{
	struct song*stemp=s;
	while (index != 1&&s!=NULL)
	{
		stemp = stemp->next;
		--index;
	}
	return stemp;
}