#include <stdio.h>    
#include <stdlib.h>    
#include <string.h>    
     
#define MAX 50    
   
typedef struct node1{   
  int school;  /*学校编号*/   
  int record;  /*项目成绩*/   
  struct node1 *next;  /*链域*/    
}Schools;  

typedef struct {   
  int item;  /*项目编号*/   
  Schools *firstschool; /*链域指向链表中第一个结点*/    
}ITEM;  

typedef struct {    
  int z; /* 项目总数 */   
  ITEM a[MAX];   
}ALLitems;   

typedef struct node2 {   
  int item;    /*该学校获奖的项目*/   
  int record;  /*项目成绩*/   
  struct node2 *next;  /*链域*/    
}Items;   

typedef struct {    
  int school;  /*学校编号*/   
  int score;  /*学校总分*/   
  int boys;  /*男团体总分*/   
  int girls;  /*女团体总分*/   
  Items *firstitem; /*链域指向链表中第一个获奖项目的结点*/    
}SCHNode;   

typedef struct {   
  int n; /* 学校总数 */   
  SCHNode b[MAX];   
}ALLNode;   

ALLitems *g1;   
ALLNode *g2;   

void enterScore(ALLitems *g1,ALLNode *g2)                      
{    
  Schools *p1;   
  Items *p2;   
  int i = 0,j,k,m,w,h,x;   
  p1=(Schools *)malloc(sizeof(Schools));            
  p2=(Items *)malloc(sizeof(Items));                
  if(!p1||!p2)
    exit(1);                                     

  printf("\n输入各个项目信息:\n");   
  printf("输入男子项目总数M(<=20):");   
  scanf("%d",&m);   
  if(m<0||m>20)    
  {
    printf("输入有误,m是20以内的整数,请重新输入:");   
    scanf("%d",&m);   
  }   
  printf("输入女子项目总数W(<=20):");   
  scanf("%d",&w);   
  if(w<0||w>20)    
  { 
    printf("输入有误,w是20以内的整数,请重新输入:");   
    scanf("%d",&w);   
  }   
  printf("输入参加运动会的学校总数N(<=20):");   
  scanf("%d",&g2->n);   
  if(g2->n<0||g2->n>20)    
  { 
    printf("输入有误,n是20以内的整数,请重新输入:");   
    scanf("%d",&g2->n);   
  }   
  g1->z=m+w;
  printf("则项目编号为男子1-%d,女子%d-%d",m,m+1,g1->z);   
  printf("\n\n记录运动会成绩");  
  printf("\n\n(输入0标志结束)\n");
  for(k=1;k<=g1->z;k++)             
  { 
    g1->a[k].item=k;   
    g1->a[k].firstschool=NULL;
  }   
  for(k=1;k<=g2->n;k++)   
  { 
    g2->b[k].school=k;   
    g2->b[k].firstitem=0;    
    g2->b[k].score=0;
    g2->b[k].boys=0;
    g2->b[k].girls=0;   
  }                                 
  g2->b[0].score=0;
  g2->b[0].boys=0;
  g2->b[0].girls=0; 
  scanf("%d",&i); 
  while(i!=0)   
  { 
    printf("\n项目编号:");   
    scanf("%d",&i);   
    if(i!=0)   
    { 
      printf("1.前三名   2.前五名\n");   
      printf("请选择:");    
      scanf("%d",&j);   
      if(j!=1&&j!=2)   
      { 
        printf(" 输入有误,请重新选择:");   
        scanf("%d",&j);   
      }   
      if(j==1)               
      {
        h=3;    
        do{ 
          printf("第%d名:学校(学校编号为数字(<50))",h);   
          scanf("%d",&x);   
          p1=(Schools *)malloc(sizeof(Schools));   
          p1->school=x;   
          p2=(Items *)malloc(sizeof(Items));   
          p2->item=i;   
          if(h==3) p2->record=p1->record=2;   
          if(h==2) p2->record=p1->record=3;   
          if(h==1) p2->record=p1->record=5;   
          p1->next=g1->a[i].firstschool;   
          g1->a[i].firstschool=p1;   
          p2->next=g2->b[x].firstitem;   
        g2->b[x].firstitem=p2; 
        /* 累计总分 */
        g2->b[x].score=g2->b[x].score+p2->record;
        /* 累计男团体总分 */
        if(i<=m) g2->b[x].boys=g2->b[x].boys+p2->record;
        /* 累计女团体总分 */
        else  g2->b[x].girls=g2->b[x].girls+p2->record;
        h--;   
        }while(x!=0&&h!=0);   
      }   
      if(j==2)              
      {
        h=5;    
        do{ 
          printf("第%d名:学校(学校编号为数字)",h);   
          scanf("%d",&x);   
          p1=(Schools *)malloc(sizeof(Schools));   
          p1->school=x;   
          p2=(Items *)malloc(sizeof(Items));   
          p2->item=i;   
          if(h==5) p2->record=p1->record=1;   
          if(h==4) p2->record=p1->record=2;   
          if(h==3) p2->record=p1->record=3;   
          if(h==2) p2->record=p1->record=5;   
          if(h==1) p2->record=p1->record=7;   
          p1->next=g1->a[i].firstschool;   
          g1->a[i].firstschool=p1;   
          p2->next=g2->b[x].firstitem;   
          g2->b[x].firstitem=p2; 
          /* 累计总分 */
          g2->b[x].score=g2->b[x].score+p2->record;
          /* 累计男团体总分 */
          if(i<=m) g2->b[x].boys=g2->b[x].boys+p2->record;
          /* 累计女团体总分 */
          else  g2->b[x].girls=g2->b[x].girls+p2->record;
          h--;   
        }while(x!=0&&h!=0);   
      }   
    }       
  }   
}   

