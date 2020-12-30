/*c4droid代码手册
 *学生成绩管理系统
 *mrp本拉登编写
 *使用GCC编译器
 *TTHHR收集整理
 *转载请说明出处
*/
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
typedef struct
{
	char num[10];
	char name[10];
	char sex[2];
	int score[4];
	int sum;
	int ave;
} student;

// ********************************************************************************************//
// 1.数据输入//
input()
{
	student man[60];
	int i, j;
	int num;
	printf("现输入的数据将覆盖以前的所有记录\n\n");
	printf("请输入人数:\n");
	scanf("%d", &num);
	for (i = 0; i < num; i++)
	{
		printf("请输入学号:\n");
		scanf("%s", man[i].num);
		printf("请输入姓名:\n");
		scanf("%s", man[i].name);
		printf("请输入性别:\n");
		scanf("%s", man[i].sex);

		for (j = 0; j < 4; j++)
		{
			printf("请输入第%d个成绩:\n", j + 1);
			scanf("%d", &man[i].score[j]);
		}
	}
	save(man, num);
}

int save(student man[], int n)
{
	FILE *fp;
	int i;
	if ((fp = fopen("/sdcard/man_dat", "wb")) == NULL)
	{
		printf("cannot open file\n");
		return;
	}
	for (i = 0; i < n; i++)
		if (fwrite(&man[i], sizeof(student), 1, fp) != 1)
			printf("file write error\n");
	fclose(fp);
}


// 2.数据输出//
int display()
{
	student man[60];
	FILE *fp;
	int i, n, j;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);
	system("busybox clear");
	 printf("\t\t\t     数据输出\n");
	printf
		("=======================================================================\n");
	printf
		("学号\t姓名\t性别\t英语\t数学\t计算机\tC语言\t总分\t平均分\n");
	printf
		("=======================================================================\n");
	for (i = 0; i < n; i++)
	{
		man[i].sum = 0;
		printf("%s\t%s\t%s", man[i].num, man[i].name, man[i].sex);
		for (j = 0; j < 4; j++)
		{
			printf("\t%d", man[i].score[j]);
			man[i].sum += man[i].score[j];
		}
		man[i].ave = man[i].sum / 4;
		printf("\t%d\t%d", man[i].sum, man[i].ave);
		printf("\n");
		printf
			("-----------------------------------------------------------------------\n");
	}
	getche();
}

// 3.信息查询//

find()
{
	student man[60], t;
	FILE *fp;
	int i, n;
	int h = 1;
	char ch;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);
	while (h == 1)
	{
		system("busybox clear");
		printf("\n          信息查询\n");
		printf("==============================\n");
		printf("         1 .学号查询\n");
		printf("         2 .姓名查询\n");
		printf("         0 .退出查询\n");
		printf("==============================\n");
		ch = getchar();
		switch (ch)
		{
		case '1':
			num1();
			break;
		case '2':
			name1();
			break;
		case '0':
			h = 0;
			break;
		default:
			printf("error\n");
		}

	}
}

// 按学号查询//
num1()
{
	student man[60], t;
	char num[10];
	FILE *fp;
	int i, n, j;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	system("busybox clear");
	printf("请输入学号\n");
	scanf("%s", num);
	printf("\t\t\t    学号查询\n");
	printf
		("=======================================================================\n");
	printf
		("学号\t姓名\t性别\t英语\t数学\t计算机\tC语言\t总分\t平均分\n");
	printf
		("=======================================================================\n");
	for (i = 0; i < n; i++)
		if (strcmp(man[i].num, num) == 0)
		{
			man[i].sum = 0;
			printf("%s\t%s\t%s", man[i].num, man[i].name, man[i].sex);
			for (j = 0; j < 4; j++)
			{
				printf("\t%d", man[i].score[j]);
				man[i].sum += man[i].score[j];
			}
			man[i].ave = man[i].sum / 4;
			printf("\t%d\t%d", man[i].sum, man[i].ave);
			printf("\n");
			printf
				("-----------------------------------------------------------------------\n");
			break;
		}
	if (i == n)
		printf("系统没有写入该学号,请重新输入！");

	getche();
}

