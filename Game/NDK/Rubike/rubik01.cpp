/*
贴吧：【记录】零基础用c4droid写一个3D魔方软件
https://tieba.baidu.com/p/6492706827
整理：QAIU
*/
#include <math.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>

const ASensor *sensor;			/* 感应器 */
ASensorEventQueue *sensor_event_queue;	/* 感应器事件队列 */
struct android_app *android_app;	/* java扔给我们的系统状态 */
EGLSurface surface;				/* egl 显示平面 */
EGLDisplay display;				/* egl 显示设备 */
EGLContext context;				/* egl 显示上下文 */

GLfloat ax=1.0f,ay=0.0f;
int last_x,last_y,state_x,state_y;
GLfloat angle;
int animating;
int first=0;
int width,height;
GLfloat anglef;
GLfloat ex=1.0f,ey=0.0f;
GLfloat fx=1.0f,fy=0.0f,fz=0.0f;
int nface;
int ncube;
int mode=0;
int firstf=0;
GLfloat plate[8][3]={{0.0f,0.0f,1.0f},//0front blue
                     {0.0f,1.0f,0.0f},//1back green
                     {1.0f,0.5f,0.0f},//2left oringe
                     {1.0f,0.0f,0.0f},//3right red
                     {1.0f,1.0f,0.0f},//4top yellow
                     {1.0f,1.0f,1.0f},//5bottom white
                     {0.5f,0.5f,0.5f},//6inside grey
                     {0.0f,1.0f,1.0f}
                     };
int cof[26][6]={{0,6,2,6,4,6},
                {0,6,6,6,4,6},
                {0,6,6,3,4,6},
                {0,6,2,6,6,6},
                {0,6,6,6,6,6},//4
                {0,6,6,3,6,6},
                {0,6,2,6,6,5},
                {0,6,6,6,6,5},
                {0,6,6,3,6,5},
                {6,6,2,6,4,6},//9
                {6,6,6,6,4,6},
                {6,6,6,3,4,6},
                {6,6,2,6,6,6},
                {6,6,6,3,6,6},
                {6,6,2,6,6,5},//14
                {6,6,6,6,6,5},
                {6,6,6,3,6,5},
                {6,1,2,6,4,6},
                {6,1,6,6,4,6},
                {6,1,6,3,4,6},//19
                {6,1,2,6,6,6},
                {6,1,6,6,6,6},
                {6,1,6,3,6,6},
                {6,1,2,6,6,5},
                {6,1,6,6,6,5},//24
                {6,1,6,3,6,5}};
GLfloat inipos[26][3]={{-1.0f, 1.0f, 1.0f},
                       { 0.0f, 1.0f, 1.0f},
                       { 1.0f, 1.0f, 1.0f},
                       {-1.0f, 0.0f, 1.0f},
                       { 0.0f, 0.0f, 1.0f},//
                       { 1.0f, 0.0f, 1.0f},
                       {-1.0f,-1.0f, 1.0f},
                       { 0.0f,-1.0f, 1.0f},
                       { 1.0f,-1.0f, 1.0f},
                       {-1.0f, 1.0f, 0.0f},
                       { 0.0f, 1.0f, 0.0f},
                       { 1.0f, 1.0f, 0.0f},
                       {-1.0f, 0.0f, 0.0f},
                       { 1.0f, 0.0f, 0.0f},
                       {-1.0f,-1.0f, 0.0f},
                       { 0.0f,-1.0f, 0.0f},
                       { 1.0f,-1.0f, 0.0f},
                       {-1.0f, 1.0f,-1.0f},
                       { 0.0f, 1.0f,-1.0f},
                       { 1.0f, 1.0f,-1.0f},
                       {-1.0f, 0.0f,-1.0f},
                       { 0.0f, 0.0f,-1.0f},//
                       { 1.0f, 0.0f,-1.0f},
                       {-1.0f,-1.0f,-1.0f},
                       { 0.0f,-1.0f,-1.0f},
                       { 1.0f,-1.0f,-1.0f}
                       };

GLfloat mvmat[16]={1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f};
int setfx[3][9]={{ 2, 8,25,19, 5,16,22,11,13},
                 { 1, 7,24,18, 4,15,21,10,10},
                 { 0, 6,23,17, 3,14,20, 9,12}};
                 
