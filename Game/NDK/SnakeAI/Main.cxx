// 半成品，来源：c4droid吧某大神
// 运行报错，请修改nativeActivity参数，添加 -lGLESv3(注意参数前后空格)

#include <jni.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES3/gl31.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define Pi 3.14159265358979323846
#define nn 40
/*notes
 *得到旋转矢量与旋转矩阵操作约800ns
 *double乘法比赋值快
 */
/*-------------------------log_speed--------------------------------*/
int mode=0;//0:AI;1:Human
FILE *fp;//建立一个文件操作指针
struct timespec st,ft;
clock_t gt0,gt1,gt2;
const int delay_time=100000;
int num=0;
int found=1;
/*-------------------------shader_string----------------------------*/
const char *vShaderstr_map=
"#version 310 es                                                 \n"
"#define nn 40                                                   \n"
"layout(location=0) in vec4 a_position;                          \n"
"layout(location=1) in vec4 a_color;                             \n"
"flat out vec4 v_color;                                          \n"
"void main()                                                     \n"
"{                                                               \n"
"  v_color=a_color;                                              \n"
"  float num1=float(gl_InstanceID%nn)/float(nn);                 \n"
"  float num2=float(gl_InstanceID-gl_InstanceID%nn)/float(nn*nn);\n"
"  gl_Position=a_position+vec4(num1*2.0f,num2*1.125f,0.0f,0.0f); \n"
"}                                                               \n";
const char *fShaderstr_map=
"#version 310 es                      \n"
"precision highp float;               \n"
"flat in vec4 v_color;                \n"
"out vec4 o_fragColor;                \n"
"void main()                          \n"
"{                                    \n"
"  o_fragColor=v_color;               \n"
"}                                    \n";


const char *vShaderstr_button=
"#version 310 es                          \n"
"#define nn 40                            \n"
"layout(location=2) in vec4 a_position;   \n"
"flat out vec4 v_color;                   \n"
"void main()                              \n"
"{                                        \n"
"  v_color=vec4(1.0f,1.0f,0.0f,1.0f);     \n"
"  gl_Position=a_position;                \n"
"}                                        \n";
const char *fShaderstr_button=
"#version 310 es                      \n"
"precision highp float;               \n"
"flat in vec4 v_color;                \n"
"out vec4 o_fragColor;                \n"
"void main()                          \n"
"{                                    \n"
"  o_fragColor=v_color;               \n"
"}                                    \n";


/*******************************vars*********************************/
/*-------------------------snake_data-------------------------------*/
struct int_point
{
	int x;
	int y;
};
struct int_point food_p={0,0};
struct int_point head_p={0,0};
struct int_point body[nn*nn];
struct int_point dir={1,0};
struct int_point path[nn*nn];
int path_len;

