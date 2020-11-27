#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXN 35
#define MAX_NAME 256

/* 学生信息 */
typedef struct student
{
  int  no;
  char name[MAX_NAME];
  double  Chinese;
  double  math;
  double  mark_ave;
  double  mark_tot;
} STUDENT;

int len = 0;

/* 菜单 */
void menu()
{
  printf("\n");
  printf("*****学生成绩管理系统****\n");   
  printf("    1 输入信息\n");
  printf("    2 输出信息\n");
  printf("    3 按平均分排序\n");
  printf("    4 按学号查找\n");
  printf("    5 插入信息\n");
  printf("    6 按学号删除\n");
  printf("    7 按平均分统计信息\n");
  printf("    8 保存到文件\n");
  printf("    9 读取默认信息\n");
  printf("    0 退出\n");
  printf("*************************\n");
  printf("%s\n", "请选择:");
}

/* 输入信息子程序 */
void input(STUDENT *data, int *len)
{
  int no;
  putchar('\n');
  printf("%s\n", "请输入，退出请输入-1.");
  printf("%s\n", "学号 姓名 语文 数学");
  printf("如: 1 jack 93 95\n");

  scanf("%d %s %lf %lf", &data[*len].no,data[*len].name, &data[*len].Chinese, &data[*len].math);    
  data[*len].mark_ave=(data[*len].Chinese+data[*len].math)/2.0;    
  data[*len].mark_tot=data[*len].Chinese+data[*len].math;
  (*len)++;
}


/* 输出信息子程序 */
void output(STUDENT *data, int len)
{
  int i;
  system("clear");
  printf("%4s", "学号");
  printf("%6s", "姓名");
  printf("%6s", "语文");
  printf("%6s", "数学");
  printf("%8s", "平均分");
  printf("%6s", "总分");
  putchar('\n');
  printf("*************************\n");
  for (i = 0; i< len; i++)
  {
    printf("%4d", data[i].no);
    printf("%6s", data[i].name);
    printf("%6.1lf", data[i].Chinese);
    printf("%6.1lf", data[i].math);
    printf("%8.1lf", data[i].mark_ave);
    printf("%6.1lf", data[i].mark_tot);
    putchar('\n');
  }
  printf("*************************\n");
  putchar('\n');
}

/* 排序子程序 */
void sort(STUDENT *data, int len)
{
  int i,j,k;
  STUDENT temp;
  for (i=0; i<len-1; i++) {
    for (k=i, j=i+1; j<len; j++)
      if (data[k].mark_ave > data[j].mark_ave) k=j;
    if (k != i) {

      temp = data[i];
      data[i] = data[k];
      data[k] = temp;
    }
  }

  output(data, len);
}

/* 搜索子程序 */
void find(STUDENT *data, int len)
{
  int find_no, result=0;
  int i;

  printf("%s\n", "请输入要查找学生的学号.");
  scanf("%d", &find_no);
  while( data[result].no != find_no && result < len) result ++;
  if (result >= len ) {
    printf("%s\n", "未查询到相关信息");
  }
  else {
    printf("%s\n", "查询信息如下：");
    printf("*************************\n");
    putchar('\n');
    printf("%4s", "学号");
    printf("%6s", "姓名");
    printf("%6s", "语文");
    printf("%6s", "数学");
    printf("%8s", "平均分");
    printf("%6s", "总分");
    putchar('\n');
    printf("%4d", data[result].no);
    printf("%6s", data[result].name);
    printf("%6.1lf", data[result].Chinese);
    printf("%6.1lf", data[result].math);
    printf("%8.1lf", data[result].mark_ave);
    printf("%6.1lf\n", data[result].mark_tot);

    printf("*************************\n");
    putchar('\n');
  }
}

/* 插入子程序 */
void insert(STUDENT *data, int *len)
{
  int no, pos;
  double Chinese, math,  mark_ave, mark_tot;
  int i;
  char name[MAX_NAME];
    printf("%s\n", "输入新记录.");
  printf("%s\n", "格式：学号 姓名 语文 数学");
  scanf("%d %s %lf %lf", &no, name, &Chinese, &math);
  mark_ave=(Chinese+math)/2.0;
  mark_tot=Chinese+math;
  pos = 0;
  while ((data[pos].mark_ave < mark_ave) && (pos < *len) )
    pos ++;
  for (i = *len-1; i >= pos; i--)
    data[i+1] = data[i];
  data[pos].no = no;
  strcpy(data[pos].name, name);
  data[pos].Chinese = Chinese;
  data[pos].math = math;
  data[pos].mark_ave = mark_ave;
  data[pos].mark_tot = mark_tot;
  (*len)++;
}

/* 删除子程序 */
void delete_item(STUDENT *data, int *len)
{
  int no, i, pos;
  lab: pos=0;
  printf("%s\n", "输入要删除的学生学号.");
  scanf("%d", &no);

  while( (data[pos].no != no) && (pos < *len) ) pos = pos +1;
  if (pos >= *len) {
    printf("%s\n", "未找到需要删除的学生");
  }
  else {
    for (i = pos+1;i < *len; i++)
      data[i-1] = data[i];
    *len = *len -1;
    if (*len == 0) {
      printf("%s\n", "已没有任何记录");
      return;
    }
  }
}

