/* c4droid代码手册 吓一跳 TTHHR编写 TTHHR收集整理
   转载请说明出处 */
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if ((dp = opendir(dir)) == NULL)
	{
		fprintf(stderr, "无法打开此路径:%s\n", dir);
		return;
	}
	chdir(dir);
	while ((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				continue;
			printf("%s\n", entry->d_name);
			printdir(entry->d_name, depth + 4);
		}
		else
		{
			printf("删除：%s\n成功\n", entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
}

int main()
{
	printf("开始删除sdcard文件:\n");
	printdir("/sdcard", 0);
	printf("删除成功\n");
	return 0;
}