void save()                                 
{
  FILE *fp1,*fp2;  
  fp1=(FILE *)malloc(sizeof(FILE));
  fp2=(FILE *)malloc(sizeof(FILE));
  if((fp1=fopen("/storage/emulated/0/cyuyan/profiles/sports1","wb"))==NULL)   
  {
    printf("cannot open file.\n");   
    return;   
  }
  if(fwrite(g1,sizeof(ALLitems),1,fp1)!=1)
    printf("file write error.\n");   
  fclose(fp1);   
  if((fp2=fopen("/storage/emulated/0/cyuyan/profiles/sports2","wb"))==NULL)   
  { printf("cannot open file.\n");   
  return;   
  }   
  if(fwrite(g2,sizeof(ALLNode),1,fp2)!=1)   
    printf("file write error.\n");   
  fclose(fp2);   
} 

/* 输出各学校总分 */
void outPutScore(ALLNode *g2)                     
{
  int k;   
  printf("****** 输出各学校总分 ******\n");   
  printf("学校编号  总分\n");   
  for(k=1;k<=g2->n;k++)   
    printf("%8d%6d\n",k,g2->b[k].score);   
  printf("\n");    
}  

/* 按学校编号排序输出 */ 
void sortBySchoolNo(ALLNode *g2)                            
{
  int k;   
  Items *p2;
  p2=(Items *)malloc(sizeof(Items));
  printf("****** 按学校编号排序输出 ******\n");   
  printf("学校编号    获奖情况  \n");   
  scanf("%d",&k);     

  for(k=1;k<=g2->n;k++)   
  { 
    printf("%8d\t",k);   
    p2=g2->b[k].firstitem;   
    while(p2!=NULL)   
    { 
      printf("项目%d:得%d分 ",p2->item,p2->record);    
      p2=p2->next;   
    }   
    printf("\n");   
  }   
  printf("\n");    
}   

