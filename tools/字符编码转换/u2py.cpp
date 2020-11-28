/* 
 * 1. unicode--UTF-8相互转换
 * 2. 汉字注音
 * by QAIU
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>
// unicode 转 UTF8

// 0000 0000 - 0000 007F 0xxxxxxx //7
// 0000 0080 - 0000 07FF 110xxxxx 10xxxxxx //13
// 0000 0800 - 0000 FFFF 1110xxxx 10xxxxxx 10xxxxxx //14
// 0001 0000 - 0010 FFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx //15

int utf2uni(const char *s, int *uni)
{
	if (s == NULL)
		return -1;
	char head;

	int cont = 0;
	while (*s)
	{
		head = s[0]>>4;
		if (head < 8)
			uni[cont++] = s[0], s++;
		else if (head < 14)
			uni[cont++] = ((s[0] ^ 0xc0) << 6) + (s[1] ^ 0x80), s += 2;
		else if (head < 15)
			uni[cont++] = ((s[0] ^ 0xe0) << 12) + ((s[1] ^ 0x80) << 6) + (s[2] ^ 0x80), s += 3;
		else if (head < 16)
			uni[cont++] = ((s[0] ^ 0xf0) << 18) + ((s[1] ^ 0x80) << 12) + ((s[2] ^ 0x80) << 6) + (s[3] ^ 0x80), s += 4;
	}							// */

	return cont;
}

void uni2uc(int uni, char *s)
{
	if (uni < 0x80)
		s[0] = uni;
	else if (uni < 0x800)
	{
		s[0] = uni >> 6 ^ 0xc0;
		s[1] = uni ^ (uni >> 6 << 6) ^ 0x80;
	}
	else if (uni < 0x10000)
	{
		s[0] = uni >> 12 ^ 0xE0;

		s[1] = (uni ^ uni >> 12 << 12) >> 6 | 0x80;
		s[2] = uni ^ (uni >> 6 << 6) | 0x80;
	}
	else
	{
		s[0] = uni >> 18 ^ 0xf0;
		s[1] = (uni ^ uni >> 18 << 18) >> 12 | 0x80;
		s[2] = (uni ^ uni >> 12 << 12) >> 6 | 0x80;
		s[3] = uni ^ (uni >> 6 << 6) | 0x80;
	}
}

int uni2utf(int *uni, char *s)
{
	if (uni == NULL)
		return -1;

	int cont = 0;
	while (uni[cont])
	{
		if (uni[cont] < 0x80)
			s++[0] = uni[cont];
		else if (uni[cont] < 0x800)
			
		{
			(s++)[0] = uni[cont] >> 6 ^ 0xc0;
			(s++)[0] = uni[cont] ^ (uni[cont] >> 6 << 6) ^ 0x80;
		}

		else if (uni[cont] < 0x10000)
		{
			(s++)[0] = uni[cont] >> 12 ^ 0xE0;
			(s++)[0] = (uni[cont] ^ uni[cont] >> 12 << 12) >> 6 | 0x80;
			(s++)[0] = uni[cont] ^ (uni[cont] >> 6 << 6) | 0x80;
		}
		else
		{
			(s++)[0] = uni[cont] >> 18 ^ 0xf0;
			(s++)[0] = (uni[cont] ^ uni[cont] >> 18 << 18) >> 12 | 0x80;
			(s++)[0] = (uni[cont] ^ uni[cont] >> 12 << 12) >> 6 | 0x80;
			(s++)[0] = uni[cont] ^ (uni[cont] >> 6 << 6) | 0x80;
		}
		// */
		cont++;
	}
}

struct pylist
{
	int unicode;
	char *py;

	pylist *next;
} *pyhead, *pynext;

int initu2py()
{
	char buf[64] = { };
	char buf2[32] = { };
	char pytemp[32] = { };
	char unicodetemp[32] = { };
	FILE *fp = fopen("Pinyin2.txt", "r");

	while (fgets(buf, 64, fp))
	{
		if (pyhead == NULL)
			pyhead = pynext = new pylist;
		else
		{
			pynext->next = new pylist;
			pynext = pynext->next;
		}
		strcpy(unicodetemp, "0x");
		sscanf(buf, "%s %s", unicodetemp + 2, buf2);
		sscanf(buf, "%X", &pynext->unicode);
		pynext->py = (char *)malloc(strlen(buf2) + 1);
		strcpy(pynext->py, buf2);
	}
	pynext = pyhead;
}

int u2py(char *py2,const char *str, char *sep)
{
	int len = (int)mbstowcs(NULL, str, 0);
	int len2 = 0;
	int size = 0;
	int unicode[len] = { };
	char py[len][6] = { };
	utf2uni(str, unicode);

	while (pynext)
	{
		for (int i = 0; i < len; i++)
		{
			if (unicode[i] == pynext->unicode)
			{
				strcpy(py[i], pynext->py);
				size += strlen(pynext->py) + 1;
			}
		}
		pynext = pynext->next;
	}
	pynext = pyhead;

	for (int i = 0; i < len; i++)
	{
		if (unicode[i] > 0x9fc3 || unicode[i] < 0x4e00)
		{
			char str[8] = { };
			// printf("%d\n",unicode[i]);
			uni2uc(unicode[i], str);
			// puts(str);
			strcpy(py[i], str);
			size += 4;
		}
	}

//	char *py2 = (char*)malloc(size + 1);

	for (int i = 0; i < len; i++)
	{
		strcat(py2, py[i]);
		if (unicode[i] <= 0x9fc3 && unicode[i] >= 0x4e00)
		{
			if (sep != NULL)
				strcat(py2, sep);
			else
				strcat(py2, " ");
		}
	}
//	return py2;
}

template <class T> 
void delete_list(T * &list)
{
	T *head;
	while (list)
	{
		head = list->next;
		free(list);
		list = head;
	}
}

void closeu2py()
{
	delete_list(pynext);
	pyhead = pynext = NULL;
}
void fun()
{
	char str[64]={};
	initu2py();
	u2py(str,"看到了吗，没有,呵呵老将出马,一个顶俩...", "");

	printf(str);
	closeu2py();
/*
	int a;
	utf2uni("一", &a);
	printf("%x\n",a);
	return 0;
//*/
}

int main(int argc, char **argv)
{
	initu2py();
	FILE *fp=fopen("cy5w.txt","r");
	FILE *fp2=fopen("data4.txt","w");
	char buf[64]={};
	char py[64]={};
	
	fun();
	
	closeu2py();
/*	printf("%d\n",sizeof(py));/*
	while(fgets(buf,64,fp))
	{
		usleep(1);
		memset(py,0,sizeof (py));
		u2py(py,buf,"'");
		py[strlen(py)-2]=0;
		buf[strlen(buf)-1]=0;
		
		fprintf(fp2,"%s %s\n",buf,py);
	//	py[strlen(py)-1]=0;
		
//		puts(py);
//		free( py);
//		py=NULL;
		//sscanf(buf,"%s%s%*s",cy);
	}
	fclose(fp);
	fclose(fp2);
	//*/
}