int setfy[3][9]={{ 0, 2,19,17, 1,11,18, 9,10},
                 { 3, 5,22,20, 4,13,21,12,12},
                 { 6, 8,25,23, 7,16,24,14,15}};
                 
int setfz[3][9]={{ 0, 6, 8, 2, 3, 7, 5, 1, 4},
                 { 9,14,16,11,12,15,13,10,10},
                 {17,23,25,19,20,24,22,18,21}};
                 

int rflag[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

#define __glPi 3.14159265358979323846

void gluPerspectivef(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat m[4][4];
    GLfloat sine, cotangent, deltaZ;
    GLfloat radians=(GLfloat)(fovy/2.0f*__glPi/180.0f);


    deltaZ=zFar-zNear;
    sine=(GLfloat)sin(radians);
    if ((deltaZ==0.0f) || (sine==0.0f) || (aspect==0.0f))
    {
        return;
    }
    cotangent=(GLfloat)(cos(radians)/sine);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1.0f;
    m[3][2] = -2.0f * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}
 
 
GLfloat box[] = {
	// FRONT
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	// BACK
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	// LEFT
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	// RIGHT
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	// TOP
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	// BOTTOM
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
};

GLfloat box2[] = {
	// FRONT
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// BACK
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	// LEFT
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	// RIGHT
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	// TOP
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	// BOTTOM
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
};

static int engine_init_display()
{
	// initialize OpenGL ES and EGL

	/* 
	 * Here specify the attributes of the desired configuration.
	 * Below, we select an EGLConfig with at least 8 bits per color
	 * component compatible with on-screen windows
	 */
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(android_app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, android_app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	eglMakeCurrent(display, surface, surface, context);
	
	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    width=w;
    height=h;
	// Initialize GL state.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepthf(1.0f);
	glVertexPointer(3, GL_FLOAT, 0, box);
	glEnableClientState(GL_VERTEX_ARRAY);
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	gluPerspectivef(45.0f, (1.0f * w) / h, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return 0;
}

void vec4MultMat4(GLfloat vin[4] ,GLfloat mat[16] ,GLfloat vout[4])
{
	GLfloat temp[4];
	temp[0]=mat[0+0*4]*vin[0]+mat[0+1*4]*vin[1]+mat[0+2*4]*vin[2]+mat[0+3*4]*vin[3];
	temp[1]=mat[1+0*4]*vin[0]+mat[1+1*4]*vin[1]+mat[1+2*4]*vin[2]+mat[1+3*4]*vin[3];
	temp[2]=mat[2+0*4]*vin[0]+mat[2+1*4]*vin[1]+mat[2+2*4]*vin[2]+mat[2+3*4]*vin[3];
	temp[3]=mat[3+0*4]*vin[0]+mat[3+1*4]*vin[1]+mat[3+2*4]*vin[2]+mat[3+3*4]*vin[3];
	vout[0]=temp[0];vout[1]=temp[1];vout[2]=temp[2];vout[3]=temp[3];
}

void getViewport(GLfloat m[16])
{
	GLfloat ag[6];
	glGetFloatv(GL_VIEWPORT,&ag[0]);
	glGetFloatv(GL_DEPTH_RANGE,&ag[4]);
	GLfloat x=ag[0];
	GLfloat y=ag[1];
	GLfloat w=ag[2];
	GLfloat h=ag[3];
	GLfloat n=ag[4];
	GLfloat f=ag[5];
	m[0+4*0] = w/2; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = x+w/2;
    m[1+4*0] = 0; m[1+4*1] = h/2; m[1+4*2] = 0; m[1+4*3] = y+h/2;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = (f-n)/2; m[2+4*3] = (n+f)/2;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;

}

void projectToScreen(int n,GLfloat pi[3],GLfloat po[3],GLfloat mvMatrix[16])
{
	GLfloat pjMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX,pjMatrix);
	GLfloat p4[4];
	p4[0]=pi[0]+inipos[n][0];p4[1]=pi[1]+inipos[n][1];p4[2]=pi[2]+inipos[n][2];p4[3]=1.0f;
	vec4MultMat4(p4,mvMatrix,p4);
	vec4MultMat4(p4,pjMatrix,p4);
	GLfloat wc=p4[3];
	p4[0]/=wc;p4[1]/=wc;p4[2]/=wc;p4[3]=1.0f;
	GLfloat vpMatrix[16];
	getViewport(vpMatrix);
	vec4MultMat4(p4,vpMatrix,p4);
	po[0]=p4[0];po[1]=height-p4[1];po[2]=p4[2];
}

bool pointInRegion(GLfloat px,GLfloat py,GLfloat plist[12])
{
	int nCross = 0;    // 定义变量，统计目标点向右画射线与多边形相交次数

	for (int i = 0; i < 12; i++) {   //遍历多边形每一个节点

		GLfloat p1x = plist[i];
		GLfloat p1y = plist[i+1];
		GLfloat p2x = plist[(i+3)%12];
		GLfloat p2y = plist[(i+4)%12];  // p1是这个节点，p2是下一个节点，两点连线是多边形的一条边
// 以下算法是用是先以y轴坐标来判断的

		if ( p1y == p2y )
			continue;   //如果这条边是水平的，跳过

		if ( py < (p1y<p2y?p1y:p2y)) //如果目标点低于这个线段，跳过
			continue; 

		if ( py >= (p1y>p2y?p1y:p2y)) //如果目标点高于这个线段，跳过
			continue; 
//那么下面的情况就是：如果过p1画水平线，过p2画水平线，目标点在这两条线中间
		GLfloat x = (py - p1y) * (p2x - p1x) / (p2y - p1y) + p1x; 
// 这段的几何意义是 过目标点，画一条水平线，x是这条线与多边形当前边的交点x坐标
		if ( x > px ) 
			nCross++; //如果交点在右边，统计加一。这等于从目标点向右发一条射线（ray），与多边形各边的相交（crossing）次数
	} 

	if (nCross % 2 == 1) {

		return true; //如果是奇数，说明在多边形里
	}
	else {

		return false; //否则在多边形外 或 边上
	}

}

int chosenFace(int x, int y)
{
	GLfloat xf = (GLfloat) x;
	GLfloat yf = (GLfloat) y;
	GLfloat face[12];
	GLfloat Cdepth;
	GLfloat mdepth = 100.0f;
	int num = -1;
	for (int k = 0; k < 26; k++)
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				projectToScreen(k,&box2[i * 12 + j * 3], &face[j * 3], mvmat);
			}
			if (pointInRegion(xf, yf, face))
			{
				Cdepth = (face[2] + face[5] + face[8] + face[11]) / 4;
				if (Cdepth < mdepth)
				{
					mdepth = Cdepth;
					ncube=k;
					num = i;
				}
			}
		}
	}
	return num;
}

