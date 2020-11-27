/*
例程39. 简易的文本编辑器
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/22
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define MAXLEN 256      //读取文件每行最大字符数
#define MAXLINE 2000    //文件最大行数
char buffer[MAXLEN],fname[120];
char *lineptr[MAXLINE];
FILE *fp;
void edit();
void replace();
void insert();
void del();
void quit();
void Save();
void save(char *fname);
char comch[]="EeRrIiDdSsQq";/*命令符*/
void(*comfun[])()={edit,replace,insert,del,Save,quit};//使用函数指针数组处理对应操作
int modified=0,/*正文被修改标志*/
	last;/*当前正文行数*/
char *chpt;/*输入命令行字符指针*/

int main()
{
	int j;

	last=0;
	while(1)
	{
		printf("\n\n\t请输入:\n\te :打开文件\n\tr[写入行数][起始行][结束行]:编辑指定行\n\ti[插入行数][起始行] :插入行\n\td[起始行][结束行] :删除指定行\n\ts :保存文件\n\tq :退出\n");
		gets(buffer);/*读入命令行*/
		for(chpt=buffer;*chpt==' '||*chpt=='\t';chpt++);/*掠过空白符*/
		if(*chpt=='\0') continue;/*空行重新输入*/
		for(j=0;comch[j]!='\0'&&comch[j]!=*chpt;j++);/*查命令符*/
		if(comch[j]=='\0') continue;/*非法命令符*/
		chpt++;/*掠过命令符，指向参数*/
		(*comfun[j/2])();/*执行对应函数*/
		fprintf(stdout,"The text is:\n");
		for(j=0;j<last;j++)/*显示正文*/
			printf("\e[33m%4d \e[32m%s\e[0m",j+1,lineptr[j]);
	}
}
void quit()
{
	int c;
	if(modified)/* 如正文被修改 */
	{
		printf("Save? (y/n)");
		while(!(((c=getchar())>='a'&&c<='z')||(c>='A'&&c<='Z')));
		if(c=='y'||c=='Y')
			save(fname); /* 保存被修改过的正文 */
	}
	for(c=0;c<last;c++)
		free(lineptr[c]);	/* 释放内存 */
	exit(0);
}

void insert()
{
	int k,m,i;
	sscanf(chpt,"%d%d",&k,&m);	/* 读入参数 */
	if(m<0||m>last||last+k>=MAXLINE)/* 检查参数合理性 */
	{
		printf("Error!\n");
		return;
	}
	for(i=last;i>m;i--)/* 后继行向后移 */
		lineptr[i+k-1]=lineptr[i-1];
	for(i=0;i<k;i++)   /* 读入k行正文，并插入 */
	{
		fgets(buffer,MAXLEN,stdin);
		lineptr[m+i]=(char *)malloc(strlen(buffer)+1);
		strcpy(lineptr[m+i],buffer);
	}
	last+=k;	/* 修正正文行数 */
	modified=1;	/* 正文被修改 */
}

void del()
{
	int i,j,m,n;
	sscanf(chpt,"%d%d",&m,&n);	/* 读入参数 */
	if(m<=0||m>last||n<m)	/* 检查参数合理性 */
	{
		printf("Error!\n");
		return;
	}
	if(n>last)
		n=last;		/* 修正参数 */
	for(i=m;i<=n;i++)	/* 删除正文 */
		free(lineptr[i-1]);
	for(i=m,j=n+1;j<=last;i++,j++)
		lineptr[i-1]=lineptr[j-1];
	last=i-1;	/* 修正正文行数 */
	modified=1;	/* 正文被修改 */
}

void replace()
{
	int k,m,n,i,j;
	sscanf(chpt,"%d%d%d",&k,&m,&n);	/* 读入参数 */
	//插入行数,修改起始行数,修改结束行数
	if(m<=0||m>last||n<m||last-(n-m+1)+k>=MAXLINE)/* 检查参数合理性 */
	{
		printf("Error!\n");
		return;
	}
	/* 先完成删除 */
	if(n>last)
		n=last;		/* 修正参数 */
	for(i=m;i<=n;i++)	/* 删除正文 */
		free(lineptr[i-1]);
	for(i=m,j=n+1;j<=last;i++,j++)
		lineptr[i-1]=lineptr[j-1];
	last=i-1;
	/* 以下完成插入 */
	for(i=last;i>=m;i--)
		lineptr[i+k-1]=lineptr[i-1];
	for(i=0;i<k;i++)
	{
		fgets(buffer,MAXLEN,stdin);
		lineptr[m+i-1]=(char *)malloc(strlen(buffer)+1);
		strcpy(lineptr[m+i-1],buffer);
	}
	last+=k;	/* 修正正文行数 */
	modified=1;	/* 正文被修改 */
}

void save(char *fname)	/* 保存文件 */
{
	int i;
	FILE *fp;
	if((fp=fopen(fname,"w"))==NULL)
	{
		fprintf(stderr,"Can't open %s.\n",fname);
		return ;
	}
	for(i=0;i<last;i++)
	{
		fputs(lineptr[i],fp);
		free(lineptr[i]);
	}
	fclose(fp);
}

void Save()
{
	if(modified)/* 如正文被修改 */
	{
		save(fname); /* 保存被修改过的正文 */
	}
	else 
	printf("文件没做任何修改\n");
}
void edit()	/* 编辑命令 */
{
	int i;
	FILE *fp;
	i=sscanf(chpt,"%s",fname);	/* 读入文件名 */
	if(i!=1)
	{
		printf("Enter file name.\n");
		scanf("%s",fname);
		while(kbhit())	getch();//清除多余的输入
		setbuf(stdin,NULL);
	}
	if((fp=fopen(fname,"r"))==NULL) /* 读打开 */
	{
		fp=fopen(fname,"w");	/* 如不存在，则创建文件 */
		fclose(fp);
		fp=fopen(fname,"r");	/* 重新读打开 */
	}
	i=0;
	while(fgets(buffer,MAXLEN,fp)==buffer)
	{
		lineptr[i]=(char *)malloc(strlen(buffer)+1);
//		strcpy(lineptr[i],buffer);
		sprintf(lineptr[i],"%s",buffer);
		i++;
	}
	fclose(fp);
	last=i;
}