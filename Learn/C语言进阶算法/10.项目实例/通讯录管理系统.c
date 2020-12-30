#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 

typedef struct 
{ 
  char score;     /*编号*/
  char name[10];  /*姓名*/ 
  char num[15];   /*号码*/ 
  char age[8];    /*年龄*/
}Person; 

Person pe[80];

int menu_select() 
{ 
  char c; 
  do{ 
    system("clear");
    printf("    *****通讯录***** \n");
    printf("    ┌────────┐\n");
    printf("    │ 1. 添加记录  │\n");
    printf("    │ 2. 显示记录  │\n");
    printf("    │ 3. 删除记录  │\n");
    printf("    │ 4. 查询记录  │\n");
    printf("    │ 5. 修改记录  │\n");
    printf("    │ 6. 保存记录  │\n");
    printf("    │ 7. 默认记录  │\n");
    printf("    │ 0. 退出程序  │\n");
    printf("    └────────┘\n");
    printf("    请您选择(0-7):");
    c=getchar();
  }while(c<'0'||c>'7');
  return(c-'0');
} 

/* 添加记录 */
int Input(Person per[],int n) 
{
  int i=0; 
  char sign = 'Y',x[10];
  while(sign!='n'&&sign!='N')
  {
    printf("编号:");
    scanf("%d",&per[n+i].score);
    printf("姓名:");
    scanf("%s",per[n+i].name);
    printf("年龄:");
    scanf("%s",per[n+i].age);
    printf("电话号码:");
    scanf("%s",per[n+i].num);

    scanf("%s",&x);
    printf("\n是否继续添加?(Y/N)");
    scanf("%c",&sign);
    i++;
  }
  return(n+i);
}

/* 显示记录 */
void Display(Person per[],int n)
{
  int i;
  printf("--------------------------\n");
  printf("编号 姓名 年龄        电话\n"); 
  printf("--------------------------\n"); 
  for(i=1;i<n+1;i++)
  { 
    printf("%4d%5s%5s%12s\n",per[i-1].score,per[i-1].name,per[i-1].age,per[i-1].num); 
  } 
  printf("--------------------------\n");
} 

/* 删除记录 */
int Delete_a_record(Person per[],int n) 
{ 
  char s[20]; 
  int i=0,j; 
  printf("请输入想删除记录中的名字:"); 
  scanf("%s",s); 
  while(strcmp(per[i].name,s)!=0&&i<n) i++; 
  if(i==n) 
  {
    printf("通讯录中没有此人!\n"); 
    return(n); 
  } 
  for(j=i;j<n-1;j++)
  { 
    strcpy(per[j].num,per[j+1].num); 
    strcpy(per[j].name,per[j+1].name); 
    strcpy(per[j].age,per[j+1].age);
    per[j].score=per[j+1].score; 
  } 
  printf("已经成功删除!\n"); 
  return(n-1); 
} 

/* 查找记录 */
void Query_a_record(Person per[],int n) 
{ 

  int m;
  printf("    \n请选择查询方式:\n");
  printf("    ┌──────┐\n");
  printf("    │1----- 姓名│\n");
  printf("    │2----- 电话│\n");
  printf("    │3----- 返回│\n");
  printf("    └──────┘\n");
  printf("    请选择:");
  scanf("%d",&m);
  while(m!=1&&m!=2&&m!=3)
  { 
    printf("输入错误，请重新选择:");
    scanf("%d",&m);
  }

  if(m==1)
  {
    char s[20]; 
    int i=0; 
    printf("请输入想查询的姓名:"); 
    scanf("%s",s); 
    while(strcmp(per[i].name,s)!=0&&i<n) i++; 
    if(i==n) 
    {
      printf("通讯录中没有此人!\n"); 
      return; 
    } 
    printf("此人编号: %d\n",per[i].score);
    printf("此人年龄: %s\n",per[i].age); 
    printf("电话号码: %s\n",per[i].num); 
  }

  if(m==2)
  {
    char s[20]; 
    int i=0; 
    printf("请输入想查询的电话:"); 
    scanf("%s",s); 
    while(strcmp(per[i].num,s)!=0&&i<n) i++; 
    if(i==n) 
    {
      printf("通讯录中没有此人!\n"); 
      return; 

    } 
    printf("此人编号: %d\n",per[i].score);
    printf("此人姓名: %s\n",per[i].name);
    printf("此人年龄: %s\n",per[i].age); 
  }
}