void setFloorFlag()
{
	if(fx!=0)
	{
		if(inipos[ncube][0]>0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfx[0][i]]=1;
			}
		}
		if(inipos[ncube][0]==0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfx[1][i]]=1;
			}
		}
		if(inipos[ncube][0]<0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfx[2][i]]=1;
			}
		}
	}

	if(fy!=0)
	{
		if(inipos[ncube][1]>0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfy[0][i]]=1;
			}
		}
		if(inipos[ncube][1]==0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfy[1][i]]=1;
			}
		}
		if(inipos[ncube][1]<0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfy[2][i]]=1;
			}
		}
	}
	
	if(fz!=0)
	{
		if(inipos[ncube][2]>0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfz[0][i]]=1;
			}
		}
		if(inipos[ncube][2]==0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfz[1][i]]=1;
			}
		}
		if(inipos[ncube][2]<0)
		{
			for(int i=0;i<9;i++)
			{
				rflag[setfz[2][i]]=1;
			}
		}
	}
}

void turn4x(int s[8],GLfloat d)
{
	int temp;
	int corner[4][4]={{0,4,1,5},
	                  {4,1,5,0},
	                  {2,2,2,2},
                      {3,3,3,3}};
	int edge[3][4]={{0,4,1,5},
	                {2,2,2,2},
                    {3,3,3,3}};
	if(d>0)
	{
		for(int i=0;i<4;i++)
		{
		temp=cof[s[0]][corner[i][0]];
		cof[s[0]][corner[i][0]]=cof[s[3]][corner[i][1]];
		cof[s[3]][corner[i][1]]=cof[s[2]][corner[i][2]];
		cof[s[2]][corner[i][2]]=cof[s[1]][corner[i][3]];
		cof[s[1]][corner[i][3]]=temp;
		}
		
		for(int i=0;i<3;i++)
		{
		temp=cof[s[4]][edge[i][0]];
		cof[s[4]][edge[i][0]]=cof[s[7]][edge[i][1]];
		cof[s[7]][edge[i][1]]=cof[s[6]][edge[i][2]];
		cof[s[6]][edge[i][2]]=cof[s[5]][edge[i][3]];
		cof[s[5]][edge[i][3]]=temp;
		}
	}
	if(d<0)
	{
		for(int i=0;i<4;i++)
		{
		temp=cof[s[0]][corner[i][0]];
		cof[s[0]][corner[i][0]]=cof[s[1]][corner[i][3]];
		cof[s[1]][corner[i][3]]=cof[s[2]][corner[i][2]];
		cof[s[2]][corner[i][2]]=cof[s[3]][corner[i][1]];
		cof[s[3]][corner[i][1]]=temp;
		}
		
		for(int i=0;i<3;i++)
		{
		temp=cof[s[4]][edge[i][0]];
		cof[s[4]][edge[i][0]]=cof[s[5]][edge[i][3]];
		cof[s[5]][edge[i][3]]=cof[s[6]][edge[i][2]];
		cof[s[6]][edge[i][2]]=cof[s[7]][edge[i][1]];
		cof[s[7]][edge[i][1]]=temp;
		}
	}
}

