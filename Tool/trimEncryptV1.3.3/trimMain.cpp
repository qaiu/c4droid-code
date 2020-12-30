/**
 * C语言代码压缩加密 v1.3.3
 * 
 * v1.3.2
 * 1.解决了正则匹配转义字符无法正常转换导致程序崩溃的bug
 * 2.解决了空格' '字符被去除导致加密后编译报错的bug
 * 3.解决小概率出现的内存溢出的问题
 * 4.使用随机密钥，并动态对密钥加密
 * 5.优化处理注释的算法，可以处理任意情况的注释
 * v1.3.3
 * 解决了无法处理字符串指针中有"[]"的情况
 * 
 * 已知问题: 1.源码里的main函数必须有参数, 2.不要使用宏定义字符串
 * 3.下列写法不支持 char *str={"xxx"}; char str[x]={};
 * 可以替换为这种形式: char *str="xxx"; char str[x];
 * @author QAIU
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>

#include "resources/aes.h"



#define BUFF_LEN 1024			// 缓冲行大小

#define IS_KEY_CHAR(c) (isalnum(c)||c=='_')

#define MALLOC(size,type) \
	((type*)malloc((size)*sizeof(type)))

#define CALLOC(size,type) \
	((type*)calloc(1,(size)*sizeof(type)))

/** 
 * 全局链表:
 *
 * data:字符串数据
 * ciphertext:密文
 * name:变量名
 * 
 * strListHead 头节点
 * strListNext 遍历节点
 * strListLast 临时节点
 */
struct DataList
{
	char *data;
	char *ciphertext;
	char name[8];
	struct DataList *next;
};

typedef struct DataList DataList;
typedef struct DataList* pDataList;

pDataList strListDel = NULL;
pDataList strListHead = NULL;
pDataList strListNext = NULL;
pDataList strListLast = NULL;

char* trimAES(FILE *in);

void deleteLinked(pDataList p)
{
	pDataList head = p;
	while (head != NULL)
	{
		p = head->next;
		free(head->data);
		free(head->ciphertext);
		free(head);
		head = p;
	}
}

/**
 * 获取路径里的文件名
 * 
 */
char *getPathFileName(char* path)
{
	char *fileName=strrchr(path,'/');
	return fileName == NULL?path:(fileName+1);
}

/**
 * 字符串替换
 * 注意数组下标越界
 *
 */
void replace(char *src, int start, int len, const char *sub, char *out)
{
	if (src == NULL || sub == NULL)
		return;
	int srclen = strlen(src);
	int sublen = strlen(sub);
	// if (sublen > len) return; 
	if (start + len > srclen)
	{
		// printf(">>>>>>>>>%s,error %d %d\n", src, srclen, start + len);
		return;
	}

	if (out != NULL)
		strncpy(out, src + start, len);

	char temp[srclen + sublen];
	memset(temp,0,srclen + sublen);
	strncpy(temp, src, start);
	strcat(temp, sub);
	strcat(temp, src + start + len);
	memset(src, 0, srclen);
	strcpy(src, temp);
}

// 获取文件大小
int get_file_size(FILE * file_handle)
{
	// 获取当前读取文件的位置 进行保存
	unsigned int current_read_position = ftell(file_handle);
	int file_size;
	fseek(file_handle, 0, SEEK_END);
	// 获取文件的大小
	file_size = ftell(file_handle);
	// 恢复文件原来读取的位置
	fseek(file_handle, current_read_position, SEEK_SET);
	return file_size;
}


/**
 * 合并分行宏定义
 * 请自行释放内存
 */
char *combineMacro(FILE *in,int *outSize)
{	
	int size = get_file_size(in);	
	// 单行代码不超过1024个字符(一个中文为3个字符)
	char *buff = CALLOC(BUFF_LEN,char);	// 行缓冲:
	char *buff2 = CALLOC(size+100,char);	// 文件缓冲:
	int flag = 0, len;
	char *temp;
	while (fgets(buff, BUFF_LEN, in) != NULL)
	{
		if (flag == 1) goto A;
		if (strchr(buff,'#')!=NULL)
		{
			temp = buff;
			while (buff[0]==0x20 || buff[0]=='\t') buff++;
			if (buff[0] == '#')
			{		
			A:  
				len = strlen(buff);
				//去除行末空格和Tab
				while (buff[len-2]==0x20 || buff[len-2]=='\t') len--; //bug写法:后面的len-2直接写了0(脑子呢?)
				buff[len] = '\0';
				buff[len-1] = '\n';
				
				if (buff[len-2]=='\\')
				{
					buff[len-2]='\0';
					strcat(buff2,buff);
					flag = 1;
					continue;
				}
				else 
				{//标志结束	
					strcat(buff2,buff);
					flag = 0;
					continue;
				}
			}
			else buff = temp;
		}
		strcat(buff2,buff);
	}//end
	*outSize = size;
	free(buff);
	return buff2;
}
 
