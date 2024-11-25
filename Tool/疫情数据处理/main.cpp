/**
 * Author:QAIU
 * 将首行数据转为各列数据
 * 处理日期
 * 2020.3.23 20:01
 *
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	FILE *countryFp=fopen("country.txt","r");
//	FILE *newCasesFp=fopen("new_cases.csv","r");
//	FILE *out = fopen("data.csv","w+");
	FILE *newCasesFp=fopen("total_cases.csv","r");
	FILE *out = fopen("data_total.csv","w+");
	fputs("date,value,name\n",out);
	char countryRow[256*64]={};
	char countryRows[256][64]={};
	char countryEN[256][64]={};
	char countryCH[256][64]={};
	fgets(countryRow,64*256,newCasesFp);
	int colnum=0;
	char *p=strtok(countryRow,",");
	p=strtok(NULL,",");
	p=strtok(NULL,",");
	while (p!=NULL)
	{
		strcpy(countryRows[colnum++],p);
		p=strtok(NULL,",");
	}
		
	char buff[128]={};
	int i=0, k=0;
	//国家名英文和国家名中文映射
	while (fgets(buff,128,countryFp))
	{
		for (int i = 0; i < colnum; i++) 
		{
			if (strstr(buff,countryRows[i])) 
			{
			//	printf("%s\n",countryRows[i]);	
				fgets(buff,128,countryFp);
				fgets(buff,128,countryFp);
				memset(countryRows[i],0,strlen(countryRows[i]));
				strncpy(countryRows[i],buff,strlen(buff)-1);
			//	printf("%d,%d>%s",k++,i,buff);
				break;
			}
			
		}
		strcpy(countryCH[i++],buff);
	}
	//生成csv表格文件
	char buff2[2048]={};
	while (fgets(buff2,2048,newCasesFp))
	{
		//strtok函数不支持切割成空串，所以先对当前行做预处理
		char temp[2048]={};
		for (int i = 0,k=0; i < strlen(buff2)-1; i++) 
		{
			temp[k++] = buff2[i];
			if (buff2[i]==','&&buff2[i+1]==',')
			{
				strcat(temp,"0.0");
				k+=3;
			}
			if (buff2[i]==','&&buff2[i+1]=='\n')
			{
				strcat(temp,"0.0");
				break;
			}
			
		}	
		strcpy(buff2,temp);
		char* date=strtok(buff2,",");	
		strcpy(buff,date);
		strtok(NULL,",");
		p=strtok(NULL,",");
		
		for (int i=0;p!=NULL;i++)
		{
			//去掉数据末尾.0
			if (strstr(p,".0")) p[strlen(p)-2]=0;
			//去掉中国，台湾地区，和国际均值以及过滤0数据
			if (strcmp(p,"0")
			&&strcmp(countryRows[i],"International")
			&&strcmp(countryRows[i],"Taiwan")
			&&strcmp(countryRows[i],"中国")
			)
				fprintf(out,"%s,%s,%s\n",date,p,countryRows[i]);
			//继续切割
			p=strtok(NULL,",");
		}
		
	}
	fclose(out);
	puts("OK");
	return 0;
}