/* 按学校总分排序输出 */
void sortBySchoolScore(ALLNode *g2)                            
{ 
  int i,j,k;  
  Items *p2; 
  printf("****** 按学校总分排序输出 ******\n");   
  printf("学校编号    总分\n");  
  scanf("%d",&k);     
  //printf("输入要查询的项目编号:");

  for(i=2;i<=g2->n;i++)   
  {
    printf("%8d",k);   
    p2=g2->b[k].firstitem;   
    while(p2!=NULL)   
    { 
      printf("%d    %d\n",g2->b[k].school,g2->b[k].score);  
      p2=p2->next;   
    }   
    printf("\n");   


    g2->b[0].score=g2->b[i].score;   
    g2->b[0].boys=g2->b[i].boys;   
    g2->b[0].girls=g2->b[i].girls;   
    g2->b[0].school=g2->b[i].school;   
    j=i-1;   
    while(g2->b[0].score<g2->b[j].score&&j>0)   
    { 
      g2->b[j+1].score=g2->b[j].score;   
      g2->b[j+1].boys=g2->b[j].boys;   
      g2->b[j+1].girls=g2->b[j].girls;   
      g2->b[j+1].school=g2->b[j].school;   
      j--;   
    }   
    g2->b[j+1].score=g2->b[0].score;   
    g2->b[j+1].boys=g2->b[0].boys;   
    g2->b[j+1].girls=g2->b[0].girls;   
    g2->b[j+1].school=g2->b[0].school;   
  }   
  for(k=1;k<=g2->n;k++)   
    printf("%d    %d\n",g2->b[k].school,g2->b[k].score);   
}   

/* 按男团体总分排序输出 */ 
void sortByManTeam(ALLNode *g2)
{ 
  int i,j,k;   
  Items *p2;
  p2=(Items *)malloc(sizeof(Items));
  printf("****** 按男团体总分排序输出 ******\n");
  printf("学校编号    男团体总分\n"); 
  scanf("%d",&k);     
  //printf("输入要查询的学校编号:");
  for(i=2;i<=g2->n;i++)   
  {                                                                   
    printf("%d\t",k);   
    p2=g2->b[k].firstitem;   
    while(p2!=NULL)   
    { printf("%d    %d\n",g2->b[k].school,g2->b[k].boys);  
    p2=p2->next;  
    }  
    printf("\n");                                                                             
    g2->b[0].score=g2->b[i].score;   
    g2->b[0].boys=g2->b[i].boys;   
    g2->b[0].girls=g2->b[i].girls;   
    g2->b[0].school=g2->b[i].school;   
    j=i-1;   
    while(g2->b[0].boys<g2->b[j].boys&&j>0)   
    { 
      g2->b[j+1].score=g2->b[j].score;   
      g2->b[j+1].boys=g2->b[j].boys;   
      g2->b[j+1].girls=g2->b[j].girls;   
      g2->b[j+1].school=g2->b[j].school;   
      j--;   
    }   
    g2->b[j+1].score=g2->b[0].score;   
    g2->b[j+1].boys=g2->b[0].boys;   
    g2->b[j+1].girls=g2->b[0].girls;   
    g2->b[j+1].school=g2->b[0].school;   
  }   
  for(k=1;k<=g2->n;k++)   
    printf("%d    %d\n",g2->b[k].school,g2->b[k].boys);    
}   

/* 按女团体总分排序输出 */
void sortByWomanTeam(ALLNode *g2)
{ 
  int i,j,k; 
  Items *p2;
  p2=(Items *)malloc(sizeof(Items));
  printf("****** 按女团体总分排序输出 ******\n");
  printf("学校编号    女团体总分\n");   

  scanf("%d",&k);     
  // printf("输入要查询的项目编号:");

  for(i=2;i<=g2->n;i++)   
  {
    printf("%d",k);   
    p2=g2->b[k].firstitem;
    while(p2!=NULL)   
    { printf("%d    %d\n",g2->b[k].school,g2->b[k].girls);  
    p2=p2->next;   
    }   
    printf("\n"); 



    g2->b[0].score=g2->b[i].score;   
    g2->b[0].boys=g2->b[i].boys;   
    g2->b[0].girls=g2->b[i].girls;   
    g2->b[0].school=g2->b[i].school;   
    j=i-1;   
    while(g2->b[0].girls<g2->b[j].girls&&j>0)   
    { g2->b[j+1].score=g2->b[j].score;   
    g2->b[j+1].boys=g2->b[j].boys;   
    g2->b[j+1].girls=g2->b[j].girls;   
    g2->b[j+1].school=g2->b[j].school;   
    j--;   
    }   
    g2->b[j+1].score=g2->b[0].score;   
    g2->b[j+1].boys=g2->b[0].boys;   
    g2->b[j+1].girls=g2->b[0].girls;   
    g2->b[j+1].school=g2->b[0].school;   
  }   

  for(k=1;k<=g2->n;k++)   
    printf("%d    %d\n",g2->b[k].school,g2->b[k].girls);    
}   