/* 修改数据 */
void Change(Person per[],int n) 
{
  char s[20]; 
  int i=0; 
  printf("请输入想修改的记录中的名字:"); 
  scanf("%s",s); 
  while(strcmp(per[i].name,s)!=0&&i<n) i++; 
  if(i==n) 
  { 
    printf("通讯录中没有此人!\n"); 
    return; 
  } 
  printf("编号:"); 
  scanf("%d",&per[i].score); 
  printf("姓名:"); 
  scanf("%s",per[i].name); 
  printf("年龄:"); 
  scanf("%s",per[i].age);
  printf("电话号码:"); 
  scanf("%s",per[i].num); 
  printf("修改成功！");
}

/* 保存数据 */
void WritetoText(Person per[],int n)
{ 
  int i=0; 
  FILE *fp;   /*定义文件指针*/ 
  if((fp=fopen("/storage/emulated/0/cyuyan/profiles/通讯录.txt","w"))==NULL)
  { 
    printf("无法打开文件\n"); 
    system("pause"); 
    return; 
  } 
  fprintf(fp,"***************通讯录***************\n");
  fprintf(fp,"编号     姓名   年龄          电话\n");
  fprintf(fp,"------------------------------------\n");
  while(i<n) 
  { 
    fprintf(fp,"%4d%9s%7s%12s\n",per[i].score,per[i].name,per[i].age,per[i].num); 
    i++; 
  }
  fprintf(fp,"------------------------------------\n");
  fprintf(fp,"************共有%d条记录************\n",n); 
  fclose(fp);  /*关闭文件*/ 
  printf("保存成功!\n"); 
} 

/* 默认数据 */
void DefaultData(Person per[],int *n)
{ 
  int i = 0;
  *n = 10;

  for(int i = 0; i < *n; i++)
  {
    per[i].score = i+1;
  }

  sscanf("Jack 13700000001 20", "%s %s %s", per[0].name, per[0].num, per[0].age);
  sscanf("Bill 13700000002 25", "%s %s %s", per[1].name, per[1].num, per[1].age);
  sscanf("Bob 13700000003 26", "%s %s %s", per[2].name, per[2].num, per[2].age);
  sscanf("Devin 13700000004 28", "%s %s %s", per[3].name, per[3].num, per[3].age);
  sscanf("Hale 13700000005 21", "%s %s %s", per[4].name, per[4].num, per[4].age);
  sscanf("Henry 13700000006 23", "%s %s %s", per[5].name, per[5].num, per[5].age);
  sscanf("Jim 13700000007 25", "%s %s %s", per[6].name, per[6].num, per[6].age);
  sscanf("Ken 13700000008 22", "%s %s %s", per[7].name, per[7].num, per[7].age);
  sscanf("Max 13700000009 24", "%s %s %s", per[8].name, per[8].num, per[8].age);
  sscanf("Peter 13700000010 21", "%s %s %s", per[9].name, per[9].num, per[9].age);
}

/*主函数*/ 
int main()
{
  int n=0; 
  for(;;) 
  { 
    switch(menu_select()) 
    { 
    case 1:
      printf("\n    添加记录到通讯录\n"); 
      n=Input(pe,n); 
      printf("添加完毕，按任意键返回");
     fpurge(stdin);
     getchar(); 
     system("clear");
     break; 
    case 2:
      printf("\n    通讯录记录表\n"); 
      Display(pe,n); 
      printf("输出完毕，按任意键返回");
      fpurge(stdin);
      getchar(); 
      system("clear");
      break;
    case 3:
      printf("\n    从通讯录中删除记录\n");
      n=Delete_a_record(pe,n);
      printf("删除完毕，按任意键返回");
      fpurge(stdin);
      getchar(); 
      system("clear");
      break; 
    case 4:
      printf("\n    在通讯录中查找记录\n"); 
      Query_a_record(pe,n);
      printf("查找完毕，按任意键返回");
      fpurge(stdin);
      getchar(); 
      system("clear"); 
      break;
    case 5:
      printf("\n    修改通讯录中的记录\n"); 
      Change(pe,n);
      printf("修改完毕，按任意键返回");
      fpurge(stdin);
      getchar(); 
      system("clear");
      break; 
    case 6:
      printf("\n    保存功能\n"); 
      WritetoText(pe,n);
      printf("保存完毕，按任意键返回");
      fpurge(stdin);
      getchar(); 
      system("clear");
      break; 
    case 7:
      printf("\n    默认数据\n"); 
      DefaultData(pe,&n);
      printf("加载完毕，按任意键返回\n");
      Display(pe,n); 
      fpurge(stdin);
      getchar(); 
      system("clear");
      break; 
    case 0: 
      printf("\n    谢谢使用，再见!\n");
      fpurge(stdin);
      getchar(); 
      system("clear");
      exit(0); 
    } 
  } 
}