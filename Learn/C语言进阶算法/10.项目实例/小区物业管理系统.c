#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <malloc.h>

/* 业主结构体 */
struct yezhu {
  char id[100];
  char name[20]; 
  int age; 
  int family_num; 
  int floor_num; 
  char room_num[100];
  float lost; 
  int area; 
  char rz_time[20]; 
  struct yezhu *next;
};

/* 物业信息结构体 */
struct wuye {
  char ID[100];  
  int last_year; 
  int last_month; 
  float money;
  int to_time;
  struct wuye *link; 
};
struct yezhu *phead; //业主头结点
struct wuye *pHead; //物业头结点

/* 主菜单 */
int mainmenu() 
{ 
  int n; 
  printf("**欢迎进入小区物业管理系统**\n");
  printf("----------------------------\n"); 
  printf("* * * 〓请选择〓（1～3）* * *\n");
  printf("* *\n"); 
  printf("* *\n");
  printf("* ⒈物业信息系统； *\n"); 
  printf("* *\n");
  printf("* ⒉统计查询系统； *\n");
  printf("* *\n");
  printf("* ⒊退出程序。 *\n");
  printf("* *\n");
  printf("----------------------------\n"); 
  printf("请您输入选项★№:");
  scanf("%d",&n);
  return n;
} 

/* 添加业主信息结点 */
struct yezhu *AddYezhuNode(struct yezhu *p)
{ 
  struct yezhu *t;
  if(phead==NULL) 
  { 
    phead=p; 
    p->next=NULL;
  } 
  else 
  { 
    t=phead; 
    while(t->next!=NULL) 
      t=t->next; 
    t->next=p; 
    p->next=NULL; 
    return phead; 
  } 
}

/* 查找物业信息 */
struct wuye *findWyById(char *id)
{ 
  struct wuye *p;
  p=pHead;
  while(p!=NULL) 
  { 
    if(strcmp(p->ID,id)==0)
      return p;
    p=p->link;
  }
  return NULL; 
}

/* 通过ID查找业主信息 */
struct yezhu *findYzById(char *id)
{ 
  struct yezhu *p; 
  p=phead; 
  while(p!=NULL) 
  { 
    if(strcmp(p->id,id)==0) 
      return p; 
    p=p->next; 
  }
  return NULL;
}

void Display(struct wuye *p)
{ 
  printf("业主ID:%s\n",p->ID);
  printf("最后一次缴费日期：%d年%d月\n",p->last_year,p->last_month);
  printf("最后一次缴费金额：%f\n",p->money);
  printf("费用有效期：%d个月\n",p->to_time);
} 

/* 欠费名单 */
void mychaozhao()// 
{ 
  int year,month; 
  struct wuye *k;
  struct yezhu *l;
  k=pHead; 
  l=phead;
  printf("请输入当前年份：");
  scanf("%d",&year);
  printf("请输入当前月份：");
  scanf("%d",&month);
  while(k!=NULL)
  { 
    if(year>k->last_year) 
    { 
      l=findYzById(k->ID);
      l->lost=(float)(month+12-k->to_time-k->last_month)*(0.4*l->area);
      if(l->lost>0) 
      { 
        printf("业主ID*:%s\n",l->id);
        printf("业主姓名*：%s\n",l->name);
        printf("最后一次缴费日期：%d年%d月\n",k->last_year,k->last_month);
        printf("欠费金额*：%f\n",l->lost);
        printf("\n\n");
      } 
    } 
    if(year==k->last_year) 
    { 
      l=findYzById(k->ID);
      l->lost=(float)(month-k->to_time-k->last_month)*(0.4*l->area); 
      if(l->lost>0) 
      { 
        printf("业主ID:%s\n",l->id); 
        printf("业主姓名：%s\n",l->name);
        printf("最后一次缴费日期：%d年%d月\n",k->last_year,k->last_month);
        printf("欠费金额：%f\n",l->lost); printf("\n\n"); 
      } 
    } 
    k=k->link;
  }
}

/* 查询缴费 */
void myfind() 
{ 
  struct wuye *p;
  char a[100];
  printf("请输入要查找的业主id：");
  scanf("%s",a);
  p=findWyById(a);
  if(p!=NULL)
  {
    printf("业主已经存缴!\n");
  }
  else 
  { 
    printf("业主尚没存缴信息!\n");
    return;
  } 
  Display(p);
}

