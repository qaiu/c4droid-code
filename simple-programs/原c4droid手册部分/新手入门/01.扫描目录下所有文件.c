/*c4droid代码手册
*扫描目录下所有文件
*dxkite编写
*/
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int readFolder(const char *path)
{
	DIR *np = NULL;
	np = opendir(path);
	struct dirent *r = NULL;
	while ((r = readdir(np)) != NULL)
	{
		if (r->d_type != DT_DIR)
		{
			printf("%s\n", r->d_name);
		}
		else
		{
			char new_p[1024];
			if (strcmp(r->d_name, ".") != 0 && strcmp(r->d_name, "..") != 0)
			{
				sprintf(new_p, "%s/%s", path, r->d_name);
				read(new_p);
			}
		}

	}

	closedir(np);
}

int main()
{
	// 找出给定目录下所有文件
	// . 表示当前目录
		readFolder(".");
}