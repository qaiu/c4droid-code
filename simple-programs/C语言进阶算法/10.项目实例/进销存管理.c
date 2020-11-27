#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN sizeof(struct object)
#define FORMAT1 "\t%-8d%-11s%-9.2f%-6d%-4d\n"
#define DATA1 (p1+i)->num,(p1+i)->name,(p1+i)->inprice,(p1+i)->inyear,(p1+i)->inmonth
#define FORMAT2  "\t%-8d%-11s%-9.2f%-9.2f%-7.2f%-6d%-4d%-6d%-4d\n"
#define DATA2  (p1+i)->num,(p1+i)->name,(p1+i)->inprice,(p1+i)->outprice,(p1+i)->profit,(p1+i)->inyear,(p1+i)->inmonth,(p1+i)->outyear,(p1+i)->outmonth

struct object
{
  int num,inyear,inmonth,outyear,outmonth;
  char name[10];
  float inprice,outprice,profit;
} obj[100];
struct object   *p1=obj;

/* 菜单 */
void menu()
{
  printf("\n");
  printf("*****进销存管理系统****\n");   
  printf("    1.进货操作\n");
  printf("    2.售货操作\n");
  printf("    3.修改信息\n");
  printf("    4.账目清单\n");
  printf("    5.月利润查询\n");
  printf("    0 退出程序\n");
  printf("*************************\n");
  printf("%s\n", "请选择:");
}

/* 进货操作 */
void in()
{
  int i,m=0;
  char ch[2];
  FILE *p;
  if( (p=fopen("/storage/emulated/0/cyuyan/profiles/data.txt","a+"))==NULL)
  {
    printf("打不开文件！\n");   //打开文件
    return;
  }
  while(!feof(p))
  {
    if(fread(&obj[m],LEN,1,p)==1)  
      m++;                    //将数据读入内存
  }
  fclose(p);
  if(m==0)
    printf("无记录！\n");

  printf("请确认是否进货？（y/n）");
  scanf("%s",ch);
  while(strcmp(ch,"y")==0||strcmp(ch,"y")==0)
  {
    printf("请输入设备编号：");
    scanf("%d",&obj[m].num);
    for(i=0;i<m;i++)
      if((p1+i)->num==obj[m].num)
      {
        printf("此号已存在，按任意键继续！");
        getchar();
        fclose(p);
        return;
      }
      printf("设备名称：");
      scanf("%s",obj[m].name);
      printf("进价：");
      scanf("%f",&obj[m].inprice);
      printf("进货年、月：");
      scanf("%d%d",&obj[m].inyear,&obj[m].inmonth);
      obj[m].outprice=0;obj[m].outyear=0;(p1+m)->outmonth=0;
      fopen("data","w");                     //打开文件用以向磁盘导入数据
      for(i=0;i<=m;i++)
        fwrite(&obj[i],LEN,1,p);

      if(fwrite(&obj[m],LEN,1,p)!=1)
      {
        printf("无法保存！");
      }
      else 
      {
        printf("%s 保存成功！\n",obj[m].name);
        m++;
      }
      printf("继续？（y/n）");
      scanf("%s",ch);
  }
  fclose(p);
  printf("保存完毕！\n");
}

/* 售货操作 */
void sell()
{
  FILE *p;
  int snum,i,j,m=0;
  char ch[2];
  if((p=fopen("/storage/emulated/0/cyuyan/profiles/data.txt","rb"))==NULL)
  {
    printf("无法打开文件！\n");
    return;
  }                                          //打开文件
  while(!feof(p))
    if(fread(&obj[m],LEN,1,p)==1)
      m++;                              //读入
  fclose(p);
  if(m==0)
  {
    printf("无记录！\n");
    return;
  }
  printf("请输入编号：");
  scanf("%d",&snum);
  for(i=0;i<m;i++)
    if(snum==(p1+i)->num)
      break;
  if(i<m) 
  {
    printf("查得此物品，确认出售？（y/n）");
    scanf("%s",ch);
    if(strcmp(ch,"y")==0||strcmp(ch,"Y")==0)
    {
      printf("请输入售价：");   
      scanf("%f",&(p1+i)->outprice);
      (p1+i)->profit=((p1+i)->outprice-(p1+i)->inprice);
      printf("\n请输入售出年、月：");
      scanf("%d%d",&(p1+i)->outyear,&(p1+i)->outmonth);
      if((p=fopen("data","wb+"))==NULL)
      {
        printf("打不开文件！\n");
        return;
      }
      for(j=0;j<m;j++)
      {
        if(fwrite(&obj[j],LEN,1,p)!=1)
          printf("无法保存！\n");
      }

      printf("售货成功！");
      fclose(p);
      return;

    }
    else 
      return;
  }
  else 
    printf("无此物品！");
  return; 
}

