#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengles.h"
#include "SDL2/SDL_opengles2.h"
#include "SDL2/SDL_opengles2_gl2.h"
#include "SDL2/SDL_opengles2_gl2ext.h"
//使用opengles3.0版本请用 #version 300 es
char* vertexShaderSource =( char*)
"#version 300 es    \n"
"layout (location = 0) in vec3 position;\n"
"void main() \n"
"{   gl_Position = vec4(position.x, position.y, position.z, 1.0);}   \n";

char* fragmentShaderSource = (char*)
"#version 300 es \n"
"out vec4 color;  \n "
"void main()   \n"
"{   color = vec4(1.0f, 0.5f, 0.2f, 1.0f); }  \n" ;


GLuint shaderProgram;

void createshader()
{
	  // 顶点 shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	//这里不做错误处理
	 // 初始化橙色片段 shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	 //链接黄色 shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检查链接错误
	//checkError(shaderProgramYellow, GL_LINK_STATUS);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

int mIsAlive = 1;


//三角形顶点坐标
float vertices[] = 
{
	0.5f,0.5f,0.0f,// top
	-0.5f,-0.5f,0.0f,// bottom left
	0.5f,-0.5f,0.0f // bottom right
};



void render()
{
	/*要想使用VAO，要做的只是使用glBindVertexArray绑定VAO。
	从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。
	当我们打算绘制一个物体的时候，我们只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了	
	*/


	//使用VAO
    GLuint VAO;
    GLuint VBO;
	glGenVertexArraysOES(1, &VAO);  
	
	// 1. 绑定VAO
	glBindVertexArrayOES(VAO);
	/*
	glBindVertexArray用名称数组​绑定顶点数组对象。
	数组​是以前通过调用glGenVertexArray返回的顶点数组对象的名称，或者为零，用于中断现有的顶点数组对象绑定。
	如果不存在具有名称数组​的顶点数组对象，则在首先绑定数组​时创建一个。
	如果绑定成功，则不会更改顶点数组对象的状态，并且任何以前的顶点数组对象绑定都会中断。	
	*/
	
    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);      
    /*glEnableVertexAttribArray启用index指定的通用顶点属性数组。
     glDisableVertexAttribArray禁用index指定的通用顶点属性数组。 
     默认情况下，禁用所有客户端功能，包括所有通用顶点属性数组。 
     如果启用，将访问通用顶点属性数组中的值，并在调用顶点数组命令（如glDrawArrays或glDrawElements）时用于呈现。
        
    */
    //解绑VAO;
	glBindVertexArrayOES(0);
	
    glUseProgram(shaderProgram);
	glBindVertexArrayOES(VAO);	//表示使用VAO中的数据
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArrayOES(0); //关闭
	
	/*
	就这么多了！前面做的一切都是等待这一刻，一个储存了我们顶点属性配置和应使用的VBO的顶点数组对象。
	一般当你打算绘制多个物体时，你首先要生成/配置所有的VAO（和必须的VBO及属性指针)，然后储存它们供后面使用。
	当我们打算绘制物体的时候就拿出相应的VAO，绑定它，绘制完物体后，再解绑VAO。
	*/
	
	
	
	
	
}

void init()
{
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0,&mode);
	//这里为了防止产生偏差，我们先使用相等的屏幕
	glViewport(0,0,mode.w,mode.w);
	
	
}



// 程序从 main 函数开始
int main(int args,char*argv[])
{
	
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
		SDL_LogError(0,"%s",SDL_GetError());
		return-1;
	}
	
	
	//*** opengles 使用版本 2.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
	
	

	
	// 窗口创建
	SDL_Window*win = SDL_CreateWindow("Hello World!",0,0,1920,1080,SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	if(win==NULL){
		SDL_LogError(0,"%s",SDL_GetError());
		return-1;
	}
	// *** opengles 创建上下文 
	SDL_GL_CreateContext(win);
	//*** 设置 清除背景 颜色
	glClearColor(1,1,1,1);
	
	// 设置窗口矩阵
	init();
	//初始化 shader 
	createshader();   
	mIsAlive = 1;
	while(mIsAlive)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type==SDL_QUIT){
				mIsAlive = 0;
				break;
			}
		}
		
		//*** 清除颜色缓冲 *** 深度缓冲
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		render();
		//*** opengles 更新窗口 
		SDL_GL_SwapWindow(win);
		SDL_Delay(20);
	}
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
