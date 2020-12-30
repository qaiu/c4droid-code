#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp = NULL, *fpj = NULL, *fps = NULL;
int i, choice;
typedef struct book {
	char name[8];
	char num[8];
	int kucun;
	int jiechu;
} bo;
typedef struct student {
	char name[8];
	char num[8];
	int jie;
} st;
typedef struct jieyue {
	char snum[8];
	char bnum[8];
} ji;
/* 菜单 */
void menu()
{
	printf("\n");
	printf("**********主菜单*********\n");
	printf("    1-----图书录入\n");
	printf("    2-----图书浏览\n");
	printf("    3-----图书查询\n");
	printf("    4-----修改删除图书\n");
	printf("    5-----借阅图书\n");
	printf("    6-----归还图书\n");
	printf("    7-----借阅查询\n");
	printf("*************************\n");
}

/* 录入图书 */
void end()
{
	bo boo, booq;
	printf("请输入图书名(最多6个字符)：\n");
	scanf("%s", boo.name);
	do {
		i = 1;
		printf("请输入图书编号(最多6个字符)：\n");
		scanf("%s", boo.num);
		fread(&booq, sizeof(bo), 1, fp);
		while (!feof(fp)) {
			if (strcmp(booq.num, boo.num) == 0) {
				printf("\n该编号已存在请重新输入\n\n");
				i = 0;
				break;
			}
			fread(&booq, sizeof(bo), 1, fp);
		}
		rewind(fp);
	} while (i == 0);

	printf("请输入图书入库数：\n");
	scanf("%d", &boo.kucun);
	boo.jiechu = 0;
	getchar();
	fseek(fp, 0, 2);
	fwrite(&boo, sizeof(bo), 1, fp);

	printf("\n录入成功，按任意键返回主菜单");
	fpurge(stdin);
	getchar();
	system("clear");
}

/* 浏览图书 */
void print()
{
	bo boo;
	printf("  书名  编号  原库存  借出\n");
	fread(&boo, sizeof(bo), 1, fp);
	while (feof(fp) == 0) {
		printf("%6s%6s%8d%6d\n", boo.name, boo.num, boo.kucun, boo.jiechu);
		fread(&boo, sizeof(bo), 1, fp);
	}
	printf("\n浏览完毕，按任意键返回主菜单");
	fpurge(stdin);
	getchar();
	system("clear");
}

/* 对比两个字符串的相关度（用于精确搜索） */
int dui(char *p, char *p1)
{
	int bao = 0, fan = 1;
	for (i = 0; i < (int) strlen(p1); i++) {
		if (p[0] == p1[i]) {
			bao = i;
			break;
		}
	}

	if ((int) strlen(p) > (int) strlen(p1) - bao + 1)
		return 0;
	else
		for (i = 0; i < (int) strlen(p); i++) {
			if (p[i] != p1[bao + i])
				fan = 0;
		}
	return fan;
}

/* 查询图书 */
void find()
{
	char hao[10];
	int fan = 1;
	int bian = 1;
	bo boo;
	printf("请选择查询类型：\n    1-按图书编号模糊查询\n    2-按图书名关键字查询\n    3-按图书编号精确查询\n    4-按图书名精确查询\n  请选择：");
	scanf("%d", &choice);
	if (choice == 1) {
		printf("请输入编号(最多6个字符)：");
		scanf("%s", hao);
		printf("序号  书名  编号  原库存  借出\n");
		fread(&boo, sizeof(bo), 1, fp);
		while (!feof(fp)) {
			fan = dui(hao, boo.num);
			if (fan == 1) {
				printf("%4d%6s%6s%8d%6d\n", bian, boo.name, boo.num, boo.kucun, boo.jiechu);
				bian++;
			}
			fread(&boo, sizeof(bo), 1, fp);
		}
	} else if (choice == 2) {
		printf("请输入图书名关键字(最多6个字符)：");
		scanf("%s", hao);
		printf("序号  书名  编号  原库存  借出\n");
		fread(&boo, sizeof(bo), 1, fp);
		while (!feof(fp)) {
			fan = dui(hao, boo.name);
			if (fan == 1)
				printf("%4d%6s%6s%8d%6d\n", bian, boo.name, boo.num, boo.kucun, boo.jiechu);
			fread(&boo, sizeof(bo), 1, fp);
		}
	} else if (choice == 4) {
		printf("请输入图书名(最多6个字符)：");
		scanf("%s", hao);
		printf("序号  书名  编号  原库存  借出\n");
		while (!feof(fp)) {
			if (strcmp(hao, boo.name) == 0)
				printf("%4d%6s%6s%8d%6d\n", bian, boo.name, boo.num, boo.kucun, boo.jiechu);
			fread(&boo, sizeof(bo), 1, fp);
		}

	} else if (choice == 3) {
		printf("请输入图书编号(最多6个字符)：");
		scanf("%s", hao);
		printf("序号  书名  编号  原库存  借出\n");
		while (!feof(fp)) {
			if (strcmp(hao, boo.num) == 0)
				printf("%4d%6s%6s%8d%6d\n", bian, boo.name, boo.num, boo.kucun, boo.jiechu);
			fread(&boo, sizeof(bo), 1, fp);
		}

	} else {
		printf("输入错误，正在退出！");

	}

	printf("\n查询完毕，按任意键返回主菜单");
	fpurge(stdin);
	getchar();
	system("clear");
}