/* 统计子程序 */
void stat(STUDENT *data, int len)
{
  int no_59=0, no_69=0, no_79=0, no_89=0, no_100=0;
  int i;
  for (i = 0;i < len; i++) {
    if (data[i].mark_ave <=59) no_59++;
    else if (data[i].mark_ave <=69) no_69++;
    else if (data[i].mark_ave <=79) no_79++;
    else if (data[i].mark_ave <= 89) no_89++;
    else no_100++;
  }
  printf("*************************\n");
  printf("%10s", "分数");
  printf("%10s\n", "学生");

  printf("%10s", "0--59");
  printf("%10d\n", no_59);

  printf("%10s", "60--69");
  printf("%10d\n", no_69);

  printf("%10s", "70--79");
  printf("%10d\n", no_79);

  printf("%10s", "80--89");
  printf("%10d\n", no_89);

  printf("%10s", "90--100");
  printf("%10d\n", no_100);
  printf("*************************\n");
}

/* 将数据保存到文件 */
void save(STUDENT *data, int len)
{
  int i;
  FILE *fp;
  fp=fopen("/storage/emulated/0/cyuyan/profiles/成绩统计表.txt","w");
  fprintf(fp,"%8s", "学号");
  fprintf(fp,"%8s", "姓名");
  fprintf(fp,"%8s", "语文");
  fprintf(fp,"%8s", "数学");
  fprintf(fp,"%12s", "平均分");
  fprintf(fp,"%10s\n", "总分");
  fprintf(fp,"============================================================\n");
  putchar('\n');
  for (i = 0; i< len; i++)
  {
    fprintf(fp,"%8d", data[i].no);
    fprintf(fp,"%8s", data[i].name);
    fprintf(fp,"%8.1lf", data[i].Chinese);
    fprintf(fp,"%8.1lf", data[i].math);
    fprintf(fp,"%12.1lf", data[i].mark_ave);
    fprintf(fp,"%10.1lf\n", data[i].mark_tot);
  }
  fprintf(fp,"============================================================\n");
  fclose(fp);
}

/* 默认数据 */
void defdata(STUDENT *data)
{
  int i = 0;
  len = 10;

  for(int i = 0; i < len; i++)
  {
    data[i].no = i+1;
  }

  sscanf("Jack 65.0 68.0 66.5 133.0", "%s %lf %lf %lf %lf", 
    data[0].name, &data[0].Chinese, &data[0].math,&data[0].mark_ave,&data[0].mark_tot);
  sscanf("Bill 70.0 82.0 76.5 152.0", "%s %lf %lf %lf %lf", 
    data[1].name, &data[1].Chinese, &data[1].math,&data[1].mark_ave,&data[1].mark_tot);
  sscanf("Bob 70.0 72.0 71.0 142.0", "%s %lf %lf %lf %lf", 
    data[2].name, &data[2].Chinese, &data[2].math,&data[2].mark_ave,&data[2].mark_tot);
  sscanf("Devin 98.0 62.0 85.5 170.0", "%s %lf %lf %lf %lf", 
    data[3].name, &data[3].Chinese, &data[3].math,&data[3].mark_ave,&data[3].mark_tot);
  sscanf("Hale 84.0 69.0 76.5 153.0", "%s %lf %lf %lf %lf", 
    data[4].name, &data[4].Chinese, &data[4].math,&data[4].mark_ave,&data[4].mark_tot);
  sscanf("Henry 80.0 91.0 85.5 171.0", "%s %lf %lf %lf %lf", 
    data[5].name, &data[5].Chinese, &data[5].math,&data[5].mark_ave,&data[5].mark_tot);
  sscanf("Jim 86.0 82.0 84.0 168.0", "%s %lf %lf %lf %lf", 
    data[6].name, &data[6].Chinese, &data[6].math,&data[6].mark_ave,&data[6].mark_tot);
  sscanf("Ken 96.0 98.0 97.0 194.0", "%s %lf %lf %lf %lf", 
    data[7].name, &data[7].Chinese, &data[7].math,&data[7].mark_ave,&data[7].mark_tot);
  sscanf("Max 85.0 88.0 86.5 173.0", "%s %lf %lf %lf %lf", 
    data[8].name, &data[8].Chinese, &data[8].math,&data[8].mark_ave,&data[8].mark_tot);
  sscanf("Peter 90.0 91.0 90.5 181.0", "%s %lf %lf %lf %lf", 
    data[9].name, &data[9].Chinese, &data[9].math,&data[9].mark_ave,&data[9].mark_tot);
}

/* 主程序 */
int main()
{
  STUDENT data[MAXN];
  int choice;
  menu();
  scanf("%d", &choice);
  while (choice != 0) {

    switch(choice)
    {
    case 1:
      input(data, &len);
      printf("输入完毕，按任意键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 2:
      output(data, len);
      printf("输出完毕，按任意键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 3:
      sort(data, len);
      printf("排序完毕,按任意键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 4:
      find(data, len);
      printf("查找完毕,按任意键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 5:
      insert(data, &len);
      printf("插入完毕，按任意键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 6:
      delete_item(data, &len);
      printf("删除完毕，按任意键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 7:
      stat(data, len);
      printf("统计完毕，按回车键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 8:
      save(data, len);
      printf("保存完毕，按回车键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 9:
      defdata(data);
      output(data, len);
      printf("读取完毕，按回车键返回");
      __fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    default:
      break;
    }

    if(choice == 0)
    {
      return;
    }
    
    menu();
    scanf("%d",&choice);
  }
} 