/* 统计查询菜单显示 */
void tongjimenu()
{
  int choose; 
  do { 
    printf("********统计查询系统********\n");
    printf("----------------------------\n"); 
    printf("* * * 〓请选择〓（1～3）* * *\n");
    printf("* *\n"); 
    printf("* *\n");
    printf("* ⒈查询缴费信息； *\n");
    printf("* *\n");
    printf("* ⒉欠费名单一览表； *\n"); 
    printf("* *\n"); 
    printf("* ⒊返回主菜单。 *\n");
    printf("* * * * * * * * * * * * * * *\n"); 
    printf("请您输入选项★№:");
    scanf("%d",&choose);
    switch(choose) 
    { 
    case 1 : myfind();
      break;
    case 2 : mychaozhao();
      break;
    case 3 : return;
    default: printf("输入错误，请重新输入!\n\n");
    } 
  } while(1);
} 

/* 保存业主信息至文件 */
void SaveYezhu(struct yezhu *p)
{ 
  FILE *fp; 
  fp=fopen("/storage/emulated/0/cyuyan/profiles/yezhu.dat","wb");
  p=phead;
  while(p!=NULL)
  { 
    fwrite(p,sizeof(struct yezhu),1,fp);
    p=p->next;
  }
  fclose(fp);
}

/* 录入业主信息 */
void AddYezhu()
{ 
  struct yezhu *p;
  p=(struct yezhu *)malloc(sizeof(struct yezhu));
  printf("请录入业主信息：\n"); 
  printf("ID:");
  scanf("%s",p->id);
  printf("姓名:");
  scanf("%s",p->name);
  printf("年龄："); 
  scanf("%d",&p->age);
  printf("家庭成员数：");
  scanf("%d",&p->family_num);
  printf("所在楼号："); 
  scanf("%d",&p->floor_num);
  printf("单元房间号:");
  scanf("%s",p->room_num);
  printf("住宅面积:");
  scanf("%d",&p->area); 
  printf("入住时间：");
  scanf("%s",p->rz_time);
  AddYezhuNode(p); 
  SaveYezhu(p);
  printf("录入成功!\n");
} 


/* 修改业主信息 */
void UpdateYezhu()
{ 
  struct yezhu *k; 
  char id[100]; 
  printf("请输入要修改的业主ID:"); 
  scanf("%s",id); 
  k=findYzById(id); 
  if(k!=NULL) 
  {
    printf("请输入要修改的业主信息：\n"); 
    printf("ID:"); 
    scanf("%s",k->id); 
    printf("姓名:"); 
    scanf("%s",k->name); 
    printf("年龄："); 
    scanf("%d",&k->age); 
    printf("家庭成员数："); 
    scanf("%d",&k->family_num); 
    printf("所在楼号："); 
    scanf("%d",&k->floor_num); 
    printf("单元房间号:"); 
    scanf("%s",k->room_num); 
    printf("住宅面积:"); 
    scanf("%d",&k->area); 
    printf("入住时间："); 
    scanf("%s",k->rz_time); 
    SaveYezhu(k); 
    printf("修改成功!\n");
  }
  else 
  { 
    printf("无此业主ID!\n"); 
    return; 
  } 
} 

/* 保存物业信息至文件 */
void SaveWuye(struct wuye *p)
{ 
  FILE *fp;
  fp=fopen("/storage/emulated/0/cyuyan/profiles/wuye.dat","wb"); 
  p=pHead;
  while(p!=NULL)
  { 
    fwrite(p,sizeof(struct wuye),1,fp);
    p=p->link;
  } 
  fclose(fp);
} 

/* 删除业主信息 */
void DelYezhu()
{ 
  struct yezhu *p,*pp; 
  struct wuye *k,*kk; 
  char id[100];
  pp=phead;
  kk=pHead;
  printf("请输入要删除人的ID:"); 
  scanf("%s",id);
  p=findYzById(id);
  k=findWyById(id);
  if(p!=NULL)
  {
    if(p==phead) 
    { 
      phead=p->next;
      free(p);
    } 
    else 
    { 
      while(pp->next!=p) 
        pp=pp->next;
      pp->next=p->next;
      free(p);
    }
  }
  else 
  { 
    printf("无此业主ID!\n");
    return;
  } 
  printf("删除业主信息成功!\n"); 
  SaveYezhu(p); 
  if(k!=NULL) 
  {
    if(k==pHead)
    { 
      pHead=k->link; 
      free(k); 
    }
    else 
    { 
      while(kk->link!=k) 
        kk=kk->link; 
      kk->link=k->link; 
      free(k); 
    } 
  }
  else 
  { 
    printf("无此业主ID!\n"); 
    return; 
  } 
  printf("删除物业信息成功!\n");
  SaveWuye(k); 
} 