/* 修改删除图书 */
void del()
{
	FILE *p;
	ji jie;
	int j = 0;
	char num[10];
	bo boo, booq;
	int xu;
	char aor;
	char cha[10];
	printf("\n删除修改图书只能通过图书编号来完成！\n是否调用图书查询来查询你要操作的图书编号（Y/N）\n");
	getchar();
	scanf("%c", &aor);
	if (aor == 'Y' || aor == 'y') {
		find();
		rewind(fp);
	}
	printf("\n请输入你要操作的图书的编号（如果输入错误的编号，将不做任何操作）：");
	scanf("%s", cha);
	fread(&jie, sizeof(ji), 1, fpj);
	while (!feof(fpj)) {
		if (strcmp(jie.bnum, cha) == 0) {
			printf("\n这本书已有人借阅，不能操作！\n正在退出 ");
			return;
		}
		fread(&jie, sizeof(ji), 1, fpj);
	}
	printf("\n\n1-删除图书   2-修改图书\n\n");
	printf("请选择操作类型：");
	scanf("%d", &choice);
	fread(&boo, sizeof(bo), 1, fp);
	p = fopen("books1.txt", "wb+");
	while (!feof(fp)) {
		j++;
		if (strcmp(boo.num, cha) == 0) {
			if (choice == 2) {
				printf("\n\n1-书名    2-编号    3-库存\n\n");
				printf("请选择要修改的项目:");
				scanf("%d", &xu);
				if (xu == 1) {
					printf("你要把%s改为：", boo.name);
					scanf("%s", boo.name);
				} else if (xu == 2) {
					do {
						i = 1;
						printf("你要把%s改为：", boo.num);
						scanf("%s", num);
						rewind(fp);
						fread(&booq, sizeof(bo), 1, fp);
						while (!feof(fp)) {
							if (strcmp(booq.num, num) == 0) {
								printf("\n该编号已存在请重新输入\n\n");
								i = 0;
								break;
							}
							fread(&booq, sizeof(bo), 1, fp);
						}
					} while (i == 0);
					rewind(fp);
					for (i = 0; i < j; i++) {
						fread(&boo, sizeof(bo), 1, fp);
						strcpy(boo.num, num);
					}
				} else if (xu == 3) {
					printf("你要把%d改为：", &boo.kucun);
					scanf("%d", &boo.kucun);
				}
				fwrite(&boo, sizeof(bo), 1, p);
			}
		} else
			fwrite(&boo, sizeof(bo), 1, p);
		fread(&boo, sizeof(bo), 1, fp);
	}
	fclose(p);
	p = fopen("books1.txt", "rb+");
	fclose(fp);
	fp = fopen("book.txt", "wb+");
	fread(&boo, sizeof(bo), 1, p);
	while (!feof(p)) {
		fwrite(&boo, sizeof(bo), 1, fp);
		fread(&boo, sizeof(bo), 1, p);
	}
	fclose(fp);

	printf("\n操作成功，按任意键返回主菜单");
	fpurge(stdin);
	getchar();
	system("clear");
}