/**
 * 转义引号字符
 * 
 */
char* escapeQuotationMarks(char *buff2,int size)
{
	//处理转义字符串
	char *temp, *start;
	char *buff3 = CALLOC(size+100, char);	// 文件缓冲:
	int k;
	if ((strstr(buff2+1,"\\\""))!=NULL)//bug写法,buff2写成了没有赋值的start
	{ 		
		temp = start = buff2;
		while ((start = strstr(start+1,"\\\""))!=NULL)
		{
			k = 1;
			while (*(start-k)=='\\') k++;
			
			if((k%2))
			{
				strncat(buff3,temp,start-temp+1);
				strcat(buff3,"x22");
				temp=start+2;
			}
		}
		strcat(buff3,temp);
		strcpy(buff2,buff3);
		memset(buff3,0,size+100);
	}//end
	free(buff3);
	return buff2;
}

/**
 * 合并分段字符串
 * 
 */
char* combineStr(char *buff2)
{
	char *start,*end;
	int flag, k;
	if ((start = strchr(buff2,'\"'))!=NULL)
	while ((end = strchr(start+1,'\"'))!=NULL)
	{
		if ((start = strchr(end+1,'\"'))!=NULL)
		{
			k = start - end;
			flag = 0;
			for (int i = 1; i < k; i++)
			{		
				if (*(end+i) != '\n'&&*(end+i) != '\x9'&&*(end+i) != 0x20)
				{
					flag = 1;
					break;
				}				
			}			
			if (flag==0)
			{
				strcpy(end,start+1);
				start = end;
				flag = 0;
			}			
		}
		else break;		
	}
	
	return buff2;
}
/**
 * 字符数组转指针
 * 
 */
char *strArray2Pointer(char *buff2)
{
	char *start,*end;
	if ((start = strstr(buff2,"char "))!=NULL)
	while ((end = strstr(start+1,";"))!=NULL)
	{	
		if (!IS_KEY_CHAR(*(start-1)))
		{
			if (*(end-1)=='\"')
			{
			//	puts((end-1));
				char temp[end - start+1];
				memset(temp,0,end - start+1);
				char *temp2=NULL;
				strncpy(temp,start,end-start);

				if(temp2=strstr(temp+1,"char "))
				{
					start = start+(temp2-temp)+1;
					continue;
				}
				
				char *pos=strstr(temp,"[");
				char *pos0=strstr(temp,"\"");
				if(pos!=NULL&&pos<pos0)
				{
					pos= start+(pos - temp);
					while (*pos != ']') (*pos++)=0x20;(*pos++)=0x20;
					*(start+4) = '*';
				}
			}
		}
		if(!(start=strstr(end+1,"char ")))
		break;
	} //end
	
	return buff2;	
}


/**
 * 提取字符串(使用全局链表存储，参考链表说明)
 * 
 */
char *extractStr(char *buff2,int size, char c)
{	
	char *buff3 = CALLOC(size+100, char);// 文件缓冲:
	char *start,*end,*temp;
	end = start = buff2;
	int num=0,k;
	while ((start = strstr(start+1,"\""))!=NULL)
	{
		//判断是否为头文件
		Start:
		k = 1;
		while (*(start-k)!='\n')
		{
			if (*(start-k++)=='#')
			{
				start = strstr(start+1,"\"");
				if ((start = strstr(start+1,"\""))!=NULL)
				{
					goto Start;
				}
				else goto B;
			}
		}//判断结束
		
		int flag = 0;
		temp = start;
		start = strstr(start+1,"\"");
		
		char *line = CALLOC(start - temp + 1,char);
		char *varName;
//		printf("%p\n",start-temp);
		strncpy(line,temp+1,start-temp-1);
//		puts(line);
		pDataList pTemp = strListNext;
		strListNext = strListHead;
		while (strListNext != NULL)
		{
			if (strListNext -> data != NULL)
			{
				if (strcmp(strListNext -> data,line) == 0)
				{
					flag = 1;
					varName = strListNext -> name;
					strListNext = pTemp;
					break;
				}
			}
			strListNext = strListNext -> next;
		}
		strListNext = pTemp;
		
		if (flag == 1)
		{
			strncat(buff3,end,temp-end);
			strcat(buff3,varName);
			end=start+1;//+strlen(varName);
			continue;
		}
		
		if (strListHead == NULL)
		{
			strListHead = strListNext = CALLOC(1,DataList);
		}
		else
		{
			strListNext ->next = CALLOC(1,DataList);
			strListNext = strListNext -> next;
		}		
//		puts(line);
		strListNext -> next = NULL;
		strListNext -> data = line;
		sprintf(strListNext -> name,"%c%02x",c,num++);
		
		strncat(buff3,end,temp-end);
		strcat(buff3,strListNext -> name);
		end=start+1;	
	}
	B:
	strcat(buff3,end);
	free(buff2);
	strListNext = strListHead; 
	return buff3;
}


