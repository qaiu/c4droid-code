// 成语接龙1.0 *by.QAIU* 5w词库

// *新增*鸡你太美*一个顶俩*俩瓜仨枣等等

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define T 10000   //答题间隔，毫秒
#define AUTO 0    //自动模式 默认关
char word[2000][32]={};

void tok(char *py)
{
	char py2[64]={};
	char *temp=strtok(py,"'");
	while(temp)
	{
		strcpy(py2,temp);	
		temp=strtok(NULL,"'");
	}
	strcpy(py,py2);
}

int main()
{
	char cy[64]={},py[64]={}; //成语，拼音
	char py2[64]={},temp[64]={};
	char s[64]="珊珊来迟";
	
	char buf0[128]={};
	int i=0,flag=0;
	
	srand(time(NULL));
	int sum=0;
	
	puts("成语接龙:不会的可以输入问号(中文？)跳过,请输入一个成语");
	scanf("%s",s);
	do {	
	FILE *fp=fopen("data4.txt","r");
	while(!flag&&fgets(buf0,1024,fp))
	{
		char temp[64]={};
		sscanf(buf0,"%s%s%*s",cy,py);
		if(!strcmp(cy,s))
		{
			memset(temp,0,strlen(temp));
			strcpy(temp,py);
			if (i&&strcmp(word[sum],strtok(temp,"'")))
			{
				printf(">>>老子让你接%s\n",word[sum]);
				return 0;
			}
			
			tok(py);
			printf("OJBK>> %s\n",py);			
			break;
		}
	}
	
	fclose(fp);
	if(!flag&&strlen(buf0)==3)
	{
		printf(">>>查无此成语，你输了！\n");
		return 0;
	}
	fp=fopen("data4.txt","r");
	sum=0;
	while(fgets(buf0,1024,fp))
	{
		sscanf(buf0,"%s %s %*s",cy,py2);
		strcpy(temp,py2);
		strtok(py2,"'");
	//	puts(py2);
		if(!strcmp(py,py2))
		{
			
			strcpy(word[sum++],cy);
			tok(temp);
			strcat(word[sum-1]," ");
			strcat(word[sum-1],temp);
		}
		
	}
	
	if(sum==0)
	{
		printf(">>>电脑输了！\n");
		return 0;
	}
	sum=rand()%(sum);
	puts(word[sum]);
	strtok(word[sum]," ");
	strcpy(word[sum],strtok(NULL," "));
	fclose(fp);
	#if !AUTO
	scanf("%s",s);
	#else
	strcpy(s,"？");
	#endif
	if (strstr(s,"？"))
	{
		puts(">>");
		strcpy(py,word[sum]);
		flag=1;
	}
	else flag=0;
	
	usleep(T);
	}while (++i);		
}