/* 借阅图书 */
void lend()
{
	bo boo;
	FILE *p;
	st stu;
	ji jie;
	int j = 0;
	char aor, num[10], xnum[10];
	printf("\n借阅图书只能通过图书编号完成！\n是否调用图书查询来查询你要操作的图书编号（Y/N）\n");
	getchar();
	scanf("%c", &aor);
	if (aor == 'Y' || aor == 'y')
		find();
	printf("\n请输入你要借阅的图书的编号：");
	scanf("%s", num);
	fread(&boo, sizeof(bo), 1, fp);
	while (!feof(fp)) {
		if (strcmp(num, boo.num) == 0) {
			j = 1;
			break;
		}
		fread(&boo, sizeof(bo), 1, fp);
	}
	if (j == 0) {
		printf("\n书库没有该图书 正在退出\n");
		return;
	}
	printf("\n你需要借阅的图书是：\n");
	printf("  书名  编号  原库存  借出\n");
	printf("%6s%6s%8d%6d\n", boo.name, boo.num, boo.kucun, boo.jiechu);
	if (boo.kucun == boo.jiechu) {
		printf("\n该册图书已全部借出！正在退出\n");
		return;
	}
	printf("\n请输入你的学号：");
	scanf("%s", xnum);
	j = 0;
	fread(&jie, sizeof(ji), 1, fpj);
	while (!feof(fpj)) {
		if ((strcmp(jie.bnum, num) == 0) && (strcmp(jie.snum, xnum) == 0)) {
			j = 1;
			break;
		}
		fread(&jie, sizeof(ji), 1, fpj);
	}
	if (j == 1) {
		printf("\n你已经借阅过该书  正在退出\n");
		return;
	}
	fread(&stu, sizeof(st), 1, fps);
	j = 0;
	while (!feof(fps)) {
		if (strcmp(xnum, stu.num) == 0) {
			j = 1;
			break;
		}
		fread(&stu, sizeof(st), 1, fps);
	}
	if (j == 0) {
		printf("\n你是第一次借阅图书，为了方便管理请输入你的姓名：");
		scanf("%s", stu.name);
		strcpy(stu.num, xnum);
		stu.jie = 1;
		fseek(fps, 0, 2);
		fwrite(&stu, sizeof(st), 1, fps);
	} else {
		if (stu.jie == 5) {	// 这里可以控制每个学生最多借几本书
			printf("\n你已经借了5本书，不能再借\n");
			return;
		}
		p = fopen("jie.txt", "wb+");
		rewind(fps);
		fread(&stu, sizeof(st), 1, fps);
		while (!feof(fps)) {
			if (strcmp(xnum, stu.num) == 0)
				stu.jie++;
			fwrite(&stu, sizeof(st), 1, p);
			fread(&stu, sizeof(st), 1, fps);
		}
		fclose(p);
		fclose(fps);
		p = fopen("jie.txt", "rb+");
		fps = fopen("student.txt", "wb+");
		fread(&stu, sizeof(st), 1, p);
		while (!feof(p)) {
			fwrite(&stu, sizeof(st), 1, fps);
			fread(&stu, sizeof(st), 1, p);
		}
	}
	strcpy(jie.bnum, num);
	strcpy(jie.snum, xnum);
	fseek(fpj, 0, 2);
	fwrite(&jie, sizeof(ji), 1, fpj);
	rewind(fp);
	p = fopen("jie.txt", "wb+");
	fread(&boo, sizeof(bo), 1, fp);
	while (!feof(fp)) {
		if (strcmp(num, boo.num) == 0)
			boo.jiechu++;
		fwrite(&boo, sizeof(bo), 1, p);
		fread(&boo, sizeof(bo), 1, fp);
	}
	fclose(p);
	p = fopen("jie.txt", "rb+");
	fclose(fp);
	fp = fopen("book.txt", "wb+");
	fread(&boo, sizeof(bo), 1, p);
	while (!feof(p)) {
		fwrite(&boo, sizeof(bo), 1, fp);
		fread(&boo, sizeof(bo), 1, p);
	}

	printf("\n借阅成功，按任意键返回主菜单");
	fpurge(stdin);
	getchar();
	system("clear");
}

