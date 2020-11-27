#include <stdio.h>
#include <dirent.h>

#define FilePath "./"

int main()
{
	int filesize = 0;
	DIR *dir = NULL;
	struct dirent *entry;
	int i = 0;

	if ((dir = opendir(FilePath)) == NULL)
	{
		printf("opendir failed!");
		return -1;
	}
	else
	{
		while (entry = readdir(dir))
		{
			 printf("filename%d = %s   ", i, entry->d_name);	// 输出文件或者目录的名称
			printf("filetype = %d\n", entry->d_type);	// 输出文件类型
		}

		closedir(dir);
	}
	return 0;
}