/**
 * 去除多余的空格字符 (一键丑化代码(๑• . •๑))
 * 0,处理空格
 * 1,处理空格换行
 */
char* trimSpace(char* data,int size,int mode)
{
	int k = mode;
	const char* splitStr;
	if (k < 2)
		splitStr = "\x9 ";
	else
		splitStr ="\n";
	
	char* dest = CALLOC(size+100,char);
	char* last;
	char* next;
	if (data[0]=='#')
	{
		strcat(dest,"\n");
		strcat(dest,data);
		strcpy(data,dest);
		memset(dest,0,size+100);
	}
	
	
	last = strtok(data,splitStr);
	while ((next = strtok(NULL,splitStr)) != NULL)
	{
		strcat(dest, last);
		
		if (last[0] == 0x23||next[0] == 0x23)
		{
			strcat(dest, "\n");
		}
		else
		{
			int len = strlen(last);
			if(IS_KEY_CHAR(last[len-1]) && IS_KEY_CHAR(next[0]))
			{
				strcat(dest," ");
			}
		}
		last = next;
	}
	strcat(dest, last);
	strcpy(data,dest);
	free(dest);
	if (mode == 1)
	data = trimSpace(data,size,mode+1);
	return data;
}

//处理char空格
char *changeSpaceChar(char* buff)
{
	char *temp = buff;
	while ((temp=strstr(temp,"' '"))!=NULL)
	{
		temp[0] = '0';
		temp[1] = '4';
		temp[2] = '0';
		temp+=1;	
	}
	return buff;	
}

// 处理注释
void trimNotation(FILE * in, FILE * out)
{
	char buff[BUFF_LEN] = { };	// 行缓冲:
	// 单行代码不超过1024个字符(一个中文为3个字符)
	int flag = 0;
	while (fgets(buff, BUFF_LEN, in))
	{
		char *singleStart = strstr(buff, "\x2f\x2f");
		char *multipleStart = strstr(buff, "\x2f\x2a");
		char *multipleEnd;
		char *quotationMarksStart;
		char *quotationMarksEnd;
		
		if (singleStart != NULL)
		{
			quotationMarksStart = strchr(buff,0x22);
			quotationMarksEnd = strrchr(buff,0x22);		
			if (singleStart > quotationMarksStart&&singleStart < quotationMarksEnd)
				singleStart = strstr(quotationMarksEnd, "\x2f\x2f");
		}
		
		if (flag == 1)
		{
			// puts("处理末尾多行注释");
			if ((multipleEnd = strstr(buff, "\x2a\x2f")) != NULL)
			{
				char temp[512] = { };
				strcpy(temp, multipleEnd + 2);
				memset(buff, 0, 1024);
				strcpy(buff, temp);

				flag = 0;
			}
			else
			{
				buff[0] = 0x0;
			}
		}
		else if (singleStart != NULL || multipleStart != NULL)
		{
			if (multipleStart > singleStart)
			{
				if (singleStart == NULL)
				{
				  A:
					if ((multipleEnd = strstr(buff, "\x2a\x2f")) != NULL)
					{
						char temp[512] = { };
						strncpy(temp, buff, multipleStart - buff);
						strcat(temp, multipleEnd + 2);
						// printf("\n%s\n>>>>%s\n", buff, temp);
						memset(buff, 0, 1024);
						strcpy(buff, temp);
						// puts(buff);
					}
					else
					{
						flag = 1;
						multipleStart[0] = 0x0a;
						multipleStart[1] = 0x00;
					}
				}
				else
					goto B;
			}
			else
			{
				if (multipleStart == NULL)
				{
				  B:
					singleStart[0] = 0x0a;
					singleStart[1] = 0x00;
				}
				else
					goto A;
			}
		}
		// puts(buff);
		fputs(buff, out);
	}
}