/* 归还图书 */
void huan()
{
	FILE *p;
	bo boo;
	ji jie;
	st stu;
	int j = 0;
	char snum[10], bnum[10];
	i = 0;
	printf("\n请输入你的学号：");
	scanf("%s", snum);
	printf("\n你借阅了以下图书\n");
	fread(&jie, sizeof(ji), 1, fpj);
	printf("  书名  编号  原库存  借出\n");
	while (!feof(fpj)) {
		if (strcmp(jie.snum, snum) == 0) {
			fread(&boo, sizeof(bo), 1, fp);
			while (!feof(fp)) {
				if (strcmp(jie.bnum, boo.num) == 0) {
					printf("%6s%6s%8d%6d\n", boo.name, boo.num, boo.kucun, boo.jiechu);
					j = 1;
				}
				fread(&boo, sizeof(bo), 1, fp);
			}
		}
		fread(&jie, sizeof(ji), 1, fpj);
	}
	if (j == 0) {
		printf("\n你没有借阅任何图书！\n\n正在退出");
		return;
	}
	rewind(fp);
	rewind(fpj);
	printf("\n请输入你归还的图书的编号：");
	scanf("%s", bnum);
	fread(&jie, sizeof(jie), 1, fpj);	// 改变jieyue.txt里的数据
	p = fopen("jie.txt", "wb+");
	while (!feof(fpj)) {
		if ((strcmp(jie.bnum, bnum) == 0) && (strcmp(jie.snum, snum) == 0))
			i = 1;
		else
			fwrite(&jie, sizeof(jie), 1, p);
		fread(&jie, sizeof(jie), 1, fpj);
	}
	if (i == 0) {
		printf("\n你没有借过该图书\n正在退出");
		return;
	}
	fclose(p);
	fclose(fpj);
	p = fopen("jie.txt", "rb+");
	fpj = fopen("jieyue.txt", "wb+");
	fread(&jie, sizeof(ji), 1, p);
	while (!feof(p)) {
		fwrite(&jie, sizeof(ji), 1, fpj);
		fread(&jie, sizeof(ji), 1, p);
	}
	fclose(p);		// 改变book.txt里的数据
	p = fopen("jie.txt", "wb+");
	fread(&boo, sizeof(bo), 1, fp);
	while (!feof(fp)) {
		if (strcmp(bnum, boo.num) == 0)
			boo.jiechu--;
		fwrite(&boo, sizeof(bo), 1, p);
		fread(&boo, sizeof(bo), 1, fp);
	}
	fclose(p);
	p = fopen("jie.txt", "rb+");
	fclose(fp);
	fp = fopen("book.txt", "wb+");
	fread(&boo, sizeof(bo), 1, p);
	while (!feof(p)) {
		fwrite(&boo, sizeof(bo), 1, fp);
		fread(&boo, sizeof(bo), 1, p);
	}
	fclose(p);		// 改变student.txt里的数据
	p = fopen("jie.txt", "wb+");
	rewind(fps);
	fread(&stu, sizeof(st), 1, fps);
	while (!feof(fps)) {
		if (strcmp(snum, stu.num) == 0)
			stu.jie--;
		fwrite(&stu, sizeof(st), 1, p);
		fread(&stu, sizeof(st), 1, fps);
	}
	fclose(p);
	fclose(fps);
	p = fopen("jie.txt", "rb+");
	fps = fopen("student.txt", "wb+");
	fread(&stu, sizeof(st), 1, p);
	while (!feof(p)) {
		fwrite(&stu, sizeof(st), 1, fps);
		fread(&stu, sizeof(st), 1, p);
	}

	printf("\n归还成功，按任意键返回主菜单");
	fpurge(stdin);
	getchar();
	system("clear");
}

/* 借阅查询 */
void cha()
{
	char num[10];
	bo boo;
	ji jie;
	printf("\n请输入你的学号：");
	scanf("%s", num);
	fread(&jie, sizeof(ji), 1, fpj);
	printf("  书名  编号  原库存  借出\n");

	while (!feof(fpj)) {
		if (strcmp(jie.snum, num) == 0) {
			fread(&boo, sizeof(bo), 1, fp);
			while (!feof(fp)) {
				if (strcmp(jie.bnum, boo.num) == 0)
					printf("%6s%6s%8d%6d\n", boo.name, boo.num, boo.kucun, boo.jiechu);
				fread(&boo, sizeof(bo), 1, fp);
			}
		}
		fread(&jie, sizeof(ji), 1, fpj);
	}


	printf("\n查询完毕，按任意键返回主菜单");
	fpurge(stdin);
	getchar();
	system("clear");
}

int main()
{
	menu();
	printf("  请选择:");
	scanf("%d", &choice);

	while (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7) {
		fp = fopen("book.txt", "rb+");
		if (fp == NULL) {
			printf("book.txt open error\n");
		}
		fps = fopen("student.txt", "rb+");
		if (fps == NULL) {
			printf("student.txt open error\n");

		}
		fpj = fopen("jieyue.txt", "rb+");
		if (fpj == NULL) {
			printf("jieyue.txt open error\n");
			return 0;
		}
		switch (choice) {
		case 0:
			break;
		case 1:
			end();
			break;
		case 2:
			print();
			break;
		case 3:
			find();
			break;
		case 4:
			del();
			break;
		case 5:
			lend();
			break;
		case 6:
			huan();
			break;
		case 7:
			cha();
			break;
		}
		fclose(fp);
		fclose(fpj);
		fclose(fps);
		menu();
		printf("\n  请选择:");
		scanf("%d", &choice);
	}
}