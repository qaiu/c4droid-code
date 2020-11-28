/*c4droid代码手册
 *文件浏览器
 *炼狱火雨编写
 *TTHHR收集整理
 *转载请说明出处
    *终端模拟器字体大小设置为12点阵
    */
#include"stdio.h"
#include"conio.h"
#include"dirent.h"
#include"string.h"
#include"stdlib.h"


#define N 25

struct files
{
	int num;
	unsigned char type;
	char sname[200];
	char lname[250];
	struct files *next;
	struct files *last;
} *HEAD = NULL, *TAIL = NULL;
char addr[250] = "/sdcard/";
DIR *dir;

struct files *getifo(const int gl);
void ranklist(void);
int addfilelist(int number, unsigned char type, char sn[], char ln[]);
int addfilelist(int number, unsigned char type, char sn[], char ln[]);
int list(const int gl);
void destroylist(void);
int menu(const int gl);
int manage(const int gl, const char c);
int strwide(const char a[]);
char *shortstr(const char oldstr[]);
int control(const char ch, int *gl);
void drawf(const int x, const int y, const int w, const int h);
void clrxy(int x, int y, int w, int h);
int rm(char a[]);

int main()
{
	int number;
	int gl = 1, t;
	char tmp[250];
	char ch;

	struct dirent *dnt;

	printf("\033[?25l");

	while (1)
	{
		strcpy(tmp, addr);
		if (NULL == (dir = opendir(addr)))
		{
			printf("can't open %s", addr);
			exit(0);
		}
		number = 1;

		while (NULL != (dnt = readdir(dir)))
		{

			if (strcmp(dnt->d_name, ".") == 0)
				continue;

			addfilelist(number, dnt->d_type, dnt->d_name,
						strcat(tmp, dnt->d_name));
			strcpy(tmp, addr);

			number++;

		}

		ranklist();
		ch = 0;

		while ('5' != ch && '1' != ch)
		{
			clrscr();
			list(gl);
			printf("\n\033[32m提示:使用数字键盘控制：\n"
				   "28选择，46翻页，5确定，1开关菜单，0退出\033[0m");
			ch = getch();
			if ('1' == ch && 1 == gl)
				ch = 0;
			else control(ch, &gl);

		}
	}
}

int control(const char ch, int *gl)
{
	switch (ch)
	{
	case '1':
		if (1 == *gl)
			break;
		menu(*gl);
		closedir(dir);
		destroylist();
		break;
	case '2':
		(*gl)--;
		if (*gl < 1)
			*gl = TAIL->num;
		break;
	case '8':
		(*gl)++;
		if (*gl > TAIL->num)
			*gl = 1;
		break;
	case '6':
		if (*gl == TAIL->num)
			*gl = 1;
		else if (*gl + 10 <= TAIL->num)
			(*gl) += 10;
		else
			*gl = TAIL->num;
		break;
	case '4':
		if (*gl == 1)
			*gl = TAIL->num;
		else if (*gl - 10 >= 1)
			(*gl) -= 10;
		else
			*gl = 1;
		break;
	case '0':
		closedir(dir);
		clrscr();
		printf("END!");
		system("input keyevent 4");
		exit(0);

	case '5':

		if (getifo(*gl)->type == 4)
		{
			strcat(addr, getifo(*gl)->sname);
			*gl = 1;
		}
		closedir(dir);
		destroylist();
		ranklist();
	}
	return *gl;

}

int list(const int gl)
{
	int i;
	struct files *p = HEAD;
	char ssname[250], gstr[250];
	char *fs;
	printf("\033[32;4;5m当前路径：%s\033[0m\n", addr);
	if (gl > 7)
	{
		for (i = 1; i < gl - 7; i++)
			p = p->next;
	}
	i = 1;
	do
	{
		strcpy(ssname, p->sname);

		if (strwide(p->sname) > N)
		{
			fs = shortstr(p->sname);
			strcpy(ssname, fs);
			free(fs);
		}

		if (gl == (p->num))
		{
			printf("\033[41;33;4;5m%s\033[0m\n", ssname);
			strcpy(gstr, p->lname);
		}
		else
			printf("%s\n", ssname);
		i++;
	}
	while ((p = p->next) != NULL && i < 14);
	gotoxy(1, 15);
	printf("\n\033[36m选中：\033[41;37m%s\033[0m", gstr);

	return 0;
}

