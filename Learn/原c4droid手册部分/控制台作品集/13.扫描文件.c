/*c4droid代码手册
 *扫描文件
 *TTHHR编写
 *转载请说明出处
*/
#include<sys/types.h>
#include <stdio.h>
#include<dirent.h>
#include<unistd.h>
int main(int argc,char **argv)
{
DIR * dir;
struct dirent * ptr;
int i;
if(argc==1)
dir=opendir("/sdcard");
else 
dir=opendir(argv[1]);
while((ptr=readdir(dir))!=NULL)
{
printf("%s\n",ptr->d_name);
}
closedir(dir);
return 0;
}