/* 浏览业主信息 */
void DisplayYezhu()
{
  struct yezhu *p;
  p=phead;
  while(p!=NULL) 
  { 
    printf("\n\n");
    printf("ID:%s\n",p->id);
    printf("姓名:%s\n",p->name);
    printf("年龄：%d\n",p->age); 
    printf("家庭成员数：%d\n",p->family_num);
    printf("所在楼号：%d\n",p->floor_num); 
    printf("单元房间号:%s\n",p->room_num);
    printf("住宅面积:%d\n",p->area); 
    printf("入住时间：%s\n",p->rz_time); 
    printf("\n\n");
    p=p->next;
  } 
}

/* 业主菜单函数 */
void yezhumenu()
{
  int choose;
  do {
    printf("******业主信息编辑系统******\n"); 
    printf("----------------------------\n");
    printf("* * * 〓请选择〓（1～5）* * *\n");
    printf("* *\n"); 
    printf("* *\n");
    printf("* ⒈业主信息录入； *\n"); 
    printf("* *\n");
    printf("* ⒉业主信息修改； *\n"); 
    printf("* *\n"); 
    printf("* ⒊业主信息删除； *\n");
    printf("* *\n");
    printf("* ⒋业主信息浏览； *\n");
    printf("* *\n"); 
    printf("* ⒌返回上级菜单。 *\n");
    printf("* * * * * * * * * * * * * * *\n"); 
    printf("请输入选项★№："); 
    scanf("%d",&choose);
    //getchar(); 
    switch(choose) 
    { 
    case 1 :AddYezhu();
      break; 
    case 2 :UpdateYezhu();
      break; 
    case 3 :DelYezhu(); 
      break; 
    case 4 :DisplayYezhu();
      break; 
    case 5 : return;
    default: printf("输入错误，请重新输入：\n");
    } 
  } while(1);
} 

/* 添加物业信息结点 */ 
struct wuye *AddWyNode(struct wuye *p)
{ 
  struct wuye *t;
  if(pHead==NULL) 
  { 
    pHead=p; 
    p->link=NULL; 
  }
  else 
  { 
    t=pHead;
    while(t->link!=NULL) 
      t=t->link;
    t->link=p;
    p->link=NULL;
  }
  return pHead;
} 


/* 录入物业信息 */ 
void AddWuye()
{ 
  struct wuye *p;
  struct yezhu *t;
  int x;
  p=(struct wuye *)malloc(sizeof(struct wuye)); 
  printf("请录入物业信息：\n");
  printf("ID:");
  scanf("%s",p->ID);
  t=findYzById(p->ID);
  if(t!=NULL)
  {
    printf("上一次缴费年份：");
    scanf("%d",&p->last_year);
    printf("月份：");
    scanf("%d",&p->last_month);
    printf("缴费金额：");
    scanf("%f",&p->money);
    x=p->money/(0.4*t->area);
    p->to_time=(int)x;
    AddWyNode(p);
    SaveWuye(p);
    printf("录入成功!\n");
  }
  else 
  { 
    printf("无此业主ID!\n");
    return;
  } 
} 




/* 修改物业信息 */
void UpdateWuye()//
{ 
  char id[20];
  int x;
  struct wuye *k;
  struct yezhu *t;
  printf("请输入要修改的业主ID:");
  scanf("%s",id);
  if(k!=NULL)
  {
    printf("您要修改的信息为：\n");
    printf("请输入新的信息：\n");
    printf("ID:");
    scanf("%s",k->ID);
    printf("上一次缴费年份：");
    scanf("%d",&k->last_year);
    printf("月份：");
    scanf("%d",&k->last_month);
    printf("缴费金额：");
    scanf("%f",&k->money);
    t=findYzById(k->ID);
    x=(int)k->money/(0.4*t->area);
    k->to_time=(int)x;
    AddWyNode(k);
    SaveWuye(k);
    printf("修改成功!\n");
  }
  else 
  { 
    printf("无此业主ID!\n");
    return;
  }
} 