void turn4y(int s[8],GLfloat d)
{
	int temp;
	int corner[4][4]={{0,2,1,3},
	                  {2,1,3,0},
	                  {4,4,4,4},
                      {5,5,5,5}};
	int edge[3][4]={{0,2,1,3},
	                {4,4,4,4},
                    {5,5,5,5}};
	if(d>0)
	{
		for(int i=0;i<4;i++)
		{
		temp=cof[s[0]][corner[i][0]];
		cof[s[0]][corner[i][0]]=cof[s[3]][corner[i][1]];
		cof[s[3]][corner[i][1]]=cof[s[2]][corner[i][2]];
		cof[s[2]][corner[i][2]]=cof[s[1]][corner[i][3]];
		cof[s[1]][corner[i][3]]=temp;
		}
		
		for(int i=0;i<3;i++)
		{
		temp=cof[s[4]][edge[i][0]];
		cof[s[4]][edge[i][0]]=cof[s[7]][edge[i][1]];
		cof[s[7]][edge[i][1]]=cof[s[6]][edge[i][2]];
		cof[s[6]][edge[i][2]]=cof[s[5]][edge[i][3]];
		cof[s[5]][edge[i][3]]=temp;
		}
	}
	if(d<0)
	{
		for(int i=0;i<4;i++)
		{
		temp=cof[s[0]][corner[i][0]];
		cof[s[0]][corner[i][0]]=cof[s[1]][corner[i][3]];
		cof[s[1]][corner[i][3]]=cof[s[2]][corner[i][2]];
		cof[s[2]][corner[i][2]]=cof[s[3]][corner[i][1]];
		cof[s[3]][corner[i][1]]=temp;
		}
		
		for(int i=0;i<3;i++)
		{
		temp=cof[s[4]][edge[i][0]];
		cof[s[4]][edge[i][0]]=cof[s[5]][edge[i][3]];
		cof[s[5]][edge[i][3]]=cof[s[6]][edge[i][2]];
		cof[s[6]][edge[i][2]]=cof[s[7]][edge[i][1]];
		cof[s[7]][edge[i][1]]=temp;
		}
	}
}