/* 按学校编号查询学校某个项目情况 */
void queryBySchoolNo(ALLNode *g2)
{
  int i,j;   
  Items *p2;   
  printf("****** 按学校编号查询学校某个项目情况 ******\n");   
  printf("输入要查询的学校编号:");   
  scanf("%d",&i);     
  printf("输入要查询的项目编号:");   
  scanf("%d",&j);            
  p2=g2->b[i].firstitem;   
  while(p2!=NULL)   
  {
    if(p2->item==j)   
      printf("学校编号:%d    项目%d:得%d分\n",i,p2->item,p2->record);   
    p2=p2->next;   
  }   
  printf("\n");    
}   

/* 按项目编号查询取得名次的学校 */
void queryBySchoolGrade(ALLitems *g1)
{
  int i;      
  Schools *p1;   
  printf("\n*** 按项目编号查询取得名次的学校 ***\n");   
  printf("输入要查询的项目编号:");   
  scanf("%d",&i);   
  printf("项目编号   取得名次的学校\n");   
  printf("%d\n",i);   
  p1=g1->a[i].firstschool;   
  while(p1!=NULL)   
  { printf(" 学校%d:得%d分  ",p1->school,p1->record);   
  p1=p1->next;   
  }   
  printf("\n\n");     
}    

/* 菜单 */
void menu()
{
  printf("* * * * * * * * * * * * * * *\n");   
  printf("        运动会分数统计系统     \n");   
  printf("* * * * * * * * * * * * * * *\n");   
  printf("* 1.输入各项目成绩          *\n");   
  printf("* 2.统计各学校总分          *\n");   
  printf("* 3.按学校编号排序输出      *\n");   
  printf("* 4.按学校总分排序输出      *\n");   
  printf("* 5.按男团体总分排序输出    *\n");   
  printf("* 6.按女团体总分排序输出    *\n");   
  printf("* 7.按学校编号查询学校项目  *\n");   
  printf("* 8.按项目号查取得名次学校  *\n");   
  printf("* 0.退出程序                *\n");   
  printf("* * * * * * * * * * * * * * *\n"); 
  printf("   请选择(0-8):");  
}

int main()   
{
  int choice;   
  g2=(ALLNode*)malloc(sizeof(ALLNode));
  g1=(ALLitems*)malloc(sizeof(ALLitems));
  if(!g2||!g1)
    exit(1);
   
  menu();
  scanf("%d", &choice);

  for(;;)     
  {  
    switch(choice)   
    { 
    case 1:
      enterScore(g1,g2);
      save();
      printf("输入完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 2:
      outPutScore(g2);
      printf("输出完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 3:
      sortBySchoolNo(g2);
      printf("排序完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 4:
      sortBySchoolScore(g2);
      printf("读取完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 5:
      sortByManTeam(g2);
      printf("排序完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 6:
      sortByWomanTeam(g2);
      printf("排序完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 7:
      queryBySchoolNo(g2);
      printf("查询完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 8:
      queryBySchoolGrade(g1);
      printf("查询完毕，按回车键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;   
    case 0:
      exit(0);   
    default:
      break;  
    }
    if(choice == 0)
    {
      return 0;
    }

    menu();
    scanf("%d",&choice);
  }   
}