/* 删除物业信息 */
void DelWuye()// 
{ 
  struct wuye *p,*pp; 
  char id[100];
  pp=pHead;
  printf("请输入要删除人的ID:");
  scanf("%s",id); 
  p=findWyById(id);
  if(p!=NULL)
  {
    if(p==pHead)
    { 
      pHead=p->link;
      free(p); 
    }
    else
    { 
      while(pp->link!=p) 
        pp=pp->link;
      pp->link=p->link; 
      free(p);
    } 
  }
  else 
  { 
    printf("无此业主ID!\n");
    return;
  } 
  SaveWuye(p);
  printf("删除成功!\n");
}

/* 浏览物业信息 */
void DisplayWuye() 
{ 
  struct wuye *p;
  p=pHead; 
  while(p!=NULL) 
  { 
    printf("ID:%s\n",p->ID);
    printf("最后一次缴费日期：%d年%d月\n",p->last_year,p->last_month); 
    printf("缴费金额:%f元\n",p->money);
    printf("费用有效期：%d个月\n",p->to_time);
    printf("\n\n\n"); 
    p=p->link;
  } 
} 


/* 从文件中获取物业信息 */
void ReadWuye()
{ 
  FILE *fp; 
  struct wuye *p;
  fp=fopen("/storage/emulated/0/cyuyan/profiles/wuye.dat","w");
  while(1) 
  { 
    p=(struct wuye *)malloc(sizeof(struct wuye));
    if(feof(fp)) 
      break;
    if(fread(p,sizeof(struct wuye),1,fp)) 
      AddWyNode(p);
  } 
  fclose(fp);
}

/* 物业菜单 */
void wuyemenu() 
{ 
  int choose=0;
  do { 
    printf("******物业费用管理系统******\n");
    printf("----------------------------\n");
    printf("* * * 〓请选择〓（1～5）* * *\n"); 
    printf("* * \n");
    printf("* *\n");
    printf("* ⒈物业信息录入； *\n"); 
    printf("* *\n");
    printf("* ⒉物业信息修改； *\n");
    printf("* *\n");
    printf("* ⒊物业信息删除； *\n");
    printf("* *\n"); 
    printf("* ⒋物业信息浏览； *\n");
    printf("* *\n");
    printf("* ⒌返回上级菜单。 *\n");
    printf("* * * * * * * * * * * * * * *\n");
    printf("请输入选项："); 
    scanf("%d",&choose);
    switch(choose) 
    { 
    case 1: AddWuye();
      break;
    case 2: UpdateWuye();
      break;
    case 3: DelWuye();
      break;
    case 4: DisplayWuye();
      break;
    case 5: return;
    default:printf("输入错误，请重新输入：\n");
    } 
  } while(1);
} 


/* 物业信息菜单显示 */
void xinximenu()
{ 
  int choose; 
  do { 
    printf("********物业信息系统********\n"); 
    printf("----------------------------\n"); 
    printf("* * * 〓请选择〓（1～3）* * *\n"); 
    printf("* *\n");
    printf("* *\n");
    printf("* ⒈小区业主信息管理； *\n"); 
    printf("* *\n"); 
    printf("* ⒉物业费用信息管理； *\n"); 
    printf("* *\n");
    printf("* ⒊返回主菜单。 *\n"); 
    printf("* * * * * * * * * * * * * * *\n"); 
    printf("请您输入选项★№:"); 
    scanf("%d",&choose); 
    switch(choose) 
    {
    case 1:yezhumenu();
      break;
    case 2:wuyemenu();
      break; 
    case 3:return;
    default:printf("输入错误，请重新输入：\n");
    } 
  } while(1);
}

/* 从文件中获取业主信息 */
void ReadYezhu() 
{ 
  FILE *fp;
  struct yezhu *p;
  fp=fopen("yezhu.dat","w");
  while(1)
  { 
    p=(struct yezhu *)malloc(sizeof(struct yezhu));
    if(feof(fp)) 
    {
      break;
    }
    if(fread(p,sizeof(struct yezhu),1,fp)) 
    {
      AddYezhuNode(p);
    }
  } 
  fclose(fp);
}

int main()//
{ 
  int m; 
//  ReadYezhu();
//  ReadWuye();//第一次没有信息，不可用！！！
  do { 
    m=mainmenu(); 
    switch(m) 
    { 
    case 1: 
      xinximenu();
      break; 
    case 2: 
      tongjimenu();
      break; 
    case 3:
      printf("******谢谢使用，再见!*******\n"); 
      exit(0); 
    } 
  } while(1); 
}