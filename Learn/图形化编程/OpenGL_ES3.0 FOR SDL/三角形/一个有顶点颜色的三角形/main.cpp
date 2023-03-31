#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengles.h"
#include "SDL2/SDL_opengles2.h"
#include "SDL2/SDL_opengles2_gl2.h"
#include "SDL2/SDL_opengles2_gl2ext.h"
//使用opengles3.0版本请用 #version 300 es


char* vertexShaderSource =( char*)
"#version 300 es    \n"
"layout (location = 1) in vec3 color; \n"
"layout (location = 0) in vec3 position;\n"
"out vec3 ourColor; \n"
"void main() \n"
"{   gl_Position = vec4(position.x, position.y, position.z, 1.0);ourColor = color;}   \n";

char* fragmentShaderSource = (char*)
"#version 300 es \n"
"out vec4 color; \n"
"in vec3 ourColor;  \n "
"void main()   \n"
"{   color = vec4(ourColor,1.0f) ; }  \n" ;


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

GLfloat vertices[] = 
{
    // 位置              // 颜色
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下, 红色
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下  绿色
     -0.5f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f  , // 顶部  蓝色
 	-0.5f,0.5f,0.0f,   0.0f,1.0f,0.0f

};


GLuint indices[] = 
{ // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};




void render()
{
	GLuint EBO;
	GLuint VAO;
	GLuint VBO;
	glGenVertexArraysOES(1, &EBO);  
	// 1. 绑定顶点数组对象
	glBindVertexArrayOES(VAO);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. 设定顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	// 4. 解绑VAO（不是EBO！）
	glBindVertexArrayOES(0);
	

	
	
	

	
	glUseProgram(shaderProgram);		
	glBindVertexArrayOES(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArrayOES(0);


	
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
		SDL_Delay(100);
	}
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}