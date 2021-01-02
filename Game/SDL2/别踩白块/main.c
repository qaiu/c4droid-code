/*c4droid代码手册
别踩白块儿
风飞飘扬是本人 编写
*/
#include "SDL2/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
//禅模式时间
#define T 20
//存储方块坐标
typedef struct
{
  double x;
  double y;
  int type;
  float area;
} Coor;
//存储排行榜数据
typedef struct Data
{
  char name[20][25];
  int score[20];
} Data;
int Time1,Time2;
SDL_Window *Win;
SDL_Renderer *Renderer;
//颜色表面
SDL_Texture *Rect_white,*Rect_black,*Rect_red,*Rect_yellow,*Rect_gray;
Coor coor[5];
Mix_Chunk *Sound[9];
Mix_Music *Timelose;
bool Volume=false;
int W,H;
long score_best[2];

//获取分数(-1初始化,0不变,1加1)
long GetScore(int n)
{
  //用数组存储分数，防止修改
  static char s[10]={0};
  if(n==-1)
  {
    for(int i=0;i<10;i++)
      s[i]=115;
    return 0;
  }
  if(n==1)
    s[9]++;
  for(int i=9;i>0;i--)
    if(s[i]-115>9)
    {
      s[i]=115;
      s[i-1]++;
    }
  //字符数组转换成数字
  long i=0;
  for(int j=0;j<10;j++)
    i+=(s[9-j]-115)*pow(10,j);
  return i;
}
//获取最佳成绩
int GetBest(int i)
{
  FILE *fp;
  int size;
  i--;
  fp=fopen("score_V2.1_.dat","rb+");
  if(fp==NULL)
  {
    fp=fopen("score_V2.1_.dat","wb+");
    fwrite(score_best,sizeof(score_best),1,fp);
    fflush(fp);
  }
  else
  {
    rewind(fp);
    fread(score_best,sizeof(score_best),1,fp);
  }
  if(GetScore(0)>score_best[i])
  {
    score_best[i]=GetScore(0);
    rewind(fp);
    fwrite(score_best,sizeof(score_best),1,fp);
  }
  fclose(fp);
}
//创建空表面
SDL_Surface *GetSurface()
{
  SDL_Surface *surface;
  surface=SDL_CreateRGBSurface(0,W,H,32,0,0,0,0);
  return surface;
}
//初始化
int Init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  SDL_Surface *rect_white,*rect_black,*rect_red,*rect_yellow,*rect_gray;
  Win=SDL_CreateWindow(" ",0,0,0,0,0);
  //获取分辨率
  SDL_GetWindowSize(Win,&W,&H); Renderer=SDL_CreateRenderer(Win,-1,SDL_RENDERER_ACCELERATED);
  rect_white=GetSurface();
  rect_black=GetSurface();
  rect_red=GetSurface();
  rect_yellow=GetSurface();
  rect_gray=GetSurface();
  SDL_FillRect(rect_white,NULL,0xffffff);
  SDL_FillRect(rect_black,NULL,0x000000);
  SDL_FillRect(rect_red,NULL,0xff0000);
  SDL_FillRect(rect_yellow,NULL,0xffff00);
  SDL_FillRect(rect_gray,NULL,0xc0c0c0);
  Rect_white=SDL_CreateTextureFromSurface(Renderer,rect_white);
  Rect_black=SDL_CreateTextureFromSurface(Renderer,rect_black);
  Rect_red=SDL_CreateTextureFromSurface(Renderer,rect_red);
  Rect_yellow=SDL_CreateTextureFromSurface(Renderer,rect_yellow);
  Rect_gray=SDL_CreateTextureFromSurface(Renderer,rect_gray);
  SDL_FreeSurface(rect_white);
  SDL_FreeSurface(rect_black); 
  SDL_FreeSurface(rect_red); 
  SDL_FreeSurface(rect_yellow); 
  SDL_FreeSurface(rect_gray); 
  Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
  //加载音调音效
  char s[50];
  for(int i=0;i<9;i++)
  {
    sprintf(s,"s%d.wav",i);
    Sound[i]=Mix_LoadWAV(s);
  }
  //加载时间音效
  Timelose=Mix_LoadMUS("timelose.wav");
  //设置渲染器颜色为白色
  SDL_SetRenderDrawColor(Renderer,0xff,0xff,0xff,0xff);
}
//从渲染器截取表面
SDL_Surface* ScreenShot()
{
  SDL_Rect real_rect;
  Uint32 format;
  int bpp;
  Uint32 rmask,gmask,bmask,amask;
  char*pixels;
  int pitch;
  real_rect.x=0;
  real_rect.y=0;
  real_rect.w=W;
  real_rect.h=H;
  format=SDL_GetWindowPixelFormat(Win);
  SDL_PixelFormatEnumToMasks(format,&bpp,&rmask,&gmask,&bmask,&amask); 
  pitch=SDL_BYTESPERPIXEL(format)*(real_rect.w);
  pixels=(char*)malloc(pitch*(real_rect.h)*sizeof (char));
  SDL_RenderReadPixels(Renderer,&real_rect,format,(void*)pixels,pitch);
  return SDL_CreateRGBSurfaceFrom((void*)pixels,real_rect.w,real_rect.h,bpp,pitch,rmask,gmask,bmask,amask);
}
//打印表面
void Blit(double x,double y,int w,int h,SDL_Texture *Draw_Texture)
{
  SDL_Rect rect;
  rect.x=x*W;
  rect.y=y*H;
  if(w==0||h==0)
    SDL_QueryTexture(Draw_Texture,NULL,NULL,&w,&h);
  rect.w=w;
  rect.h=h;
  //坐标修正
  if(h!=H&&h!=1)
    rect.h++;
  if(w!=W&&W%2!=0&&w!=1)
    rect.w++;
  SDL_RenderCopy(Renderer,Draw_Texture,NULL,&rect);
}
//创建字体表面
SDL_Texture *Text(char *Fonts,int fontsize,int r,int g,int b)
{
  SDL_Color color;
  color.r=r;
  color.g=g;
  color.b=b;
  color.a=0;
  SDL_Surface *temp=NULL;
  static TTF_Font *library;
  static int size=0;
  if(!fontsize)
    return NULL;
  //新字体大小打开新字体库
  if(size!=fontsize)
  {
    size=fontsize;
    if(size)
      TTF_CloseFont(library);
    library=TTF_OpenFont("../../../Resources/Font/D2-coding.ttf",size*W/480);
  }
  temp=TTF_RenderUTF8_Blended(library,Fonts,color);
  SDL_Texture *fonts=NULL;
  fonts=SDL_CreateTextureFromSurface(Renderer,temp);
  SDL_FreeSurface(temp);
  return fonts;
}
//查询分数长度
int ScoreDigit()
{
  int n;
  for(n=1;GetScore(0)/(int)pow(10,n);n++);
  if(n)
    n--;
  return n;
}
//打印游戏界面
void draw()
{
  static SDL_Texture *score=NULL;
  static int scorel=-1;
  if(GetScore(0)!=scorel)
  {
    char s[10];
    SDL_DestroyTexture(score);
    sprintf(s,"%d",GetScore(0));
    score=Text(s,50,255,0,0);
  }
  SDL_RenderClear(Renderer);
  //打印方块
  for(int i=0;i<5;i++)
  {
    if(coor[i].type==1||coor[i].type==0&&coor[i].area!=1)
      Blit(coor[i].x,coor[i].y,W/4,H/4,Rect_black);
    if(coor[i].type==0)
    {
      Blit(coor[i].x+0.125*(1-coor[i].area),coor[i].y+0.125*(1-coor[i].area),W/4*coor[i].area,H/4*coor[i].area,Rect_gray);
      if(coor[i].area<1)
        coor[i].area+=0.05;
    }
  }
  //打印线条
  for(int i=0;i<=3;i++)
  {
    Blit(0,coor[1].y+i*0.25,W,1,Rect_black);
    Blit(i*0.25,0,1,H,Rect_black);
  }
  Blit(0.47-0.028*ScoreDigit(),0,0,0,score);
  //打印时间长度
  if(Time1!=-1)
    Blit(0,0,W-(double)(Time2-Time1)/T*W,5,Rect_yellow);
  scorel=GetScore(0);
}
//游戏开始界面
void Star()
{
  SDL_Texture *text;
  text=Text("开始",50,225,225,225);
  Blit(coor[3].x+0.025,0.58,0,0,text);
  Blit(0,0.75+(double)1/W,W,H/4-1,Rect_yellow);
  for(double i=0.25;i<1;i+=0.25)
    Blit(i,0.75,1,H/4,Rect_black);
}
//闪烁
void Flicker(double x,double y,SDL_Texture *texture,int i)
{
    int w=0,h=0;
    //方块大小
    if(i==1)
    {
      w=W/4;
      h=H/4;
    }
    draw();
    Blit(x,y,w,h,texture);
    SDL_RenderPresent(Renderer);
    SDL_Delay(200);
    draw();
    SDL_RenderPresent(Renderer);
    SDL_Delay(200);
    draw();
    Blit(x,y,w,h,texture);
    SDL_RenderPresent(Renderer);
    SDL_Delay(500);
}
//打印结算菜单
void output_quit(int i)
{
  static SDL_Texture *text[5];
  char s[20];
  SDL_RenderClear(Renderer);
  if(GetScore(0)==score_best[i-1])
    SDL_RenderCopy(Renderer,Rect_red,NULL,NULL);
  else
    SDL_RenderCopy(Renderer,Rect_black,NULL,NULL);
  if(i==1)
    strcpy(s,"  禅模式");
  else
    strcpy(s,"街机模式");
  text[0]=Text(s,100,225,225,225);
  text[1]=Text("重来",60,225,225,225);
  text[2]=Text("返回",60,225,225,225);
  sprintf(s,"%d",GetScore(0));
  text[3]=Text(s,70,225,225,225);
  sprintf(s,"最佳:%d",score_best[i-1]);
  text[4]=Text(s,50,225,225,225);
  SDL_Texture *text1;
  Blit(0.083,0.1875,0,0,text[0]);
  Blit(0.104,0.75,0,0,text[1]);
  Blit(0.646,0.75,0,0,text[2]);
  Blit(0.48-0.04*ScoreDigit(),0.375,0,0,text[3]);
  Blit(0.3-0.03*ScoreDigit(),0.5,0,0,text[4]);
  for(int i=0;i<5;i++)
    SDL_DestroyTexture(text[i]);
}
//结算菜单(闪烁位置x,y,菜单类型)
bool Quit(double x,double y,int type)
{
  void effect(int);
  SDL_Event event;
  //误触方块闪烁
  if(Time1==-1)
  {
    if(Volume)
    Mix_PlayChannelTimed(1,Sound[8],0,-1);
    Flicker(x,y,Rect_red,1);
  }
  //防止误触
  for(int i=0;i<50;i++)
    SDL_PollEvent(&event);
  while(1)
  {
    //打印结算菜单
    output_quit(type);
    SDL_RenderPresent(Renderer);
    if(SDL_PollEvent(&event))
    if(event.type==SDL_FINGERDOWN)
    {
      if(event.tfinger.y>0.625&&event.tfinger.y<0.875)
      {
        if(event.tfinger.x<0.5)
          return false;
        else
        {
          output_quit(type);
          effect(-1);
          return true;
        }
      }
    }
  }
}
//初始化方块坐标
void Init_coor()
{
  for(int i=0;i<4;i++)
  {
    chun_biaohao:
    coor[i].x=rand()%4*0.25;
    for(int j=0;j<i;j++)
      if(coor[i].x==coor[j].x)
        goto chun_biaohao;
    coor[i].y=(i-1)*0.25;
    coor[i].type=1;
  }
  coor[4].type=-1;
}
//检查是否按中黑方块
int check(double x,double y)
{
  int i,j;
  for(j=1;j>=0;j--)
  for(i=4;i>=0;i--)
  if(coor[i].type==j)
    if((x>=coor[i].x)&&(x<=coor[i].x+0.25)&&(y>=coor[i].y-0.25)&&(y<=coor[i].y+0.25))
    return i;
}
//加载菜单字体
void menu_load(SDL_Texture *text[])
{
  char s[20];
  text[0]=Text("禅模式",50,0,0,0);
  text[1]=Text("街机模式",50,225,225,225);
  if(Volume)
      strcpy(s,"声音:开");
    else
      strcpy(s,"声音:关");
  text[2]=Text(s,50,225,225,225);
  text[3]=Text("排行榜",50,0,0,0);
}
//打印菜单(字体表面,瓷砖位置,移动类型,移动部位)
void menu_Blit(SDL_Texture *text[],double i,int j,int type)
{
  int a=0,b=0;
  if(type==1) 
    a=1;
  if(type==2)
    b=1;
  //上半部分拉开
  //或者收回
  if(j==1&&type==1||j==-1||j==0)
  {
  Blit(-i*a,0,W/2,H/2,Rect_white);
    Blit(0.5+i*a,0,W/2+0.1*W,H/2,Rect_black);
  Blit(0.084-i*a,0.1875,0,0,text[0]);
    Blit(0.5412+i*a,0.1875,0,0,text[1]);
  }
  //拉开
  //或者下半部分收回
  if(j==1||j==-1&&type==2||j==0)
  {
   Blit(0.5+i*b,0.5,W/2,H/2,Rect_white);
    Blit(-i*b,0.5,W/2,H/2,Rect_black);
    Blit(0.084-i*b,0.6875,0,0,text[2]);
    Blit(0.604+i*b,0.6875,0,0,text[3]);
  }
}
//菜单移动效果
void effect(int n)
{
  double i;
  SDL_Texture *texture;
  if(n==1)
    i=0;
  else
    i=0.5;
  SDL_Surface *surface=ScreenShot();
  texture=SDL_CreateTextureFromSurface(Renderer,surface);
  SDL_FreeSurface(surface);
  SDL_Texture *text[4];
  menu_load(text);
  //j==1上半部分移动
  //j==2下半部分移动
  //i瓷砖位置
  for(int j=1;j<3;)
  {
    Blit(0,0,0,0,texture);
    menu_Blit(text,i,n,j);
    SDL_RenderPresent(Renderer);
    if(n==1)
      if(i<0.5)
        i+=0.025;
      else
      {
        i=0;
        j++;
      }
    else
      if(i>0)
        i-=0.025;
      else
      {
        i=0.5;
        j++;
      }
  }
  SDL_Event event;
  for(int i=0;i<20;i++)
    SDL_PollEvent(&event);
  for(int i=0;i<4;i++)
    SDL_DestroyTexture(text[i]);
}
//禅模式
void chan()
{
  SDL_Event event;
  FILE *fp;
  char s_fp[50];
  int aaaa=0;
  chan_star:
  //打开音乐文件
  if(Volume)
  {
    sprintf(s_fp,"music_%d.txt",rand()%5+1);
    fp=fopen(s_fp,"r");
  }
  //初始化
  GetScore(-1);
  Time2=Time1=1;
  Init_coor();
  draw();
  Star();
  if(aaaa==0)
  {
    effect(1);
    aaaa=1;
  }
  else
    SDL_RenderPresent(Renderer);
  goto chan_first;
  while(1)
  {
    //第一次
    if(0)
    {
      chan_first:
      while(1)
      {
        if(SDL_WaitEvent(&event))
        if(event.type==SDL_FINGERDOWN)
          break;
  }
      Time2=Time1=SDL_GetTicks()/1000;
      goto chan_first2;
    }
    while(1)
    {
      //开始计时
      Time2=SDL_GetTicks()/1000;
      //时间音效
      if(Volume&&T-(Time2-Time1)==9)
        Mix_PlayMusic(Timelose,0);
      if(Time2-Time1>=T)
      {
        Time1=0;
        SDL_Texture *text;
        text=Text("时间到",50,255,0,0);
        Flicker(0.35,0.06,text,2);
        SDL_DestroyTexture(text);
        goto chan_Quit;
      }
      if(SDL_PollEvent(&event))
      if(event.type==SDL_FINGERDOWN)
      {
        chan_first2:
        //y坐标范围判断
        if(!(event.tfinger.y>=0.25))
          continue;
        //触摸区域判断
        if(!((int)(event.tfinger.x/0.25)==(int)(coor[3].x/0.25)))
        {
          double x;
          Time1=-1;
          //停止时间音效
          Mix_HaltMusic();
          x=(int)(event.tfinger.x/0.25)*0.25;
          chan_Quit:
          if(Volume)
            fclose(fp);
          //获取最好成绩
          GetBest(1);
          //是否重来
          if(Quit(x,0.5,1)==true)
            return;
          else
            goto chan_star;
        }
        GetScore(1);
        coor[3].area=0.8;
        //触摸方块音效
        if(Volume)
        {
          int i;
          fscanf(fp,"%d",&i);
          if(i!=0)
          Mix_PlayChannelTimed(1,Sound[i-1],0,-1);
        }
        //打开新的音乐文件
        if(Volume&&feof(fp))
        {
          fclose(fp);
                    sprintf(s_fp,"music_%d.txt",rand()%5+1);
          fp=fopen(s_fp,"r");
        }
        coor[3].type=0;
        //移动效果
        for(int i=0;i<5;i++)
        {
          for(int i=0;i<5;i++)
            coor[i].y+=0.25/5;
          draw();
          SDL_RenderPresent(Renderer);
        }
        //移动方块
        for(int i=3;i>=0;i--)
          coor[i+1]=coor[i];
        coor[0].x=rand()%4*0.25;
        coor[0].y=-0.25;
        break;
      }
      draw();
      SDL_RenderPresent(Renderer);
    }
  }
}
//街机模式
void jieji()
{
  SDL_Event event;
  Time1=-1;
  FILE *fp;
  char s_fp[50];
  int aaaa=0;
  jieji_star:
  if(Volume)
  {
    sprintf(s_fp,"music_%d.txt",rand(
  )%5+1);
    fp=fopen(s_fp,"r");
  }
  Init_coor();
  GetScore(-1);
  long long i,Timel=0,time1=0,time2;
  double x,y,t=0;
  bool aa=false;
  if(aaaa==0)
  {
    draw();
    Star();
    aaaa=1;
    effect(1);
  }
  else
    SDL_RenderPresent(Renderer);
  while(1)
  {  
    //方块移动
    for(int i=0;i<5;i++)
      coor[i].y+=t;
    time1=SDL_GetTicks();
    //画出方块
    draw();
    if(!t)
      Star();
    //数组重新分配
    if(coor[0].y+t>=0)
    {
      double y=coor[0].y;
      for(int i=3;i>=0;i--)
        coor[i+1]=coor[i];
      coor[0].x=rand()%4*0.25;
      coor[0].y=-1*0.25+y;
      coor[0].type=1;
    }
    time2=SDL_GetTicks();
    //帧数限制
    if(time2-time1<1000/70)
      SDL_Delay(1000/70-(time2-time1));
    SDL_RenderPresent(Renderer);
    //判断黑方块是否到底
     if(coor[4].type==1&&coor[4].y>=0.85)
    {
      x=coor[4].x;
      y=coor[4].y;
      jieji_Quit:
      if(Volume)
        fclose(fp);
      GetBest(2);
      if(Quit(x,y,2)==false)
        goto jieji_star;
      else
        return;
    }
    //触摸检测
    if(SDL_PollEvent(&event)||aa)
    if(event.type==SDL_FINGERDOWN)
    {
      if(aa)
        aa=false;
      //如果按到黑方块
    if((i=check(event.tfinger.x,event.tfinger.y))!=-1)
    {
      if(coor[i].type==1)
      {
        if(!t)
          t=0.015;
        if(Volume)
        {
          int i;
          if(feof(fp))
          {
            fclose(fp);
                      sprintf(s_fp,"music_%d.txt",rand()%5+1);
            fp=fopen(s_fp,"r");
          }
          fscanf(fp,"%d",&i);
          if(i!=0)
          Mix_PlayChannelTimed(1,Sound[i-1],0,-1);
        }
        coor[i].type=0;
        coor[i].area=0.8;
        GetScore(1);
      }
    }
  else
  {
    //获取白方块位置
    x=(int)(event.tfinger.x/0.25)*0.25;
    y=coor[1].y+(int)((event.tfinger.y-coor[1].y)/0.25)*0.25;
    goto jieji_Quit;
  }
    }
    //移动速度增加
   if(t&&time1-Timel>=500&&t<0.026)
   {
    t+=0.0001;
    Timel=time1;
   }
  }
}
//输入昵称
char *getname()
{
  FILE *fp;
  static char name[25]="\0";
  if(fp=fopen("name.dat","r"))
  {
    int i;
    memset(name,0,sizeof(name));
    for(i=0;i<24&&!feof(fp);i++)
      name[i]=fgetc(fp);
    name[i-1]=0;
    fclose(fp);
    return name;
  }
  SDL_RenderClear(Renderer);
  SDL_Texture *text;
text=Text("输入你的昵称:",50,0,0,0);
  Blit(0.104,0.125,0,0,text);
  SDL_DestroyTexture(text);
  SDL_RenderPresent(Renderer);
  SDL_Event event;
  SDL_StartTextInput();
  while(1)
  {
    SDL_RenderClear(Renderer);
    text=Text("输入你的昵称:",50,0,0,0);
    Blit(0.104,0.125,0,0,text);
    SDL_DestroyTexture(text);
    SDL_WaitEvent(&event);
    if(event.type==SDL_FINGERDOWN)
      SDL_StartTextInput();
    else
    if(event.type==SDL_KEYDOWN)
    {
      //回车键退出
      if(event.key.keysym.sym==13)
        if(strcmp(name,"无")==0||strlen(name)==0)
          continue;
        else
          break;
      //删除键
      if(event.key.keysym.sym==8)
        if(name[0])
          if(name[strlen(name)-3]>225)
            name[strlen(name)-3]=0;
          else
            name[strlen(name)-1]=0;
    }
    else
    if(event.type==SDL_TEXTINPUT)
      if(strlen(name)+strlen(event.text.text)<=24&&strcmp(event.text.text," ")!=0)
        strcat(name,event.text.text);
      else
        continue;
    if(name[0])
    {
      char s[40];
      char s_[10];
      int size;
      if(strlen(name)<11)
        size=50;
      else
        size=30;
      sprintf(s_,"(%d字)",24-strlen(name));
      strcpy(s,name);
      strcat(s,s_);
      text=Text(s,size,0,0,0);
      Blit(0.041,0.25,0,0,text);
      SDL_DestroyTexture(text);
    }
    SDL_RenderPresent(Renderer);
  }
  SDL_StopTextInput();
  fp=fopen("name.dat","w");
  fprintf(fp,"%s",name);
  fclose(fp);
  effect(-1);
  return name;
}
//连接邮箱
int link(char *name,int port)
{
  int sockfd;
  struct sockaddr_in s_addr;
  struct hostent *host;
  char buff[500];
  if(!(host=gethostbyname(name)))
    return 0;
  s_addr.sin_family=host->h_addrtype;
  s_addr.sin_port=htons(port);
  s_addr.sin_addr=*((struct in_addr *)host->h_addr);
  memset(&(s_addr.sin_zero),0,8);
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    return 0;
  if(connect(sockfd,(struct sockaddr *)&s_addr,sizeof(struct sockaddr))==-1)
    return 0;
  memset(buff,0,sizeof(buff));
  recv(sockfd,buff,sizeof(buff),0);
  return sockfd;
}
//字体格式打印
void BlitText(const char *s,int size)
{
  SDL_Texture *text;
  char s_[100];
  int j=0;
  double y=0;
  SDL_RenderClear(Renderer);
  while(s[j])
  {
    int i;
    for(i=0;s[j]!='\n'&&s[j];i++)
      s_[i]=s[j++];
    s_[i]=0;
    j++;
    SDL_Rect rect;
    text=Text(s_,size,0,0,0);
    Blit(0,y,0,0,text);
    SDL_DestroyTexture(text);
    y+=0.002*size;
  }
  SDL_RenderPresent(Renderer);
  SDL_Delay(2000);
}
//下载排行榜
int download(Data *data)
{
  int sockfd;
  fd_set fd;
  SDL_Texture *text;
  struct timeval waittime={5,0};
  if(!(sockfd=link("pop.163.com",110)))
    return 0;
  char buff[2000];
  char *p;
  unsigned long long n;
  for(int i=0;i<4;i++)
  {
    switch(i)
    {
      case 0:p="USER xxxxxx@163.com\r\n";break;
      case 1:p="PASS xxxxxx\r\n";break;
      case 2:p="STAT\r\n";break;
      case 3:strcpy(buff,"RETR ");
                   char s[5];
                   sprintf(s,"%d\r\n",n);
                   strcat(buff,s);
                   p=buff;
                   break;
    }
    FD_ZERO(&fd);
    FD_SET(sockfd,&fd);
    send(sockfd,p,strlen(p),0);
    memset(buff,0,sizeof(buff));
    if(!select(sockfd+1,&fd,NULL,NULL,&waittime))
      return -1;
    recv(sockfd,buff,sizeof(buff),0);
    if(i==2)
      sscanf(buff,"+OK %d",&n);
  }
while(strstr(buff,"\r\nFrom")==NULL)
  {
    memset(buff,0,sizeof(buff));
    recv(sockfd,buff,sizeof(buff),0);
  }
  p=strstr(buff,"\r\nFrom");
  for(p+=2;*p!='\n';p++);
  for(p+=2;*p!='\n';p++);
  p+=3;
  if(!strstr(p,"."))
  {
    char buff_[1000];
    memset(buff_,0,sizeof(buff_));
    recv(sockfd,buff_,sizeof(buff_),0);
    strlcat(p,buff_,sizeof(buff)-strlen(buff));
  }
  for(int i=0;i<2;i++)
    for(int j=0;j<20;j++)
    {
      sscanf(p,"%d %s",&data[i].score[j],data[i].name[j]);
      for(;*p!='\n';p++);
      p++;
    }
  close(sockfd);
  return 1;
}
//检查排行榜
int check_score(Data *data,char *name)
{
  int n=0;
  for(int i=0,j;i<2;i++)
  {
    if(score_best[i]>data[i].score[19])
    {
      for(j=0;j<20;j++)
        if(!strcmp(data[i].name[j],name))
          if(data[i].score[j]<score_best[i])
          {
            data[i].score[j]=score_best[i];
            break;
          }
        else
          goto biaohao;
      if(j==20)
      {
        strcpy(data[i].name[19],name);
        data[i].score[19]=score_best[i];
      }
      n=1;
    }
    biaohao:;
  }
  for(int i=0;n&&i<2;i++)
    for(int j=0,p;j<19;j++)
    {
      p=j;
      for(int k=j+1,q=data[i].score[j];k<20;k++)
        if(data[i].score[k]>q)
        {
          q=data[i].score[k];
          p=k;
        }
      if(p!=j)
      {
        int t=data[i].score[j];
        data[i].score[j]=data[i].score[p];
        data[i].score[p]=t;
        char s[25];
        strcpy(s,data[i].name[j]);
        strcpy(data[i].name[j],data[i].name[p]);
        strcpy(data[i].name[p],s);
      }
    }
  return n;
}
//上传新排行榜
int upload(Data *data)
{
  int sockfd;
  if(!(sockfd=link("smtp.163.com",25)))
    return 0;
  char buff[2000];
  char *p;
  //登录邮箱
  for(int i=0;i<4;i++)
  {
    switch(i)
    {
      case 0:p="EHLO server\r\n";break;
      case 1:p="AUTH LOGIN\r\n";break;
      case 2:p="xxxxxx\r\n";break;
      case 3:p="xxxxxx\r\n";break;
    }
    send(sockfd,p,strlen(p),0);
    memset(buff,0,sizeof(buff));
    recv(sockfd,buff,sizeof(buff),0);
  }
  //发送邮件
  for(int i=0;i<5;i++)
  {
    switch(i)
    {
      case 0:p="MAIL FROM: <xxxxxx@163.com>\r\n";break;
      case 1:p="RCPT TO: <xxxxxx@163.com>\r\n";break;
      case 2:p="DATA\r\n";break;
      case 3:strcpy(buff,"SUBJECT:111\r\n\r\n");
             for(int i=0;i<2;i++)
               for(int j=0;j<20;j++)
               {
                 char s[50];
                 sprintf(s,"%d %s\n",data[i].score[j],data[i].name[j]);
                 strcat(buff,s);
               }
             strcat(buff,"\n\n\r\n.\r\n");
             p=buff;
             break;
      case 4:p="QUIT\r\n";break;
    }
    send(sockfd,p,strlen(p),0);
    memset(buff,0,sizeof(buff));
    recv(sockfd,buff,sizeof(buff),0);
  }
  close(sockfd);
  return 1;
}
//显示排行榜
void display(char *name)
{
  SDL_Texture *text;
  SDL_Event event;
  Data data[2];
  text=Text("上传中...",60,0,0,0);
  SDL_RenderClear(Renderer);
  Blit(0.25,0.375,0,0,text);
  SDL_DestroyTexture(text);
  SDL_RenderPresent(Renderer);
  biaohao:
  //下载排行榜
  switch(download(data))
  {
  	  case 0:text=Text("网络错误,请重试！",60,0,0,0);
    SDL_RenderClear(Renderer);
    Blit(0,0.375,0,0,text);
    SDL_DestroyTexture(text);
    SDL_RenderPresent(Renderer);
    do
      SDL_WaitEvent(&event);
    while(event.type!=SDL_FINGERUP);
    return;
    case -1:goto biaohao;
  }
  //检查能否上榜
  if(check_score(data,name))
  {
    //上传新排行榜
    if(!upload(data))
    {
      text=Text("上传错误,请重试！",60,0,0,0);
      SDL_RenderClear(Renderer);
      Blit(0,0.375,0,0,text);
      SDL_DestroyTexture(text);
      SDL_RenderPresent(Renderer);
      do
        SDL_WaitEvent(&event);
      while(event.type!=SDL_FINGERUP);
      return;
    }
  }
  for(int i=0;i<50;i++)
    SDL_PollEvent(&event);
  int n=0;
  while(1)
  {
    Blit(n*0.5,0,W/2,100*H/800,Rect_gray);
    Blit((1-n)*0.5,0,W/2,100*H/800,Rect_black);
    text=Text("禅模式",40,225,225,225);
    Blit(0.104,0.025,0,0,text);
    SDL_DestroyTexture(text);
    text=Text("街机模式",40,225,225,225);
    Blit(0.6,0.025,0,0,text);
    SDL_DestroyTexture(text);
    text=Text("返      回",50,0,0,0);
    Blit(0.3125,0.875,0,0,text);
    SDL_DestroyTexture(text);
    for(int i=0;i<20;i++)
    {
      char s[50];
      if(i+1<10)
        sprintf(s,"No.%d    %d  %s",i+1,data[n].score[i],data[n].name[i]);
      else
        sprintf(s,"No.%d  %d  %s",i+1,data[n].score[i],data[n].name[i]);
      text=Text(s,30,0,0,0);
      Blit(0.02,0.125+0.0375*i,0,0,text);
      SDL_DestroyTexture(text);
    }
    SDL_RenderPresent(Renderer);
    SDL_WaitEvent(&event);
    if(event.type!=SDL_FINGERUP)
      continue;
    if(event.tfinger.y<0.125)
      if(event.tfinger.x<0.5)
        n=0;
      else
        n=1;
    else
      if(event.tfinger.y>0.875)
        return;
  }
}
//排行榜
void charts()
{
  FILE *fp;
  char *name;
  name=getname();
  SDL_Surface *surface;
  //显示成绩
  {
    int size;
    if(strlen(name)<11)
      size=50;
    else
      size=30;
    char s[50];
    SDL_RenderClear(Renderer);
    SDL_Texture *text=Text(name,size,0,0,0);
    Blit(0.028,0.125,0,0,text);
    SDL_DestroyTexture(text);
    GetScore(-1);
    GetBest(1);
    sprintf(s,"禅模式:%d",score_best[0]);
    text=Text(s,50,0,0,0);
    Blit(0.1,0.25,0,0,text);
    SDL_DestroyTexture(text);
    sprintf(s,"街机模式:%d",score_best[1]);
    text=Text(s,50,0,0,0);
    Blit(0.1,0.375,0,0,text);
    SDL_DestroyTexture(text);
    strcpy(s,"上传成绩");
    text=Text(s,50,0,0,0);
    Blit(0.028,0.75,0,0,text);
    SDL_DestroyTexture(text);
    strcpy(s,"返      回");
    text=Text(s,50,0,0,0);
    Blit(0.625,0.75,0,0,text);
    SDL_DestroyTexture(text);
    
    surface=ScreenShot();
    effect(1);
  }
  SDL_Event event;
  while(1)
  {
    SDL_WaitEvent(&event);
    if(event.type==SDL_FINGERUP)
      if(event.tfinger.y>0.7&&event.tfinger.y<0.85)
      {
        if(event.tfinger.x<0.625)
          display(name);
        SDL_Texture *texture=SDL_CreateTextureFromSurface(Renderer,surface);
        SDL_RenderClear(Renderer);
        SDL_RenderCopy(Renderer,texture,NULL,NULL);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        effect(-1);
        return;
      }
  }
}