/* 修改信息 */
void modify()
{
  FILE *p;
  int i,j,m=0,snum;
  if((p=fopen("/storage/emulated/0/cyuyan/profiles/data.txt","rb"))==NULL)
  {printf("打不开文件！\n");
  return;
  }
  while(!feof(p))
    if(fread(&obj[m],LEN,1,p)==1)
      m++;
  if(m==0)
  {
    printf("无记录！\n");
    fclose(p);
    return;
  }
  printf("请输入欲修改信息的货物编号：");
  scanf("%d",&snum);
  for(i=0;i<m;i++)
    if(snum==(p1+i)->num)
      break;
  if(i<m)                   
  {
    printf("找到此货物！\n");
    printf("\n名称：");
    scanf("%s",(p1+i)->name);
    printf("\n进价：");
    scanf("%f",&(p1+i)->inprice);
    printf("\n进货年、月：");
    scanf("%d%d",&(p1+i)->inyear,&(p1+i)->inmonth);
    if((p=fopen("data","wb+"))==NULL)
    {
      printf("打不开文件！\n");
      return;
    }
    for(j=0;j<m;j++)
      if(fwrite(&obj[j],LEN,1,p)!=1)
      {
        printf("无法保存！");
      }
      fclose(p);
  }
  else 
    printf("无此货物！");
  return;
}

/* 账目清单 */
void show()
{
  FILE *p;
  int i,m=0;
  if((p=fopen("/storage/emulated/0/cyuyan/profiles/data.txt","rb"))==NULL)
    printf("打不开！");
  while(!feof(p))
    if(fread(&obj[m],LEN,1,p)==1)
      m++;     
  fclose(p);
  printf("已售\n编号    名称       进价     售价     利润   进货时间  出售时间\n");
  for(i=0;i<m;i++)
  {
    if((p1+i)->profit!=0)
      printf(FORMAT2,DATA2);
  }
  printf("仓储\n");
  printf("编号    名称       进价     进货时间\t\n");
  for(i=0;i<m;i++)
  {
    if((p1+i)->profit==0)
      printf(FORMAT1,DATA1);
  }
}

/* 月利润 */
void monthearn()
{
  FILE *p;
  int m=0,s=0,a=0,b=0,c=0,d=0,y,z;
  float sum1=0,sum2=0,sum3=0,sum4=0;
  if((p=fopen("/storage/emulated/0/cyuyan/profiles/data.txt","rb"))==NULL)
  {
    printf("打不开文件！\n");
    return;                                  
  }                            //打开文件
  while(!feof(p))
    if(fread(&obj[m],LEN,1,p)==1)
      m++;
  fclose(p);                      //将数据读入内存
  printf("请输入欲查询的年份、月份：");
  scanf("%d%d",&y,&z);
  for(s=0;s<m;s++)
  {
    if((strcmp(obj[s].name,"zhuji")==0)&&(y==obj[s].outyear)&&(z==obj[s].outmonth))
    {
      a++;
      sum1+=obj[s].profit;
    }
    if((strcmp(obj[s].name,"xianshiqi")==0)&&(y==obj[s].outyear)&&(z==obj[s].outmonth))
    {
      b++;
      sum2+=obj[s].profit;
    }
    if((strcmp(obj[s].name,"jianpan")==0)&&(y==obj[s].outyear)&&(z==obj[s].outmonth))
    {
      c++;
      sum3+=obj[s].profit;
    }

    if((strcmp(obj[s].name,"shubiao")==0)&&(y==obj[s].outyear)&&(z==obj[s].outmonth))
    {
      d++;
      sum4+=obj[s].profit;
    }
  }
  printf("本月共售出\n");
  printf("主机   %d台，总利润 %7.2f 元\n",a,sum1);  
  printf("显示器 %d台，总利润 %7.2f 元\n",b,sum2);
  printf("键盘   %d台，总利润 %7.2f 元\n",c,sum3);
  printf("鼠标   %d台，总利润 %7.2f 元\n",d,sum4);

}

int main()
{     
  int n;
  menu();
  scanf("%d", &n);

  while(n)
  {
    switch(n)
    {
    case 1: 
      in();
      printf("操作完毕,按任意键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;
    case 2:
      sell();
      printf("操作完毕,按任意键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;
    case 3:
      modify();
      printf("操作完毕,按任意键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear");
      break;
    case 4:
      show();
      printf("操作完毕,按任意键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;
    case 5:
      monthearn();
      printf("操作完毕,按任意键返回");
      setbuf(stdin,NULL);
      getchar(); 
      system("clear"); 
      break;
    default:
      break;
    }

    if(n == 0)
    {
      return 0;
    }
    menu();
    scanf("%d",&n);
  }
}