char* trimAES(FILE *in)
{
	int size;
	char *buff2 = combineMacro(in,&size); //合并宏定义
	buff2=escapeQuotationMarks(buff2,size); //转义双引号
	buff2=combineStr(buff2); //合并字符串
	buff2=strArray2Pointer(buff2); 
	buff2=extractStr(buff2,size,'x'); //提取
	buff2=trimSpace(buff2,size,1);  
	return buff2;
}

//生成随机密钥
char* getRandKey(char *key,int len)
{
	char b64_table[64];
	int k=0;
	for (int i = 0x41; i >= 0x21; i--)
		b64_table[k++] = i;
		
	for (int i = 0x44; i <= 0x60; i++)
		b64_table[k++] = i;
	b64_table[k++] = 0x7b;
	b64_table[k++] = 0x7d;

	b64_table[57] = 0x64;
	b64_table[31] = 0x69;
	
	srand(clock());
	for (int i = 0; i < len; i++)
		key[i] = b64_table[(rand() % 60)];
	return key;
}


/*
void touppers(char *str,char* dest)
{
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '.')
		dest[i] = '_';
		else if(str[i]>='a'&&str[i]<='z')
		dest[i]=str[i]+'A'-'a';
		else dest[i] = str[i];
	}
	
}
*/

/**
 * 加密字符串 写入本文文件
 * 返回处理后的文本
 */
char *encryptStr(char *buff2,int size,const char* file)
{
	char *buff3 = CALLOC(size+100,char);// 文件缓冲:
	
	char *keyName = MALLOC(128,char);
	strcpy(keyName,file);
	strcat(keyName,".key");
	
	char *destName = MALLOC(128,char);
	strcpy(destName,file);
	strcat(destName,".out.cpp");
	
	FILE *out;
	if ((out = fopen(keyName,"w"))==NULL)
	{
		perror("open file");
		return NULL;
	}
	while (strListNext)
	{
		strListNext->ciphertext = encrypt(strListNext->data);
		strListNext=strListNext->next;
	}
	strListNext=strListHead;
	char name[64] = {};
	//puts(getPathFileName(keyName));
//	touppers(getPathFileName(keyName),name);
//	touppers(keyName,name);
	fprintf(out,"#ifndef %s\n","KEY_FILE");
	fprintf(out,"#define %s\n","KEY_FILE");
	memset(name,0,strlen(name));
	
	while ((strListNext))
	{
		fprintf(out,"char %s[] = \"%s\";\n",strListNext->name,strListNext->ciphertext);
	//	printf("%s:%s%d\n",strListNext->name,strListNext->ciphertext,strlen(strListNext->ciphertext));
		//puts(strListNext->ciphertext);
		strListNext=strListNext->next;
	}
	
	strListLast = strListDel = strListHead;
	strListNext = strListHead = NULL;
	
	FILE *aesFile;
	if ((aesFile=fopen("resources/aes.h","r"))==NULL)
	{
		perror("open file");
		return NULL;
	}
	char *temp = trimAES(aesFile);
	strListNext = strListHead;
	
	free(strListNext->data);
	char *key=CALLOC(128,char);
	strcpy(key,tempkey);
	strListNext->data = key;
	
	while(strListNext)
	{
		fprintf(out,"char %s[] = \"%s\";\n",strListNext->name,strListNext->data);
		strListNext=strListNext->next;
	}
	fputs(temp,out);
	free(temp);
	
	//fprintf(out,"\n#include \"%s\"\n",getPathFileName(destName));
	fprintf(out,"\nint Main(int,char**);\n");
	fprintf(out,"void decoder()\n");
	fprintf(out,"{\n");
	fprintf(out,"	initEncoder();\n");
	while ((strListLast))
	{
		fprintf(out,"	_____(%s);\n",strListLast->name);
		strListLast=strListLast->next;
	}
//	deleteLinked();
	fprintf(out,"}\n");
	fprintf(out,"int main(int n,char **s)\n");
	fprintf(out,"{\n");
	fprintf(out,"    decoder();\n");
	fprintf(out,"    Main(n,s);\n");
	fprintf(out,"}\n");
	fprintf(out,"\n#endif\n");
	fclose(out);
	
	char buffLine[512] = {};
	sprintf(buffLine,"#include \"%s\"\n",getPathFileName(keyName));
	strcat(buff3,buffLine);
	
//	touppers(getPathFileName(destName),name);
	sprintf(buffLine,"#ifndef %s\n","DEST_FILE");
	strcat(buff3,buffLine);
	sprintf(buffLine,"#define %s\n","DEST_FILE");
	strcat(buff3,buffLine);
	memset(name,0,strlen(name));
	if ((temp=strstr(buff2,"int main("))!=NULL)	
	temp[4] = 'M';
	
	strcat(buff3,buff2);
	strcat(buff3,"\n#endif");
	free(buff2);
	free(keyName);
	free(destName);
	return buff3;
}