//unsigned int head=0;
int nt=0;
int body_len=5;
bool touched=0,ate=0;
struct finder_data
{
	struct int_point point_p;
	int f;
	int g;
	int h;
};
struct finder_data opened[nn*nn];
struct finder_data closed[nn*nn];
struct finder_data finder_temp;
int opened_len=0;
int closed_len=0;
/*-------------------------map--------------------------------------*/
unsigned char map[nn][nn]={0};
/*
  0:null;
  1:block;
  2:body;
  3:food;
*/
/*-------------------------graphic_draw-----------------------------*/
GLuint shader_map,shader_button;
//map
GLuint vboid0,vboid1;
struct a_point
{
	GLfloat x;
	GLfloat y;
};
struct a_color
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
};
struct a_block
{
	struct a_point p1;
	struct a_point p2;
	struct a_point p3;
	struct a_point p4;
};
struct a_block block;
struct a_color color_food={1.0f,0.0f,0.0f};//4
struct a_color color_head={0.0f,0.0f,1.0f};//2
struct a_color color_body={0.0f,1.0f,0.0f};//3
struct a_color color_block={1.0f,1.0f,0.0f};//1
struct a_color color_null={0.1f,0.1f,0.1f};//0
struct a_color color_test={1.0f,1.0f,1.0f};//5
struct a_color color_map[nn*nn];
GLfloat button_vertex[48]=
{
	0.5833333f,-0.453125f,
	0.5833333f,-0.671875f,
	0.1944444f,-0.671875f,
	
	0.5833333f,-0.453125f,
	0.1944444f,-0.453125f,
	0.1944444f,-0.671875f,
	
	
	-0.5833333f,-0.453125f,
	-0.5833333f,-0.671875f,
	-0.1944444f,-0.671875f,
	
	-0.5833333f,-0.453125f,
	-0.1944444f,-0.453125f,
	-0.1944444f,-0.671875f,
	
	
	0.1944444f,-0.234375f,
	-0.1944444f,-0.234375f,
	-0.1944444f,-0.453125f,
	
	0.1944444f,-0.234375f,
	0.1944444f,-0.453125f,
	-0.1944444f,-0.453125f,
	
	
	0.1944444f,-0.890625f,
	-0.1944444f,-0.890625f,
	-0.1944444f,-0.671875f,
	
	0.1944444f,-0.890625f,
	0.1944444f,-0.671875f,
	-0.1944444f,-0.671875f,
};
/**************************functions*********************************/
void AI_cycle();
void Human_cycle();
void setmap(int x,int y,unsigned char type);
void init();
void judge();
void food();
void finder();
void min_heap_delete(int num);
void min_heap_swap(int num);
int search(int x,int y);

