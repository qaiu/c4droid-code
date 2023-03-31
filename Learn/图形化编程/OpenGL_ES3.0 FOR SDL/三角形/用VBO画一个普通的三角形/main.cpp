/*这里我们使用opengles3.0版本，3.0已经可以满足我们的需求了
当然SDL目前最高的支持是3.2
*/
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



	//使用VBO
	GLuint VBO;
	
	//1. 生成1个缓冲ID
	glGenBuffers(1, &VBO);  
	 //2. 绑定到顶点坐标数据缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 //3. 向顶点坐标数据缓冲送入数据
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);
	 //4. 将顶点位置数据送入渲染管线
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	//5. 启用顶点位置属性
	glEnableVertexAttribArray(0);
    //6. 使用程序片段
    glUseProgram(shaderProgram);
	//glBindVertexArrayOES(VBO);
	//7. 开始绘制三角形
	glDrawArrays(GL_TRIANGLES, 0, 3);
    //8. 解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER,0);

	
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