/**
 * 处理加密主函数
 * 
 */
int trimMain(const char *inFile)
{
	FILE *in;
	FILE *out;
	if ((in = fopen(inFile, "r")) == NULL)
	{
		perror("open file1");
		return -1;
	}
	
	char *tempFile = MALLOC(128,char);
	strcpy(tempFile,inFile);
	strcat(tempFile,".temp");
	
	if ((out = fopen(tempFile, "w")) == NULL)
	{
		perror("open file2");
		return -1;
	}
	
	trimNotation(in, out);
	fclose(in);
	fclose(out);
	
	if ((in = fopen(tempFile,"r")) == NULL)
	{
		perror("open file3");
		return -1;
	}
	
	int size;
	//处理多行宏定义
	char *buff2 = combineMacro(in,&size);
	fclose(in);
	
	//处理转义字符串
	buff2=escapeQuotationMarks(buff2,size);
	
	//处理分段字符串
	buff2=combineStr(buff2);
	
	//含值的字符数组转指针
	buff2 = strArray2Pointer(buff2);
	
	//提取字符串
	buff2=extractStr(buff2,size,'_');
	
	//转换' '(空格) char类型符号为八进制为去除空格做准备
	buff2=changeSpaceChar(buff2);
	//去除空格
	buff2=trimSpace(buff2,size,1);
	
	//执行加密
	buff2=encryptStr(buff2,size,inFile);
		
	char *destName = MALLOC(128,char);
	
	strcpy(destName,inFile);
	strcat(destName,".out.cpp");
	if ((out=fopen(destName,"w"))==NULL)
	{
		perror("error 03 Open writeFile-dest fail");
		return -1;
	}
	
	fputs(buff2,out);
	free (buff2);
	free (destName);
	
	deleteLinked(strListDel);
	strListHead=strListNext=strListLast=NULL;
	fclose(out);
	
	if (remove(tempFile) != 0)
		perror("warning Remove tempfile fail");
	free (tempFile);
	return 0;
}

int batchProcess(char *filePath)
{
	int filesize = 0;
	DIR *dir = NULL;
	struct dirent *entry;
	char fileName[128] = {};
	int i = 1;

	if ((dir = opendir(filePath)) == NULL)
	{
		perror("打开路径失败!");
		return -1;
	}
	while (entry = readdir(dir))
	{
		if (entry->d_type == 8)
		{
			
			int size = strlen(entry->d_name); 
			
			if (((
			strcmp((entry->d_name + (size - 8)), ".out.cpp") != 0)&&
			strcmp((entry->d_name + (size - 4)), ".cpp") == 0)||
			strcmp((entry->d_name + (size - 2)), ".c") == 0||
			strcmp((entry->d_name + (size - 3)), ".cc") == 0)
			{
				printf("> 正在处理%d:%s \n", i, entry->d_name);	// 输出文件或者目录的名称
				sprintf(fileName,"%s%s",filePath,entry->d_name);
				if (trimMain(fileName) != 0)
				{
					puts("\e[31;1m致命错误，任务已终止!");
					return -1;
				}
				i++;
			}
		}
	}

	closedir(dir);
	return 0;
}

int main(int argc, char **argv)
{	
	tempkey=getRandKey(_0_key,127);
	initEncoder();
	
	puts("\e[33;1m> QAIU源码字符串加密v1.3.3\e[0m\n");
	puts("━━━━━━━━━━━━━━━\n");
	puts("> \e[32m如果是目录名则做批量转换\e[0m");
	puts("> \e[32m如果是文件名,后缀必须是.c,.cpp,.cc中的一种\e[0m");
	puts("> \e[32m源码必须含有带参main函数\e[0m");
	
	while (1)
	{
		puts("> \e[32;1m请输入源文件绝对路径: \e[0m");
		printf("> ");
		char path[128]={};
		scanf("%s",path);
	
	
		int size = strlen(path); 
		int state;
		if (access(path,0)==0&&
			(strcmp((path + (size - 4)), ".cpp") == 0||
		strcmp((path + (size - 2)), ".c") == 0||
		strcmp((path + (size - 3)), ".cc") == 0))
		state = trimMain(path);
		else
			state = batchProcess(path);
	
		puts("\n━━━━━━━━━━━━━━━");
		if (state == 0)
			puts("\e[36;1m>> 已完成\e[0m");
		else 
			puts("\e[31;1m>> 未完成...\e[0m");
		puts("\n");
	}
}