int addfilelist(int number, unsigned char type, char sn[], char ln[])
{
	struct files *p;
	p = (struct files *)malloc(sizeof(struct files));
	p->num = number;
	p->type = type;
	strcpy(p->sname, sn);
	strcpy(p->lname, ln);
	if (4 == type && strcmp(sn, "..") != 0)
	{
		strcat(p->sname, "/");
		strcat(p->lname, "/");
	}
	if (NULL != TAIL)
	{
		TAIL->next = p;
		p->last = TAIL;
		p->next = NULL;
		TAIL = p;
	}
	else
	{
		HEAD = TAIL = p;
		p->last = NULL;
		p->next = NULL;
	}
	return 0;
}

void ranklist(void)
{
	int tmp;
	struct files *c = HEAD, *a, *b, *d;
	if (NULL != c)
	{
		while (NULL != (c = c->next))
		{
			if (c->type < c->last->type)
			{

				b = c->last;
				d = c->next;
				a = b->last;
				b->next = d;
				b->last = c;
				c->next = b;
				c->last = a;
				if (NULL != a)
					a->next = c;
				if (NULL != d)
					d->last = b;
				if (c == TAIL)
					TAIL = b;
				if (b == HEAD)
					HEAD = c;
				c = HEAD;
			}

		}
		c = HEAD;
		tmp = 1;
		do
		{
			c->num = tmp;
			tmp++;
		}
		while (NULL != (c = c->next));
		if ((tmp = strlen(HEAD->lname)) > 3)
		{
			while (HEAD->lname[tmp - 4] != '/')
				tmp--;
			HEAD->lname[tmp - 3] = '\0';
		}
		else
			HEAD->lname[1] = '\0';
	}


	tmp = strlen(addr) - 2;
	if (strcmp(&addr[tmp], "..") == 0)
	{

		if (tmp + 2 > 3)
		{
			tmp -= 2;
			while ('/' != addr[tmp])
				tmp--;
			addr[tmp + 1] = '\0';
		}
		else
		{
			addr[tmp] = '\0';
		}
	}

}

struct files *getifo(const int gl)
{
	struct files *p = HEAD;
	while (gl != p->num)
	{
		p = p->next;
		if (NULL == p)
			return NULL;
	}
	return p;
}

void destroylist(void)
{
	struct files *pa, *pb;
	pb = HEAD;
	do
	{
		pa = pb;
		pb = pb->next;
		free(pa);
	}
	while (NULL != pb);
	HEAD = TAIL = NULL;
}

int menu(const int gl)
{
	int select = 1, done = 1;
	char ch = 0, c;
	while (0 != done)
	{
		ch = 0;
		while ('5' != ch)
		{
			printf("\033[36;1m\033[6;30H┌──────┐\n");
			printf("\033[7;30H│ 打开 │\n");
			printf("\033[8;30H│ 删除 │\n");
			printf("\033[9;30H│ 复制 │\n");
			printf("\033[10;30H│ 剪切 │\n");
			printf("\033[11;30H│ 粘贴 │\n");
			printf("\033[12;30H│重命名│\n");
			printf("\033[13;30H└──────┘\n");

			printf("\033[41;33m");
			switch (select)
			{
			case 1:
				printf("\033[7;31H 打开 \n");
				c = 'o';
				break;
			case 2:
				printf("\033[8;31H 删除 \n");
				c = 'd';
				break;
			case 3:
				printf("\033[9;31H 复制 \n");
				c = 'c';
				break;
			case 4:
				printf("\033[10;31H 剪切 \n");
				c = 't';
				break;
			case 5:
				printf("\033[11;31H 粘贴 \n");
				c = 'p';
				break;
				case 6:
				printf("\033[12;31H重命名\n");
				c = 'r';
				break;
				
			}
			printf("\033[0m");
			ch = getch();
			switch (ch)
			{
			case '1':
				return 0;
			case '5':
				done = manage(gl, c);
				break;
			case '2':
				select--;
				if (0 == select)
					select = 6;
				break;
			case '8':
				select++;
				if (7 == select)
					select = 1;
				break;
			}
		}
	}
	return 0;
}

int strwide(const char a[])
{
	int i = 0, num = 0;
	while ('\0' != a[i])
	{

		if (a[i] >> 4 == 14)
		{
			if (a[i + 1] >> 6 == 2)
			{
				if (a[i + 2] >> 6 == 2)
				{
					num += 2;
					i += 3;
				}
				else
					return -1;
			}
			else
				return -1;
		}
		else if (a[i] >> 5 == 6)
		{
			if (a[i + 1] >> 6 == 2)
			{
				num++;
				i += 2;
			}
			else
				return -1;
		}
		else if (a[i] >> 6 == 2)
		{
			return -1;
		}
		else
		{
			num++;
			i++;
		}

	}
	return num;
}