void turn4z(int s[8],GLfloat d)
{
	int temp;
	int corner[4][4]={{2,4,3,5},
	                  {4,3,5,2},
	                  {0,0,0,0},
                      {1,1,1,1}};
	int edge[3][4]={{2,4,3,5},
	                {0,0,0,0},
                    {1,1,1,1}};
	if(d>0)
	{
		for(int i=0;i<4;i++)
		{
		temp=cof[s[0]][corner[i][0]];
		cof[s[0]][corner[i][0]]=cof[s[3]][corner[i][1]];
		cof[s[3]][corner[i][1]]=cof[s[2]][corner[i][2]];
		cof[s[2]][corner[i][2]]=cof[s[1]][corner[i][3]];
		cof[s[1]][corner[i][3]]=temp;
		}
		
		for(int i=0;i<3;i++)
		{
		temp=cof[s[4]][edge[i][0]];
		cof[s[4]][edge[i][0]]=cof[s[7]][edge[i][1]];
		cof[s[7]][edge[i][1]]=cof[s[6]][edge[i][2]];
		cof[s[6]][edge[i][2]]=cof[s[5]][edge[i][3]];
		cof[s[5]][edge[i][3]]=temp;
		}
	}
	if(d<0)
	{
		for(int i=0;i<4;i++)
		{
		temp=cof[s[0]][corner[i][0]];
		cof[s[0]][corner[i][0]]=cof[s[1]][corner[i][3]];
		cof[s[1]][corner[i][3]]=cof[s[2]][corner[i][2]];
		cof[s[2]][corner[i][2]]=cof[s[3]][corner[i][1]];
		cof[s[3]][corner[i][1]]=temp;
		}
		
		for(int i=0;i<3;i++)
		{
		temp=cof[s[4]][edge[i][0]];
		cof[s[4]][edge[i][0]]=cof[s[5]][edge[i][3]];
		cof[s[5]][edge[i][3]]=cof[s[6]][edge[i][2]];
		cof[s[6]][edge[i][2]]=cof[s[7]][edge[i][1]];
		cof[s[7]][edge[i][1]]=temp;
		}
	}
}
void colorTurn()
{
	if(fx!=0)
	{
		if(inipos[ncube][0]>0)
		{
			turn4x(&setfx[0][0],fx);
		}
		if(inipos[ncube][0]==0)
		{
			turn4x(&setfx[1][0],fx);
		}
		if(inipos[ncube][0]<0)
		{
			turn4x(&setfx[2][0],fx);
		}
	}
	if(fy!=0)
	{
		if(inipos[ncube][1]>0)
		{
			turn4y(&setfy[0][0],fy);
		}
		if(inipos[ncube][1]==0)
		{
			turn4y(&setfy[1][0],fy);
		}
		if(inipos[ncube][1]<0)
		{
			turn4y(&setfy[2][0],fy);
		}
	}
	if(fz!=0)
	{
		if(inipos[ncube][2]>0)
		{
			turn4z(&setfz[0][0],fz);
		}
		if(inipos[ncube][2]==0)
		{
			turn4z(&setfz[1][0],fz);
		}
		if(inipos[ncube][2]<0)
		{
			turn4z(&setfz[2][0],fz);
		}
	}
}
static void engine_draw_frame() {
    if (display == NULL) {
        // No display.
        return;
    }
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0,0,-15);
    glRotatef(angle, ax, ay, 0.0f);  
    if(first)
    {
    	glTranslatef(0,0,15);
    }
    else
    {
    	first=1;
    }
    glMultMatrixf(mvmat);
    glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
    for(int i=0;i<26;i++)
    {
    glLoadMatrixf(mvmat);
    if(rflag[i])glRotatef(anglef, fx, fy, fz);
    glTranslatef(inipos[i][0],inipos[i][1],inipos[i][2]);
    glVertexPointer(3, GL_FLOAT, 0, box);
    glColor4f(plate[cof[i][0]][0], plate[cof[i][0]][1], plate[cof[i][0]][2], 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glColor4f(plate[cof[i][1]][0], plate[cof[i][1]][1], plate[cof[i][1]][2], 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glColor4f(plate[cof[i][2]][0], plate[cof[i][2]][1], plate[cof[i][2]][2], 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glColor4f(plate[cof[i][3]][0], plate[cof[i][3]][1], plate[cof[i][3]][2], 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glColor4f(plate[cof[i][4]][0], plate[cof[i][4]][1], plate[cof[i][4]][2], 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glColor4f(plate[cof[i][5]][0], plate[cof[i][5]][1], plate[cof[i][5]][2], 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
    glVertexPointer(3, GL_FLOAT, 0, box2);
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
    glLineWidth(6);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
    glDrawArrays(GL_LINE_LOOP, 8, 4);
    glDrawArrays(GL_LINE_LOOP, 12, 4);
    glDrawArrays(GL_LINE_LOOP, 16, 4);
    glDrawArrays(GL_LINE_LOOP, 20, 4);
    }
    glFlush();
    eglSwapBuffers(display, surface);
}

static void engine_term_display()
{
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(display, context);
	eglDestroySurface(display, surface);
	eglTerminate(display);
	animating = 0;
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}

void refresh()
{
	int x, y, z;				/* 获得陀螺仪矢量的x/y/z值 */
	ASensorEvent event;			/* 创立一个感应器事件用来储存 */
	int looper_id;				/* 函数ALooper_pollAll()返回的事件代码 */
	struct android_poll_source *source;
	while ((looper_id = ALooper_pollAll(animating ? 0 : -10, NULL, NULL, (void **)&source)) >= 0)	/* 有事件发生，保存事件源到source，并保存事件代码 */
		
	{
		if (source)				/* 如果事件源有效 */
			source->process(android_app, source);
		/* 将java扔给我们的系统状态与事件源扔进事件源中名为process的成员函数，对事件进行处理 */
		if (looper_id == LOOPER_ID_USER)	/* 如果返回的事件标识与我们初始化陀螺仪时设置的标识相同 */
			while (ASensorEventQueue_getEvents(sensor_event_queue, &event, 1) > 0)	/* 循环从感应器事件队列sensor_event_queue中获得事件，并保存到感应器事件event中，每次保存一个 */
			{
				x = event.acceleration.x;	/* 从感应器事件event的成员acceleration中得到陀螺仪矢量的x值 */
				y = event.acceleration.y;	/* 从感应器事件event的成员acceleration中得到陀螺仪矢量的y值 */
				z = event.acceleration.z;	/* 从感应器事件event的成员acceleration中得到陀螺仪矢量的z值 */
			}
	}
}

int on_input_event(struct android_app *app, AInputEvent * input_event)	/* app是java扔给我们的系统状态,input_event是java扔给我们的输入事件 */
{
	if (AInputEvent_getType(input_event) == AINPUT_EVENT_TYPE_MOTION)	/* 我们用AInputEvent_getType()得知输入事件是屏幕触碰 */
	{
		switch (AMotionEvent_getAction(input_event))
		{
		case AMOTION_EVENT_ACTION_MOVE:
			{
				state_x = AMotionEvent_getX(input_event, 0);
				state_y = AMotionEvent_getY(input_event, 0);
				
				if(mode==1)
				{
				animating = 1;
				ax=(state_y-last_y);
				ay=(state_x-last_x);
				angle=0.5f*sqrt(pow(ay,2)+pow(ax,2));
				}
				
				if(mode==2)
				{
					if(firstf==0)
					{
						GLfloat ppp[12];
						for(int i=0;i<4;i++)
						{
							projectToScreen(ncube,&box2[nface*12+i*3],&ppp[i*3],mvmat);
						}
						GLfloat dir=atan2(state_y-last_y,state_x-last_x);
						GLfloat dd=fabs(dir-atan2(ppp[4]-ppp[1],ppp[3]-ppp[0]));
						int n=0;
						for(int i=1;i<4;i++)
						{
							GLfloat dd0=fabs(dir-atan2(ppp[(i+1)%4*3+1]-ppp[i*3+1],ppp[(i+1)%4*3]-ppp[i*3]));
							if(dd>dd0)
							{
								dd=dd0;
								n=i;
							}
						}
						GLfloat dx=ppp[(n+1)%4*3]-ppp[n*3];
						GLfloat dy=ppp[(n+1)%4*3+1]-ppp[n*3+1];
						ex=dx/sqrt(pow(dx,2)+pow(dy,2));
						ey=dy/sqrt(pow(dx,2)+pow(dy,2));
						fx=box2[nface*12+(n+2)%4*3]-box2[nface*12+(n+1)%4*3];
						fy=box2[nface*12+(n+2)%4*3+1]-box2[nface*12+(n+1)%4*3+1];
						fz=box2[nface*12+(n+2)%4*3+2]-box2[nface*12+(n+1)%4*3+2];
						setFloorFlag();
						firstf=1;
					}
					anglef+=0.5*((state_x-last_x)*ex+(state_y-last_y)*ey);
					if(anglef>=90||anglef<=0)
					{
						if(anglef>=90)
						{
							colorTurn();
						}
						
						for(int i=0;i<26;i++)
						{
							rflag[i]=0;
						}
						mode=0;
						anglef=0;
					}
					animating=1;
				}
				last_x=state_x;
				last_y=state_y;
			}
			break;
		case AMOTION_EVENT_ACTION_DOWN:
			{
				last_x = AMotionEvent_getX(input_event, 0);
				last_y = AMotionEvent_getY(input_event, 0);
				nface=chosenFace(last_x,last_y);
				if(nface==-1)mode=1;
				else {mode=2;firstf=0;}
			}
			break;
		case AMOTION_EVENT_ACTION_UP:
		    if(anglef>45)colorTurn();
		    for(int i=0;i<26;i++)
			{
				rflag[i]=0;
			}
		    mode=0;
		    anglef=0;
		    angle=0;
		    animating=1;
			break;
		}
	}
	return 0;					/* 应当返回0，但如果认为input_event事件已经完全处理完毕的话，可以返回1 */
}

void on_app_cmd(struct android_app *app, int event_type)	/* app是java扔给我们的系统状态,event_type是java扔给我们的事件类型 */
{

	switch (event_type)			/* 我们根据java扔给我们的事件类型做出不同的反应 */
	{
	case APP_CMD_INIT_WINDOW:	/* java扔给我们的事件类型是：初始化窗口 */
		engine_init_display();
		engine_draw_frame();
        break;
	case APP_CMD_TERM_WINDOW:	/* java扔给我们的事件类型是：关闭窗口 */
		engine_term_display();
		break;
	case APP_CMD_GAINED_FOCUS:	/* java扔给我们的事件类型是：获得焦点 */
		ASensorEventQueue_enableSensor(sensor_event_queue, sensor);	/* 为感应器事件队列(sensor_event_queue)启用陀螺仪感应器(sensor) */
		ASensorEventQueue_setEventRate(sensor_event_queue, sensor, 1000000 / 60);	/* 设定感应器的事件频率，换算后是1/60秒，与一般的屏幕刷新率一致） */
		break;
	case APP_CMD_LOST_FOCUS:	/* java扔给我们的事件类型是：失去焦点 */
		ASensorEventQueue_disableSensor(sensor_event_queue, sensor);	/* 为感应器事件队列(sensor_event_queue)关闭感应器(sensor) */
		animating = 0;
        engine_draw_frame();
		break;
	}
}

void android_main(struct android_app *app)
{
	ASensorManager *sensor_manager;	/* 感应器管理器 */
	app_dummy();				/* 确保glue的功能没有被忽略 */
	app->onAppCmd = on_app_cmd;	/* 将9楼完成的on_app_cmd回传函数赋值到app的onAppCmd成员 */
	app->onInputEvent = on_input_event;	/* 将13楼完成的on_input_event回传函数赋值到app的onInputEvent成员 */
	android_app = app;			/* 将参数app赋值到全局变量android_app */
	sensor_manager = ASensorManager_getInstance();	/* 获得感应器管理器 */
	sensor = ASensorManager_getDefaultSensor(sensor_manager, ASENSOR_TYPE_ACCELEROMETER);	/* 获得陀螺仪感应器 */
	sensor_event_queue = ASensorManager_createEventQueue(sensor_manager, app->looper, LOOPER_ID_USER, NULL, NULL);	/* 创建系统循环更新时的感应器事件队列 */
	/* 自定义部分，主循环也包含在这个部分中 */
	while (1)
	{
		refresh();
		if (animating) {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame();
            animating=0;
        }
	}
}