/*-------------------------system_data------------------------------*/
/*-------------------------saved_state------------------------------*/
/*Our saved state data.*/
struct saved_state
{
	float angle;
	int32_t x;
	int32_t y;
};
/*-------------------------engine-----------------------------------*/
/*Shared state for our app.*/
struct engine
{
	struct android_app *app;
	ASensorManager *sensorManager;
	const ASensor *accelerometerSensor;
	ASensorEventQueue *sensorEventQueue;
	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
};
struct engine engine;
/*************************system_functions***************************/
/********************************************************************/
void gluPerspectivef(GLfloat fovy,GLfloat aspect,GLfloat zNear, GLfloat zFar);
static int engine_init_display(struct engine *engine);
static int32_t engine_handle_input(struct android_app *app,AInputEvent * event);
static void __gluMakeIdentityf(GLfloat m[16]);
static void engine_draw_frame(struct engine *engine);
static void engine_term_display(struct engine *engine);
static void engine_handle_cmd(struct android_app *app, int32_t cmd);
/*-----****------------****------main------****----------****-------*/
/*-----****------------****------main------****----------****-------*/
void android_main(struct android_app *state)
{
	srand((int)time(NULL));
	app_dummy();
	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app=state;
	//Prepare to monitor accelerometer
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor =
	ASensorManager_getDefaultSensor
	(engine.sensorManager,ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue =
	ASensorManager_createEventQueue
	(engine.sensorManager, state->looper,LOOPER_ID_USER, NULL, NULL);
	if (state->savedState != NULL)
	{
		engine.state = *(struct saved_state *)state->savedState;
	}
	gt0=clock();
	while (1)
	{
		int ident;
		int events;
		struct android_poll_source *source;
		while
		((ident=ALooper_pollAll(engine.animating?0:-1,NULL,&events,(void **)&source))>=0)
		{
			if (source != NULL)
			source->process(state, source);
			if (ident == LOOPER_ID_USER)
				if (engine.accelerometerSensor != NULL)
				{
					ASensorEvent event;
					while
					(
					ASensorEventQueue_getEvents
					(engine.sensorEventQueue,&event,1)>0
					)
					{
						LOGI
						(
						"accelerometer: x=%f y=%f z=%f",
						event.acceleration.x,
						event.acceleration.y,
						event.acceleration.z
						);
					}
				}
			if (state->destroyRequested!=0)
			{
				engine_term_display(&engine);
				return;
			}
		}
		if (mode==0)
		AI_cycle();
		else
		Human_cycle();
		if (engine.animating)
		{
			engine.state.angle += .1f;
		}
	}
}
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/

/*************************functions**********************************/
void AI_cycle()
{
	if((clock()>gt0+1670)&&found)
	{
		path_len--;
		dir=path[path_len];
		judge();
	gt0=clock();
	}
	engine_draw_frame(&engine);
}
void Human_cycle()
{
	if(clock()>gt0+delay_time)
	{
		judge();
		touched=0;
		engine_draw_frame(&engine);
		gt0=clock();
	}
}
/*************************game_functions*****************************/
void finder()
{
	struct state_f
	{
		int state;
		int step;
		struct int_point dir;
	};
	struct finder_data swap;
	struct state_f state[nn][nn]={0,0,{0,0}};
	opened[0]=
	{
		body[nt],
		abs(body[nt].x-food_p.x)+abs(body[nt].y-food_p.y),
		0,
		abs(body[nt].x-food_p.x)+abs(body[nt].y-food_p.y)
	};
	opened_len=1;
	while(
	(finder_temp.point_p.x-food_p.x)||
	(finder_temp.point_p.y-food_p.y))
	{
		for(int c1=0;c1<opened_len;c1++)
		////fprintf(fp,"opened:%d %d\n",c1+1,opened[c1].f);
		
		//fprintf(fp,"\n");
		finder_temp=opened[0];
		opened[0]=opened[--opened_len];
//		opened[opened_len]=finder_temp;
		min_heap_delete(0);
		int xt=finder_temp.point_p.x;
		int yt=finder_temp.point_p.y;
//		setmap(xt,yt,5);
//		setmap(food_p.x,food_p.y,4);
//		for(int b1=0;b1<60;b1++)
//		engine_draw_frame(&engine);
		
//		setmap(xt,yt,6);
//		setmap(food_p.x,food_p.y,4);
		state[xt][yt].state=2;
		int f,g,h;
		struct state_f state_p;
		//----------- 向右 ----------
		if ((++xt) != nn)
		{
			if (((state_p = state[xt][yt]).state != 2) && !(map[xt][yt] % 3))
				if (state_p.state)
				{
					if ((g = (finder_temp.g + 1))<state_p.step)
					{
						state[xt][yt] = { 1,g,{ 1,0 } };
						int tt = search(xt, yt);
						opened[tt].f -= (opened[tt].g - g);
						opened[tt].g = g;
						min_heap_swap(tt);
					}
				}
				else
				{
					state[xt][yt] = { 1,finder_temp.g + 1,{ 1,0 } };
					h = abs(xt - food_p.x) + abs(yt - food_p.y);
					opened[opened_len] = { { xt,yt },h + finder_temp.g + 1,finder_temp.g + 1,h };
					min_heap_swap(opened_len);
					opened_len++;
				}
		}
		//----------- 向左 ----------
		if ((xt -= 1))
		{
			xt--;
			if (((state_p = state[xt][yt]).state != 2) && !(map[xt][yt] % 3))
				if (state_p.state == 1)
				{
					if ((g = (finder_temp.g + 1))<state_p.step)
					{
						state[xt][yt] = { 1,g,{ -1,0 } };
						int tt = search(xt, yt);
						opened[tt].f -= (opened[tt].g - g);
						opened[tt].g = g;
						min_heap_swap(tt);
					}
				}
				else
				{
					state[xt][yt] = { 1,finder_temp.g + 1,{ -1,0 } };
					h = abs(xt - food_p.x) + abs(yt - food_p.y);
					opened[opened_len] = { { xt,yt },h + finder_temp.g + 1,finder_temp.g + 1,h };
					min_heap_swap(opened_len);
					opened_len++;
				}
			xt++;
		}
		//----------- 向上 ----------
		if (++yt != nn)
		{
			if (((state_p = state[xt][yt]).state != 2) && !(map[xt][yt] % 3))
				if (state_p.state == 1)
				{
					if ((g = (finder_temp.g + 1))<state_p.step)
					{
						state[xt][yt] = { 1,g,{ 0,1 } };
						int tt = search(xt, yt);
						opened[tt].f -= (opened[tt].g - g);
						opened[tt].g = g;
						min_heap_swap(tt);
					}
				}
				else
				{
					state[xt][yt] = { 1,finder_temp.g + 1,{ 0,1 } };
					h = abs(xt - food_p.x) + abs(yt - food_p.y);
					opened[opened_len] = { { xt,yt },h + finder_temp.g + 1,finder_temp.g + 1,h };
					min_heap_swap(opened_len);
					opened_len++;
				}
		}
		//----------- 向下 ----------
		if ((yt -= 1))
		{
			yt--;
			if (((state_p = state[xt][yt]).state != 2) && !(map[xt][yt] % 3))
				if (state_p.state == 1)
				{
					if ((g = (finder_temp.g + 1))<state_p.step)
					{
						state[xt][yt] = { 1,g,{ 0,-1 } };
						int tt = search(xt, yt);
						opened[tt].f -= (opened[tt].g - g);
						opened[tt].g = g;
						min_heap_swap(tt);
					}
				}
				else
				{
					state[xt][yt] = { 1,finder_temp.g + 1,{ 0,-1 } };
					h = abs(xt - food_p.x) + abs(yt - food_p.y);
					opened[opened_len] = { { xt,yt },h + finder_temp.g + 1,finder_temp.g + 1,h };
					min_heap_swap(opened_len);
					opened_len++;
				}
		}
		if (opened_len == 0)
		{
			found = 0;
			break;
		}
	}
	if(found)
	{
		int xt=body[nt].x;
		int yt=body[nt].y;
		int x=food_p.x;
		int y=food_p.y;
		path_len=0;
	//fprintf(fp,"food:(%d,%d)\n",x,y);
		while((x-xt)||(y-yt))
		{
			path[path_len]=state[x][y].dir;
			x-=path[path_len].x;
			y-=path[path_len].y;
			//fprintf(fp,"{%d,%d},{%d,%d}\n",			x,y,path[path_len].x,path[path_len].y);
			path_len++;
		}
	//fprintf(fp,"head:(%d,%d)\n",body[nt].x,body[nt].y);
	}
}
int search(int x,int y)
{
	int c1=0;
	while(1)
	{
		if((opened[c1].point_p.x==x)&&(opened[c1].point_p.y==y))
		return c1;
		c1++;
	}
}

void min_heap_delete(int num)
{
	int min;
	if ((2 * num + 1<opened_len) && (opened[num].f>opened[2 * num + 1].f))
		min = 2 * num + 1;
	else
		min = num;
	if ((2 * num + 2<opened_len) && (opened[min].f>opened[2 * num + 2].f))
		min = 2 * num + 2;
	if (min != num)
	{
		struct finder_data temp = opened[min];
		opened[min] = opened[num];
		opened[num] = temp;
		min_heap_delete(min);
	}
}
void min_heap_swap(int num)
{
	int op = num;
	while (op)
	{
		if (opened[op].f < opened[(op - 1) / 2].f)
		{
			struct finder_data temp = opened[op];
			opened[op] = opened[(op - 1) / 2];
			op = (op - 1) / 2;
			opened[op] = temp;
		}
		else
			break;
	}
}

void food(void)
{
	int x,y;
	while(map[x=(random()%nn)][y=(random()%nn)]);
	food_p={x,y};
	setmap(x,y,4);
}
void setmap(int x,int y,unsigned char type)
{
	switch (type)
	{
		case 0:
		{
			map[x][y]=0;
			glBindBuffer(GL_ARRAY_BUFFER,vboid1);
			void *buf=glMapBufferRange
			(GL_ARRAY_BUFFER,12*(x+nn*y),12,GL_MAP_WRITE_BIT);
			memcpy(buf,&color_null,12);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
		}
		case 1:
		{
			map[x][y]=1;
			glBindBuffer(GL_ARRAY_BUFFER,vboid1);
			void *buf=glMapBufferRange
			(GL_ARRAY_BUFFER,12*(x+nn*y),12,GL_MAP_WRITE_BIT);
			memcpy(buf,&color_block,12);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
		}
		case 2:
		{
			map[x][y]=2;
			glBindBuffer(GL_ARRAY_BUFFER,vboid1);
			void *buf=glMapBufferRange
			(GL_ARRAY_BUFFER,12*(x+nn*y),12,GL_MAP_WRITE_BIT);
			memcpy(buf,&color_head,12);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
		}
		case 3:
		{
			map[x][y]=2;
			glBindBuffer(GL_ARRAY_BUFFER,vboid1);
			void *buf=glMapBufferRange
			(GL_ARRAY_BUFFER,12*(x+nn*y),12,GL_MAP_WRITE_BIT);
			memcpy(buf,&color_body,12);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
		}
		case 4:
		{
			map[x][y]=3;
			glBindBuffer(GL_ARRAY_BUFFER,vboid1);
			void *buf=glMapBufferRange
			(GL_ARRAY_BUFFER,12*(x+nn*y),12,GL_MAP_WRITE_BIT);
			memcpy(buf,&color_food,12);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
		}
		case 5:
		{
			glBindBuffer(GL_ARRAY_BUFFER,vboid1);
			void *buf=glMapBufferRange
			(GL_ARRAY_BUFFER,12*(x+nn*y),12,GL_MAP_WRITE_BIT);
			memcpy(buf,&color_test,12);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
		}
		case 6:
		{
			glBindBuffer(GL_ARRAY_BUFFER,vboid1);
			void *buf=glMapBufferRange
			(GL_ARRAY_BUFFER,12*(x+nn*y),12,GL_MAP_WRITE_BIT);
			memcpy(buf,&color_null,12);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
		}
	}
}
void judge()
{
	int xn=(nn+body[nt].x+dir.x)%nn;
	int yn=(nn+body[nt].y+dir.y)%nn;
	switch (map[xn][yn])
	{
		case 0:
		{
			setmap(xn,yn,2);
			setmap(body[nt].x,body[nt].y,3);
			nt=(nt+1)%body_len;
			setmap(body[nt].x,body[nt].y,0);
			body[nt]={xn,yn};
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			if(body_len-nt)
			{
				setmap(xn,yn,2);
				setmap(body[nt].x,body[nt].y,3);
				struct a_point temp[body_len-nt];
				memcpy(temp,&body[nt+1],8*(body_len-nt));
				memcpy(&body[nt+2],temp,8*(body_len-nt));
			}
			body[nt+1]={xn,yn};
			nt++;
			body_len++;
			food();
			if(mode==0)
			finder();
			ate=1;
			//engine_draw_frame(&engine);
			break;
		}
	}
}
void init()
{
	nt=0;
	body_len=5;
	dir={1,0};
	food();
	body[0]={20,20};
	body[1]={16,20};
	body[2]={17,20};
	body[3]={18,20};
	body[4]={19,20};
	setmap(20,20,2);//head
	setmap(16,20,3);//body
	setmap(17,20,3);
	setmap(18,20,3);
	setmap(19,20,3);
	int x,y;
	for(int k1=0;k1<100;k1++)
	{
		while(map[x=(random()%nn)][y=(random()%nn)]);
		setmap(x,y,1);
	}
	if(mode==0)
	finder();
}

/*************************system_functions***************************/
/*---------------------engine_init_display--------------------------*/
static int engine_init_display(struct engine *engine)
{
	const EGLint attribs[] =
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_NONE
	};
	int attrib[4]={EGL_CONTEXT_CLIENT_VERSION,3,EGL_NONE,0};
	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;
	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
	surface=eglCreateWindowSurface(display,config,engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, attrib);
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}
	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle=0;
	
	fp=fopen("log.txt","w+");
	block=
	{
		.p1={-1.0f+2.0f/1080.0f,-0.125f+1.125f/1080.0f},
		.p2={-1.0f+2.0f/nn-2.0f/1080.0f,-0.125f+1.125f/1080.0f},
		.p3={-1.0f+2.0f/1080.0f,-0.125f+1.125f/nn-1.125f/1080.0f},
		.p4=
		{-1.0f+2.0f/nn-2.0f/1080.0f,-0.125f+1.125f/nn-1.125f/1080.0f}
	};
	
	GLuint vShader,fShader;
	GLint v_compiled,f_compiled,p_linked;
	//shader_map
	vShader=glCreateShader(GL_VERTEX_SHADER);
	fShader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader,1,&vShaderstr_map,NULL);
	glShaderSource(fShader,1,&fShaderstr_map,NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	glGetShaderiv(vShader,GL_COMPILE_STATUS,&v_compiled);
	glGetShaderiv(fShader,GL_COMPILE_STATUS,&f_compiled);
	shader_map=glCreateProgram();
	glAttachShader(shader_map,vShader);
	glAttachShader(shader_map,fShader);
	glLinkProgram(shader_map);
	glGetProgramiv(shader_map,GL_LINK_STATUS,&p_linked);
	//fprintf(fp,"map:\n");
	//fprintf(fp,"  v_compiled:%d\n",v_compiled);
	//fprintf(fp,"  f_compiled:%d\n",f_compiled);
	//fprintf(fp,"  p_linked:%d\n",p_linked);
	//shader_button
	vShader=glCreateShader(GL_VERTEX_SHADER);
	fShader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader,1,&vShaderstr_button,NULL);
	glShaderSource(fShader,1,&fShaderstr_button,NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	glGetShaderiv(vShader,GL_COMPILE_STATUS,&v_compiled);
	glGetShaderiv(fShader,GL_COMPILE_STATUS,&f_compiled);
	shader_button=glCreateProgram();
	glAttachShader(shader_button,vShader);
	glAttachShader(shader_button,fShader);
	glLinkProgram(shader_button);
	glGetProgramiv(shader_button,GL_LINK_STATUS,&p_linked);
	//fprintf(fp,"button:\n");
	//fprintf(fp,"  v_compiled:%d\n",v_compiled);
	//fprintf(fp,"  f_compiled:%d\n",f_compiled);
	//fprintf(fp,"  p_linked:%d\n",p_linked);
	//map_vertex
	clock_gettime(CLOCK_REALTIME,&st);
	glGenBuffers(1,&vboid0);
	glBindBuffer(GL_ARRAY_BUFFER,vboid0);
	glBufferData(GL_ARRAY_BUFFER,sizeof(block),&block,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);
	clock_gettime(CLOCK_REALTIME,&ft);
	//fprintf(fp,"create map_vertex VBO's time:%ldns\n",	1000000000*(ft.tv_sec-st.tv_sec)+(ft.tv_nsec-st.tv_nsec));
	//map_color
	for(int c1=0;c1<nn;c1++)
	for(int c2=0;c2<nn;c2++)
	{
		color_map[c2+nn*c1]=color_null;
		////fprintf(fp,"%d",map[c1][c2]);
	}
	clock_gettime(CLOCK_REALTIME,&st);
	glGenBuffers(1,&vboid1);
	glBindBuffer(GL_ARRAY_BUFFER,vboid1);
	glBufferData(GL_ARRAY_BUFFER,12*nn*nn,color_map,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1,1);
	clock_gettime(CLOCK_REALTIME,&ft);
	//fprintf(fp,"create button_vertex VBO's time:%ldns\n",	1000000000*(ft.tv_sec-st.tv_sec)+(ft.tv_nsec-st.tv_nsec));
	//button
	GLuint vboid2,vboid3;
	//button_vertex
	clock_gettime(CLOCK_REALTIME,&st);
	glGenBuffers(1,&vboid2);
	glBindBuffer(GL_ARRAY_BUFFER,vboid2);
	glBufferData
	(
	GL_ARRAY_BUFFER,
	sizeof(button_vertex),
	&button_vertex,
	GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,NULL);
	clock_gettime(CLOCK_REALTIME,&ft);
	//fprintf(fp,"create button_vertex VBO's time:%ldns\n",	1000000000*(ft.tv_sec-st.tv_sec)+(ft.tv_nsec-st.tv_nsec));
	
	glViewport(0,0,engine->width,engine->height);
	init();
	GLint test;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&test);
	//fprintf(fp,"GL_MAX_VERTEX_ATTRIBS:%d\n",test);
	return 0;
}
/*-----------------------engine_draw_frame--------------------------*/
static void engine_draw_frame(struct engine *engine)
{
	if (engine->display == NULL)
	return;
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_map);//12000ns
	glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,nn*nn);//440000ns
	glUseProgram(shader_button);
