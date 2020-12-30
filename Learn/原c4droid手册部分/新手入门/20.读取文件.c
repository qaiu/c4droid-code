/*c4droid代码手册
 *读取文本
 *TTHHR编写
 *转载请说明出处
*/
#include<stdio.h>
#include<stdlib.h>
main()
{
	FILE *fp;
char ch,wjm[51];
printf("请输入文本路径:");
scanf("%s",wjm);
if (!(fp=fopen(wjm,"r")))
{
	printf("%s文件打开错误\n请检查!",wjm);
	exit(1);
}
while (feof(fp)==0)
	{
		ch=fgetc(fp);
		printf("%c",ch);
	}	
	printf("\n完成！");
	return 0;
}