// 按姓名查询//
name1()
{
	student man[60], t;
	char name[10];
	FILE *fp;
	int i, n, j;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);
	system("busybox clear");
	printf("请输入姓名:");
	scanf("%s", name);
	printf("\t\t\t    姓名查询\n");
	printf
		("=======================================================================\n");
	printf
		("学号\t姓名\t性别\t英语\t数学\t计算机\tC语言\t总分\t平均分\n");
	printf
		("=======================================================================\n");
	for (i = 0; i < n; i++)
		if (strcmp(man[i].name, name) == 0)
		{
			man[i].sum = 0;
			printf("%s\t%s\t%s", man[i].num, man[i].name, man[i].sex);
			for (j = 0; j < 4; j++)
			{
				printf("\t%d", man[i].score[j]);
				man[i].sum += man[i].score[j];
			}
			man[i].ave = man[i].sum / 4;
			printf("\t%d\t%d", man[i].sum, man[i].ave);
			printf("\n");
			printf
				("-----------------------------------------------------------------------\n");
			break;
		}
	if (i == n)
		printf("系统没有收录该姓名，请重新输入");
	getche();
}

// ********************************************************************************************//
// 4.数据插入//
// 直接插入法//
insert()
{
	student man[60], t;
	FILE *fp;
	char num2[10];
	int i, x, j, n;

	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);
	system("busybox clear");
	for (i = 0; i < n; i++)
		for (j = 0; j < n - j; j++)
			if (strcmp(man[j].num, man[j + 1].num) > 0)
			{
				t = man[j];
				man[j] = man[j + 1];
				man[j + 1] = t;
			}
	printf("\n\t\t\t数据插入\n");
	printf("==========================================================\n");
	printf("\n\t\t  请输入要插入的学号:");
	scanf("%s", num2);
	for (i = 0; i < n; i++)
		if (strcmp(man[i].num, num2) == 0)
		{
			printf
				("\n\n=========该学号已经存在请重新输入其他未使用的学号=========\n");
			getche();
			return;
		}
		else if (strcmp(man[i].num, num2) > 0)
			break;
	for (x = n - 1; x >= i; x--)
		man[x + 1] = man[x];
	printf("请输入学号:\n");
	scanf("%s", man[i].num);
	printf("请输入姓名:\n");
	scanf("%s", man[i].name);
	printf("请输入性别:\n");
	scanf("%s", man[i].sex);
	for (j = 0; j < 4; j++)
	{
		printf("请输入第%d个成绩:\n", j + 1);
		scanf("%d", &man[i].score[j]);
	}
	save(man, n + 1);
	printf
		("\n======================插入学号 %s 成功======================",
		 man[i].num);
	getche();



}


// ********************************************************************************************//
// 5.数据汇总//
sum()
{
	student man[60];
	FILE *fp;
	int i, n, j;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);
	system("busybox clear");
	printf("\t\t\t数据汇总\n");
	printf
		("=======================================================================\n");
	printf
		("学号\t姓名\t性别\t英语\t数学\t计算机\tC语言\t总分\t平均分\n");
	printf
		("=======================================================================\n");
	for (i = 0; i < n; i++)

	{
		man[i].sum = 0;
		printf("%s\t%s\t%s", man[i].num, man[i].name, man[i].sex);
		for (j = 0; j < 4; j++)
		{
			printf("\t%d", man[i].score[j]);
			man[i].sum += man[i].score[j];
		}
		man[i].ave = man[i].sum / 4;
		printf("\t%d\t%d", man[i].sum, man[i].ave);
		printf("\n");
		printf
			("-----------------------------------------------------------------------\n");
	}
	getche();

}

// ********************************************************************************************//
// 6.数据统计//
count()
{
	student man[60];
	FILE *fp;
	int i, n, j;
	int a, b, c, d, e;
	float A, D, E;
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);
	system("busybox clear");
	printf("\t\t\t\t数据统计\n\n");
	printf
		("===============================================================================\n");
	printf
		("学号\t姓名\t性别\t英语\t数学\t计算机\tC语言\t总分\t平均分\t等级\n");
	printf
		("===============================================================================\n");

	for (i = 0; i < n; i++)
	{
		man[i].sum = 0;
		printf("%s\t%s\t%s", man[i].num, man[i].name, man[i].sex);
		for (j = 0; j < 4; j++)
		{
			printf("\t%d", man[i].score[j]);
			man[i].sum += man[i].score[j];
		}
		man[i].ave = man[i].sum / 4;
		printf("\t%d\t%d", man[i].sum, man[i].ave);
		switch (man[i].ave / 10)
		{
		case 10:
		case 9:
			printf("\t优秀");
			a++;
			break;
		case 8:
			printf("\t良好");
			b++;
			break;
		case 7:
			printf("\t中等");
			c++;
			break;
		case 6:
			printf("\t及格");
			d++;
			break;
		default:
			printf("\t不及格");
			e++;
			break;
		}
		printf("\n");
		printf
			("-------------------------------------------------------------------------------\n");
	}
	A = a * 1.0 / n;
	D = (a + b + c + d) * 1.0 / n;
	E = e * 1.0 / n;
	printf("\n优秀的人数有:%d \t\t\t优秀率%.2f%%\n", a, A * 100);
	printf("良好的人数有:%d\n", b);
	printf("中等的人数有:%d \t\t\t及格率%.2f%%\n", c, D * 100);
	printf("及格的人数有:%d\n", d);
	printf("不及格的人数有:%d \t\t不及格率%.2f%%\n", e, E * 100);
	getche();

}

