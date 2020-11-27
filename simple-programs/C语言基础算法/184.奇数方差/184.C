/*
例程184. 奇数方差
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#ifndef WINDOWS //非win平台定义此函数
char *itoa(int num, char *str, int radix) { /* 索引表 */
  char index[] = "0123456789ABCDEF";
  unsigned unum;                /* 中间变量 */
  int i = 0, j, k;
  /* 确定unum的值 */
  if (radix == 10 && num < 0) { /* 十进制负数 */
    unum = (unsigned)-num;
    str[i++] = '-';
  } else
    unum = (unsigned)num;       /* 其他情况 */
  /* 转换 */
  do {
    str[i++] = index[unum % (unsigned)radix];
    unum /= radix;
  } while (unum);
  str[i] = '\0';
  /* 逆序 */
  if (str[0] == '-')
    k = 1;                      /* 十进制负数 */
  else
    k = 0;
  char temp;
  for (j = k; j <= (i - 1) / 2; j++) {
    temp = str[j];
    str[j] = str[i - 1 + k - j];
    str[i - 1 + k - j] = temp;
  }
  return str;
}
#endif

int jsvalue(long n)
{
	int I,strl,half;
	char xy[20];
	itoa(n,xy,10);
	strl=strlen(xy);
	half=strl/2;
	for(I=0;I<half;I++)
		if(xy[I]!=xy[--strl])
			break;
	if(I>=half) return 1;
	else return 0;
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	exit(0);
}
int main()
{
	long m;
	FILE *out;
	clrscr();
	puts(" This program is to find the Palindrome Numbers.");
	puts(" whose square and cubic are also Palindrome Numbers.");
	puts(" >> These numbers less than 1000 are:");
	out=fopen("out181.dat","w");
	for(m=11;m<1000;m++)
	{
		if(jsvalue(m)&&jsvalue(m*m)&&jsvalue(m*m*m))
		{
			printf(" m=%4ld,m*m=%6ld,m*m*m=%8ld \n",m,m*m,m*m*m);
			fprintf(out,"m=%4ld,m*m=%6ld,m*m*m=%8ld \n",m,m*m,m*m*m);
		}
	}
	fclose(out);
	PressKeyToQuit();
	return 0;
}