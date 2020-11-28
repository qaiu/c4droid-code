#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// 1^1=0
// 1^0=1
// 0^0=0

// unicode 				UTF-8
// 0000 0000 - 0000 007F 0xxxxxxx //7
// 0000 0080 - 0000 07FF 110xxxxx 10xxxxxx //13
// 0000 0800 - 0000 FFFF 1110xxxx 10xxxxxx 10xxxxxx //14
// 0001 0000 - 0010 FFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx //15
//					   F(0-7) (8-B)(0-F)(8-B)(0-F)(8-B)(0-F)

// 11100100 10111000 10100101
// E0
// C0
//


//	 1111   111111   111111
// 1110xxxx 10xxxxxx 10xxxxxx //14

int utf2uni(const char *s, int *uni)
{
	if (s == NULL)
		return -1;
	char head;

	int cont = 0;
	while (*s)
	{
		head = s[0];
		head = head >> 4;
		// printf(">>%d\n",head);
		if (head < 8)
			uni[cont++] = s[0], s++;
		else if (head < 14)
			uni[cont++] = ((s[0] ^ 0xc0) << 6) + (s[1] ^ 0x80), s += 2;
		else if (head < 15)
			uni[cont++] = ((s[0] ^ 0xe0) << 12) + ((s[1] ^ 0x80) << 6) + (s[2] ^ 0x80), s += 3;
		else if (head < 16)
			uni[cont++] =
				((s[0] ^ 0xf0) << 18) + ((s[1] ^ 0x80) << 12) + ((s[2] ^ 0x80) << 6) +
				(s[3] ^ 0x80), s += 4;
	}							// */

	return cont;
}

int uni2utf(int *uni, const char *s)
{	
	if (s == NULL)
		return -1;
	int cont=0;
	
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
	char unicodetemp[32]={};
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
		strcpy(unicodetemp,"0x");
		sscanf(buf, "%s %s",unicodetemp+2, buf2);
		sscanf(buf,"%X",&pynext->unicode);
		pynext->py = (char *)malloc(strlen(buf2)+1);
		strcpy(pynext->py,buf2);
	}
	pynext=pyhead;
}

char* u2py(const char *str,char *sep)
{
	int len = (int)mbstowcs(NULL, str, 0);
	int len2=0;
	int size=0;
	int unicode[len]={};
	char py[len][6] = {};
	utf2uni(str,unicode);
	while(pynext)
	{
		for (int i=0; i<len;i++)
		{
			if (unicode[i]==pynext->unicode)
			{
				strcpy(py[i],pynext->py);
				size+=strlen(pynext->py)+1;
			}
		}
		pynext=pynext->next;
	}
	pynext=pyhead;
	
	
	for (int i=0; i<len;i++)
	{
		if(unicode[i]>0x9fc3||unicode[i]<0x4e00)
		{
			strcpy(py[i],"@");
			size+=4;
		}
	}
	
	char *py2=new char[size+1];
	for (int i=0; i<len;i++)
	{
		strcat(py2,py[i]);
		if (sep!=NULL)
		strcat(py2,sep);
		else
		strcat(py2," ");
	}
	return py2;
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

void closeu2py(char *p)
{
	delete_list(pynext);
	delete []p;
	pyhead = pynext = NULL;
}
int main(int argc, char **argv)
{
	initu2py();
	char *py=u2py("老将出马，一个顶俩"," ");
	printf(py);
	return 0;
}