/*void Exit()
{
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(Win); 
  SDL_DestroyTexture(Rect_white);
  SDL_DestroyTexture(Rect_black);
  SDL_DestroyTexture(Rect_red);
  SDL_DestroyTexture(Rect_yellow);
  SDL_DestroyTexture(Rect_gray);
  TTF_CloseFont(Library);
  for(int i=0;i<8;i++)
    Mix_FreeChunk(Sound[i]);
  Mix_FreeMusic(Timelose);
  Mix_CloseAudio();
  SDL_Quit();
}*/

void menu()
{
  SDL_Texture *text[4];
  SDL_Event event;
  FILE *fp;
  //获取声音状态
  if((fp=fopen("volume.dat","rb+"))==NULL)
  {
    fp=fopen("volume.dat","wb+");
    fwrite(&Volume,sizeof(bool),1,fp);
    fflush(fp);
  }
  else
      fread(&Volume,sizeof(bool),1,fp);
  while(1)
  {
    //加载字体表面
    menu_load(text);
    //打印菜单
    menu_Blit(text,0,0,0);
    //释放字体表面
    for(int i=0;i<4;i++)
      SDL_DestroyTexture(text[i]);
    SDL_RenderPresent(Renderer);
    while(1)
    {
      //等待事件
      SDL_WaitEvent(&event);
      //事件筛选
      if(event.type!=SDL_FINGERUP)
        continue;
      //判断触摸位置
      double x,y;
      x=event.tfinger.x;
      y=event.tfinger.y;
      if(x<0.5&&y<0.5)
        chan();
      if(x>0.5&&y<0.5)
        jieji();
      if(x<0.5&&y>0.5)
      {
        Volume=1-Volume;
        rewind(fp);
        fwrite(&Volume,sizeof(bool),1,fp);
        fflush(fp);
        break;
      }
      if(x>0.5&&y>0.5)
      {
        charts();
        break;
      }
    }
  }
}

int main()
{
  srand(time(NULL));
  Init();
  {
    SDL_Texture *text;
    for(int i=0,j=0;i>=0;j==0?i+=15:i-=15)
    {
    text=Text("别踩白块儿V2.1",60,225,225,225);
    SDL_SetTextureAlphaMod(text,i);
    Blit(0.07,0.4,0,0,text);
    SDL_DestroyTexture(text);
     text=Text("百度贴吧 C4droid吧",20,225,225,225);
     SDL_SetTextureAlphaMod(text,i);
    Blit(0.6,0.9,0,0,text);
    SDL_DestroyTexture(text);
     text=Text("by 风飞飘扬是本人",20,225,225,225);
     SDL_SetTextureAlphaMod(text,i);
    Blit(0.6,0.95,0,0,text);
    SDL_DestroyTexture(text);
    SDL_RenderPresent(Renderer);
    SDL_Delay(10);
    if(i>240)
      j=1;
    }
    SDL_Event event;
    for(int i=0;i<50;i++)
      SDL_PollEvent(&event);
  }
  menu();
  //Exit();
  return 0;
}