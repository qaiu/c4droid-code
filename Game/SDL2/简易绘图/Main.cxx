/*c4droid代码手册
简易绘图
**硫酸雨制作(百度帐号dd7262000)
**语法很渣,不要介意
获取触屏
**左列是工具栏,依次为:
**生成对象窗口,移动,旋转,缩放,
**x锁定,y锁定。
**右列分别是:
**特殊键,对象列表(五格)
**上,下是翻页,中间是这页的对象
**对象列表中的对象可以拖出来
**拖到特殊键上删除
**选择颜色栏时点击特殊键以取消所
**选对象
**从特殊键上开始拖移可以提高精度
**选择缩放工具,选择圆,从特殊键
**上拖移可以调整圆的分段
**上栏是颜色栏,为红,绿,蓝,整体
**选中后在主界面上下拖移调整
**空对象时调节背景色
**手指划过摁键时若可进行操作则摁
**键亮起
**左上角摁键可以对触屏事件进行提
**示
**点击下方长摁键进入全屏
**点同样位置退出全屏
**删除对象时特殊键为红色
**差不多就这样,双拼可真累(;一_一)
**建议横屏运行
———————————————
**恃''财''放旷:有钱就是任性。
*/
#include "SDL2/SDL.h"
#include<math.h>
#include<pthread.h>
#include<stdlib.h>
#define PI 3.1416f

float dx,dy;//移动距离
float tx,ty; //触碰点坐标
int w=0,h=0;//屏幕大小
int obnum=0;//对象总数
int tool=1;//当前工具栏
int theob=0;//当前选中对象号
int page=1,pages=1;//当前页;总页数
bool box=true,boy=true;
//x;y方向锁定记录
bool bo=false;//特殊键开关
bool boo=false;//操作开关
int screen_r=120,screen_g=120,
     screen_b=120;//背景色

typedef struct color//颜色
{
	float r;//红
	float g;//黄
	float b;//蓝
}obcl;

typedef struct size//大小
{
	float a;//长
	float b;//宽
}obsi;

typedef struct object//对象总属性
{
	unsigned int mark;//对象号
	unsigned int id;//对象ID(0矩形,1圆)
	float x;//横坐标
	float y;//纵坐标
	float h;//旋转角度
	obsi size;//大小
	obcl color;//颜色
	struct object *next;//链节
}node;
 
SDL_Window *win = NULL;//窗口
SDL_Renderer *render = NULL;
//渲染器

node *allob=NULL;
//所有对象链表表头
node *nowob=NULL;
//当前选中对象

#include "drawobs.c"
//画图
#include "doobs.c"
//对象操作(生成,删除等)
#include "drawwin.c"
//画小窗口
#include "gettouch.c"
//触屏获取及处理线程

int main(int argc, char** args)
{
	allob=createnode();
	//创建链表(doobs.c)

	SDL_Init
	(SDL_INIT_EVERYTHING);
	//SDL初始化
	win=SDL_CreateWindow
	("SDL2 DRAW", 0, 0, 0, 0, 1);
	//创建窗口
	render=SDL_CreateRenderer
	(win, -1, NULL); 
	//创建渲染器

	SDL_GetWindowSize (win,&w,&h);
	//获取屏幕大小

	pthread_t tid;//线程号
	pthread_create 
	(&tid,NULL,gettouch,NULL);
	//启动线程以获取触控

	while (1)
  {
  	SDL_GetWindowSize (win,&w,&h);
  	//获取屏幕大小以适应
  	drawob();//画图(drowobs.c)
  	if (tool==0)
   	 openwi1();//开小窗1(drawwin.c)
  	else if (tool==8)
   	 openwi2();
  	SDL_SetRenderDrawColor
  	(render, 0 ,0, 0, 255);
  	SDL_RenderClear(render);
  	//刷新屏幕
  }
}