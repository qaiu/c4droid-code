/*
c4droid代码手册

*********************
~~~~~文本加密器，纯c~~~
*****By 啊o额iu鱼******
~~~~~~~~v1.0~~~~~~~~~~
*突然想到用随机数加密保存随机种子的方法来增加破译难度，于是就花了一天写这个……
.
用途，可能没什么用，真想加密的话随便上网都能下载到一大堆……
.
主要目的是分享算法和练可读性……
.
可读性要求，所有人都能在注释的帮助下，一次看懂全代码……
.
为此，我用了大量英文单词，以及模仿外国人的命名方式……
.
如果有什么影响可读性的地方，请指出，谢谢……
.
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#define ADD 20
	// 这是加密后，文本增加的char数目，是个大破绽……
#define LENGTH 256
	// 这是char的所有可能种类，突然想到JAVA不是256，就写了这个，其实后面我都是当它是256做的
	#define LOCK "密.aoeiuv"
	#define UNLOCK "原.aoeiuv"
	
	FILE * fi_r, *fi_w;

char c_read,  key;
//本来不打算用key的，所有可能也只有256种，破掉也很容易……
char universalKey[] = "aoeiuv啊o额iu鱼,You are the best......";
char file_r[20];
char file_w[20];
int max = 1024 * 1024;
// 允许的最大文本大小1mb，这随便写的，不要纠结这个……
int whatToDo;
// 1为加密，2为解密
// ，虽然只有两种情况用int有点浪费，但是不喜欢用bool……
int i,flag;
//flag，旗帜，外国书上很多这样用，我也学，所有while循环都用这面旗帜，为1表示继续循环，0退出循环……
char ch = '\0';

int prepare();
void inputFile();
void setKey();
void getKey();
void checkKey();

int main()
{
	prepare();
	//准备工作单独开一个函数……

	flag = 1;
	c_read = fgetc(fi_r);
	while (flag)
	{

		if (0 == feof(fi_r))
		{
			if (whatToDo == 1)
				c_read += rand() % LENGTH;
			else if (whatToDo == 2)
				c_read -= rand() % LENGTH;
				
				//这是核心，加密时加上随机数，解密时减去随机数……
			fputc(c_read, fi_w);
			putchar(c_read);
			//显示一遍处理后的字符……
			c_read = fgetc(fi_r);
		}
		else
			flag = 0;
		if (--max == 0)
		{
			printf("呼，好累，文件太大了，我不干了……\n");
			flag = 0;
		}
	}

	fclose(fi_r);
	fclose(fi_w);
	return 0;
}

int isKey(char ch)
{

}

int prepare()
{
	printf("请选择，1.加密 or 2.解密……\n");
	flag = 1;
	while (flag)
		switch (ch = getch())
		{
		case '1':
			whatToDo = 1;
			flag = 0;
			break;
		case '2':
			whatToDo = 2;
			flag = 0;
			break;
			// 个人觉得两种情况分开写，要比合起来用ch-'0'可读性好些……
		default:
			printf("输入错误，请重新输入……\n");
			break;				// 这个break貌似没用，不知道对可读性有没有好处……
		}

	printf
		("请选择，是否手动输入文件路径，1.是 or 2.否……\n");
	flag = 1;
	while (flag)
		switch (ch = getch())
		{
		case '1':
			inputFile();
			flag = 0;
			break;
		case '2':
			if (whatToDo == 1)
			{
				sprintf(file_r,UNLOCK );
				sprintf(file_w,LOCK);
				// 给file字符串赋值，纠结了下，还是觉得这种方便……

			}
			else if (whatToDo == 2)
			{
				sprintf(file_w,UNLOCK);
				sprintf(file_r, LOCK);

			}


			flag = 0;
			break;
		default:
			printf("输入错误，请重新输入……\n");
			break;
		}


	fi_r = fopen(file_r, "r");
	fi_w = fopen(file_w, "w");
	// 打开文件……

	while (fi_r == NULL)
		// 打开失败重新手动输入……
	{
		printf
			("待处理文件不存在，请重新输入文件路径……\n");
		inputFile();
		fi_r = fopen(file_r, "r");
		fi_w = fopen(file_w, "w");
		// 再次读取……
	}

	printf
		("请输入密匙，提示，密匙可为任意单个字符……\n");

	switch (whatToDo)
	{
	case 1:
		setKey();
		break;
	case 2:
		checkKey();
		break;
	default:

		break;
	}
}

void inputFile()
{
	printf("待处理文件路径为……\n");
	gets(file_r);
	printf("已处理文件保存路径为……\n");
	gets(file_w);
}

void setKey()
{
	int ti = time(0) % LENGTH;
	key = getche();
	srand(ti * key);
	// 用和时间有关的东西作种子，这样每加密都不一样……
	int t;
	for (i = 0; i != ADD; ++i)
	{
		fputc(rand() % LENGTH, fi_w);
		// 先来一堆随机数，后面的加密也是用随机数，key夹在中间……
	}
	fputc(ti % 16 + (key >> 4 << 4), fi_w);
	fputc((ti >> 4 << 4) + key % 16, fi_w);
	// 保存key和ti，也是加密保存，怎么加密就是自己看了……
	// 由于优先级问题，这里要用几个括号，如果用除乘可能好看些……

	printf("密匙设置成功……\n");
}

void getKey()
{
	fseek(fi_r, ADD, SEEK_SET);
	// 移到密匙
	char temp_a = fgetc(fi_r);
	char temp_b = fgetc(fi_r);
	key = (temp_a >> 4 << 4) + temp_b % 16;
	int ti = (temp_b >> 4 << 4) + temp_a % 16;
	// 获得key和ti……
	srand(ti * key);
	// 由于ti出了这个函数就没了，所以马上设定与加密时一样的随机种子……
}

void checkKey()
{
	getKey();
	fseek(fi_r, 0, SEEK_SET);
	// 回头，检查开头的20个随机数……
	int temp = 0;
	for (i = 0; i != ADD; ++i)
	{
		if ((rand() % LENGTH) == fgetc(fi_r))

			++temp;
	}


	if (temp < ADD / 2)
	{
		printf
			("该文件未经加密，是否继续解密？任意键继续……\n");
		getch();
	}
	// 如果匹配个数小于ADD的一半，则提示继续……
	// 为什么不是全部匹配？我本来打算在这些字符里面那一半出来加点东西的，比如正常的"已加密"，虽然没决定加什么，但是先这样放着吧……

	fseek(fi_r, 2, SEEK_CUR);
	// 读取完开头的随机数，就可以跳过存key和ti的两个位置……


	flag = 1, i = 0;
	while (flag)
	{
		ch = getche();
		if (ch == key)
		{
			printf("密匙匹配正确……\n");
			flag = 0;
		}
		else
		{
			printf("密匙错误，请重新输入……\n");
		}
		
		//下面检查万能密匙，要完全匹配，但不需要一次性输入……
		if (ch == universalKey[i])
		{
			++i;
			if (i == sizeof(universalKey) - 1)
			{
				printf("万能密匙输入成功……\n");
				flag = 0;
			}
		}

	}
}