// ********************************************************************************************//
// 7.数据删除//
delete()
{
	student man[60];
	FILE *fp;
	char number[10];
	int i, n, j;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);

	printf("请输入要删除的学号:");
	scanf("%s", number);		/* 输入要删除的学号 */
	for (i = 0; i < n; i++)		/* 结构体里的个数0-n循环 */
	{
		if (strcmp(man[i].num, number) == 0)	/* 结构体i里的学号和输入的学号进行比较如果相等进入IF语句 */
		{
			for (j = i; j < n - 1; j++)
				man[j] = man[j + 1];	/* 进入for循环将被删除数之后的数向前移一位，覆盖结构体数组man[j] */
			save(man, n - 1);	/* 将实参结构体数组man和n-1，传递到保存函数save中进行文件保存 */
			printf("\n删除成功");
			getche();
			break;				/* 删除成功后跳出循环 */
		}
	}
	if (i == n)					/* 如果i等价于n则说明无该学号 */
	{
		printf("\n系统没有写入该学号,请重新输入！");	/* 输出没有该学号的提示 */
		getche();
	}

}

// ********************************************************************************************//
// 8.数据修改//

update()
{

	student man[60];
	FILE *fp;
	char number[10];
	int i, n, j;
	if ((fp = fopen("/sdcard/man_dat", "rb")) == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	for (i = 0; (!feof(fp)); i++)
		fread(&man[i], sizeof(student), 1, fp);
	n = i - 1;
	fclose(fp);
	printf("请输入要修改的学号:");
	scanf("%s", number);		/* 输入要修改的学号 */
	for (i = 0; i < n; i++)		/* 结构体里的个数0-n循环 */
	{
		if ((strcmp(man[i].num, number)) == 0)	/* 结构体i里的学号和输入的学号进行比较如果相等进入IF语句 */
		{
			printf("请输入姓名:\n");
			scanf("%s", man[i].name);	/* 输入姓名 */
			printf("请输入性别:\n");
			scanf("%s", man[i].sex);	/* 输入性别 */
			for (j = 0; j < 4; j++)	/* 进入四个成绩输入的for循环 */
			{
				printf("请输入第%d个成绩:\n", j + 1);
				scanf("%d", &man[i].score[j]);	/* 输入成绩 */
			}
			save(man, n);		/* 将实参结构体数组man和，传递到保存函数save中进行文件保存 */
			printf("修改学号 %s 成功", man[i].num);
			break;				/* 修改完后跳出循环 */
		}
	}

	if (i == n)					/* 如果i等价n进入if语句 */
		printf("\n系统没有写入该学号,请重新输入！");	/* 输出
																	   系统没有写入该学号,请重新输入！ */
	getche();
}

// ********************************************************************************************//
// 主函数//
main()
{
	int n = 1;
	char ch;
	printf("\n");
	while (n == 1)
	{
		system("busybox clear");
		printf("\n");
		printf("          学生成绩管理系统\n");
		printf("================================\n");
		printf("            1.数据输入\n");
		printf("            2.数据输出\n");
		printf("            3.信息查询\n");
		printf("            4.数据插入\n");
		printf("            5.数据汇总\n");
		printf("            6.数据统计\n");
		printf("            7.数据删除\n");
		printf("            8.数据修改\n");
		printf("            0.退出系统\n");
		printf("================================\n");
		ch = getchar();
		switch (ch)
		{
		case '1':
			input();
			break;
		case '2':
			display();
			break;
		case '3':
			find();
			break;
		case '4':
			insert();
			break;
		case '5':
			sum();
			break;
		case '6':
			count();
			break;
		case '7':
			delete();
			break;
		case '8':
			update();
			break;
		case '0':
			n = 0;
			break;
		default:
			printf("error\n");
		}
	}
	system("su -c input keyevent 4");
}

// ********************************************************************************************//