//	clock_gettime(CLOCK_REALTIME,&st);
	glDrawArrays(GL_TRIANGLES,0,24);//20000ns
//	clock_gettime(CLOCK_REALTIME,&ft);
	glFlush();
	eglSwapBuffers(engine->display,engine->surface);
//	//fprintf(fp,"draw's time:%ldns\n",
//	1000000000*(ft.tv_sec-st.tv_sec)+(ft.tv_nsec-st.tv_nsec));
}
/*------------------------engine_term_display-----------------------*/
static void engine_term_display(struct engine *engine)
{
	if (engine->display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent
		(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE,EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}
/*--------------------engine_handle_input---------------------------*/

static int32_t engine_handle_input(struct android_app *app,AInputEvent * event)
{
	struct engine *engine = (struct engine *)app->userData;
	float x,y;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		int32_t id = AMotionEvent_getAction(event);
		switch(id)
		{
			case AMOTION_EVENT_ACTION_DOWN:
			{
				if(!touched)
				{
					engine->animating = 1;
					x=float(AMotionEvent_getX(event,0)-1080.0f)/1080.0f;
					y=float(1700.0f-AMotionEvent_getY(event,0))/1400.0f;
					if
					(
					(x<0.5833333f)&&
					(x>0.1944444f)&&
					(y<-0.453125f)&&
					(y>-0.671875f)&&
					(dir.x!=-1))
					dir={1,0};
					if
					(
					(x>-0.5833333f)&&
					(x<-0.1944444f)&&
					(y<-0.453125f)&&
					(y>-0.671875f)&&
					(dir.x!=1))
					dir={-1,0};
					if
					(
					(x<0.1944444f)&&
					(x>-0.1944444f)&&
					(y>-0.453125f)&&
					(y<-0.234375f)&&
					(dir.y!=-1))
					dir={0,1};
					if
					(
					(x<0.1944444f)&&
					(x>-0.1944444f)&&
					(y>-0.890625f)&&
					(y<-0.671875f)&&
					(dir.y!=1))
					dir={0,-1};
					
					//fprintf(fp,"x:%f\ny:%f\n",x,y);
					return 1;
				}
			}
			case AMOTION_EVENT_ACTION_MOVE:
				break;
			case AMOTION_EVENT_ACTION_UP:
			engine->animating = 1;
				return 1;
				break;
		}
	}
	return 0;
}
/*-----------------------engine_handle_cmd--------------------------*/
static void engine_handle_cmd(struct android_app *app, int32_t cmd)
{
	struct engine *engine = (struct engine *)app->userData;
	switch (cmd)
	{
		case APP_CMD_SAVE_STATE:
		engine->app->savedState=malloc(sizeof(struct saved_state));
		*((struct saved_state *)engine->app->savedState)=
		engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;			
		/*----------APP_CMD_INIT_WINDOW-----------------------------*/
		case APP_CMD_INIT_WINDOW:
		if (engine->app->window != NULL)
		{
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;
		/*----------APP_CMD_TERM_WINDOW-----------------------------*/
		case APP_CMD_TERM_WINDOW:
		engine_term_display(engine);
		break;
		/*----------APP_CMD_GAINED_FOCUS----------------------------*/
		case APP_CMD_GAINED_FOCUS:
		if (engine->accelerometerSensor != NULL)
		{
			ASensorEventQueue_enableSensor
			(engine->sensorEventQueue,engine->accelerometerSensor);
			ASensorEventQueue_setEventRate
			(
			engine->sensorEventQueue,engine->accelerometerSensor,
			166000//16666.66666...
			);
		}
		break;
		/*----------APP_CMD_LOST_FOCUS------------------------------*/
		case APP_CMD_LOST_FOCUS:
		if (engine->accelerometerSensor != NULL)
		{
			ASensorEventQueue_disableSensor
			(engine->sensorEventQueue,engine->accelerometerSensor);
		}
		engine->animating = 0;
		engine_draw_frame(engine);
		break;
	}
}