char *shortstr(const char oldstr[])
{
	char form[10];
	char *tmp = (char *)malloc(250);
	int i;

	strcpy(tmp, oldstr);
	i = strlen(tmp) - 1;
	while (strwide(tmp + i) < 5)
		i--;
	strcpy(form, &tmp[i]);
	do
	{
		tmp[i] = '\0';
		i--;
	}
	while (strwide(tmp) < 0 || strwide(tmp) > N - 8);
	strcat(tmp, "...");
	strcat(tmp, form);
	return tmp;
}

void drawf(const int x, const int y, const int w, const int h)
{
	int ex, i;
	ex = x + w + 1;
	gotoxy(x, y);
	printf("┌");
	for (i = 0; i < w; i++)
		printf("─");
	printf("┐");
	gotoxy(x, y + 1);
	for (i = 0; i < h; i++)
		printf("│\b\033[1B");
	printf("└");
	for (i = 0; i < w; i++)
		printf("─");
	printf("┘");
	gotoxy(ex, y + 1);
	for (i = 0; i < h; i++)
		printf("│\b\033[1B");
}
int manage(const int gl, const char c)
{								
    struct File
    {
    	char filename[300];
    	char cmd;   	    	
    };
	int select = 1;
	char ch = 0, newname[300], tmp[300];
	static struct File file;
	if ('d' == c)
	{

		do
		{
			drawf(40, 8, 6, 2);
			gotoxy(41, 9);
			if (1 == select)
			{
				printf("\033[41m 确定 \033[0m");
				printf("\033[10;41H 取消 ");
			}
			else
			{
				printf("\033[0m 确定 ");
				printf("\033[41m\033[10;41H 取消 \033[0m");
			}
			ch = getch();
			if ('2' == ch)
				select--;
			else if ('8' == ch)
				select++;
			if (0 == select)
				select = 2;
			if (3 == select)
				select = 1;
		}
		while ('1' != ch && '5' != ch);
		clrxy(40, 8, 8, 4);
		if ('1' == ch || 2 == select)
		{
			return -1;
		}
		if ('5' == ch && 1 == select)
		{
			gotoxy(40, 9);
			if (rm(getifo(gl)->lname) != 0)
			{
				printf("删除失败！");
				getch();
				clrxy(40, 9, 10, 1);
				return -1;
			}
			else
			{
				printf("删除成功！");
				getch();
				return 0;
			}
		}

	}
	if ('r' == c)
	{
		clrscr();
		printf("\033[31m重命名：\033[0m%s\n\n", getifo(gl)->sname);
		printf("\033[31m为：\033[0m\n\n\n\n\n");
		printf("\033[32m取消请输入\033[0m/\033[?25h");
		printf("\r\033[3A");
		 gets(newname);
		printf("\033[?25l");
		gotoxy(20,10);
		if (strcmp(newname, "/") != 0)
		{
			strcpy(tmp, addr);
			strcat(tmp, newname);
			if (rename(getifo(gl)->lname, tmp) == 0)
			{
				printf("\033[31m重命名成功！\033[0m");
				getch();
				return 0;
			}
			else
			{
				printf("\033[31m重命名失败！\033[0m");
				getch();
				return 0;
			}
		}
	}
	if('c'==c)
	{
		strcpy(file.filename,getifo(gl)->lname);
		file.cmd='c';
		return 0;		
	}
	if('t'==c)
	{
		strcpy(file.filename,getifo(gl)->lname);
		file.cmd='t';		
		return 0;		
	}
	
	return -1;
}

void clrxy(int x, int y, int w, int h)
{
	int i, j;
	gotoxy(x, y);
	for (i = 1; i <= h; i++)
	{
		for (j = 1; j <= w; j++)
		{
			printf(" ");
		}
		gotoxy(x, y + i);
	}

}
int rm(char a[])
{
	DIR *dir;
	struct dirent *dnt;
	char tmp[300];
	if (remove(a) == 0)
		return 0;
	else
	{
		if((dir = opendir(a))==NULL)
		return -1;
		while ((dnt = readdir(dir)) != NULL)
		{
			if (strcmp(".", dnt->d_name) == 0)
				continue;
			if (strcmp("..", dnt->d_name) == 0)
				continue;
			strcpy(tmp, a);
			strcat(tmp, dnt->d_name);
			if (4 == dnt->d_type)
				strcat(tmp, "/");
			rm(tmp);
		}
		if(remove(a)==0)
		return 0;
		else
		return -1;
	}
}