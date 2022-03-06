#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <zlib.h>
#include "krc2lrc.h"

// 新增输入路径和输出路径参数
int krc2lrc(const char *file_name, const char *krc_path, const char *decode_path, const char *convert_path)
{
	char *decode_file=(char *)malloc(strlen(decode_path)+strlen(file_name)+1);
	char *convert_file=(char *)malloc(strlen(convert_path)+strlen(file_name)+5);
	char *file_path=(char *)malloc(strlen(krc_path)+strlen(file_name)+1);
	strcpy(file_path,krc_path);
	strcat(file_path,file_name);
	
	sprintf(decode_file,"%s%s",decode_path,file_name);
	char temp_file_name[255]={};
	strncpy(temp_file_name, file_name, strlen(file_name)-4);
	sprintf(convert_file,"%s%s%s",convert_path,temp_file_name,LRC_END);
	
	FILE*fp;
	
	static const byte miarry[] = {
		'@','G','a','w','^','2','t','G','Q','6','1','-',L'Î',L'Ò','n','i'
	};
	if((fp = fopen(file_path,"rb"))==NULL)
	{
		PUT_ERROR(file_path);
		return-1;
	}
	
	char top[5]={};
	fread(top,4,1,fp);
	if (strcmp(top,"krc1"))
	{
		printf("The file type not krc\n");
		return -1;
	}
		
	int length = file_size(file_path)-4;
	if (length==-1||length>1000000)//歌词文件不能超过1M
	{
		printf("Read file size failed\n");
		return -1;
	}
	byte* zip_byte = (byte*)malloc(length);
	fread(zip_byte,length,1,fp);
	fclose(fp);
	for(int k = 0;k<length;k++)
	{
		int l = k%16;
		zip_byte[k] = (byte)(zip_byte[k]^miarry[l]);
	}
	uLong tlen = length*10;
	char*un_zip = (char*)malloc(length*10);
	if(uncompress((byte*)un_zip,&tlen,zip_byte,length)!=Z_OK)
	{
		PUT_ERROR("Compress failed");
		return -1;
	}
	strrchr(un_zip,'\n')[0]=NULL;
	
	if((fp = fopen(decode_file,"wb+"))==NULL)
	{
		PUT_ERROR(decode_file);
		return -1;
	}
	
	if(fwrite(un_zip,strlen(un_zip),1,fp)==1)
	{
		printf("解码成功！新文件写入 %s\n",decode_file);
	}
	else
	{
		PUT_ERROR("写入失败");
	}
	
	
	fclose(fp);
	
	// krc转lrc
	if((fp = fopen(convert_file,"wb+"))==NULL)
	{
		PUT_ERROR(convert_file);
		return-1;
	}
	
	int len = strlen(un_zip)+1;
	char temp[512]={};
	char temp2[512][512]={};
	for(int i = 0,j = 0,k = 0,flag = 0;i<len;i++)
	{
		if(un_zip[i]=='<')
		{
			flag = 1;
		}
		if(un_zip[i]=='>')
		{
			un_zip[i] = 0;
			flag = 0;
		}
		if(un_zip[i]=='[')
		{
			flag = -1;
			un_zip[i] = 0;
		}
		if(un_zip[i]==']')
		{
			j = 0;
			un_zip[i] = 0;
			flag = 0;
			if(isdigit(temp[0]))
			{
				int a,b;
				sscanf(temp,"%d,%d",&a,&b);
				sprintf(temp2[k++],"\n[%02d:%02d.%02d]",a/60000,(a/1000)%60,(a-(a/1000)*1000)/10);
			}
			else
			{
				sprintf(temp2[k++],"\n[%s]",temp);
			}
			memset(temp,'\0',strlen(temp)+1);
		}
		if(flag)
		{
			if(flag==-1&&un_zip[i]&&j<500)
			{
				//单行歌词不超过500字节，相当于186个汉字或500个字母，否则作截断处理
				temp[j++] = un_zip[i];
			}
			un_zip[i] = 0;
		}
		
	}
	char*str = (char*)calloc(len,sizeof(char));
	for(int i = 0,k = 0,m = 0;i<len;i++)
	{
		if(i==0||un_zip[i]==10)
		{
			if(i)
				un_zip[i] = '\0';
			for(int j = 0,l = strlen(temp2[m++]);j<l;j++)
				str[k++] = temp2[m-1][j];
		}
		if(un_zip[i])
		{
			str[k++] = un_zip[i];
		}
	}
	for(int i = 0;i<len;i++)
	{
		str[i] = str[i+1];
	}
	
	// printf("%s",str);
	if(fwrite(str,strlen(str),1,fp)==1)
	{
		printf("转码成功！新文件写入 %s\n",convert_file);
	}
	fclose(fp);	
	free(str);
	free(zip_byte);
	free(un_zip);	
	free(file_path);
	free(decode_file);
	free(convert_file);
	return 0;
}

int file_size(const char*filename)
{
	struct stat statbuf;
	if (stat(filename,&statbuf)==-1)
		return -1;
	return statbuf.st_size;
}