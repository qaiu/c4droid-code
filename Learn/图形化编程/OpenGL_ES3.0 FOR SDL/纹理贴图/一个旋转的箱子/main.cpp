#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengles.h"
#include "SDL2/SDL_opengles2.h"
#include "SDL2/SDL_opengles2_gl2.h"
#include "SDL2/SDL_opengles2_gl2ext.h"
#include <SDL2/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

char *vertexShaderTextureSource = (char*)
"#version 300 es  \n"
"layout (location = 0) in vec3 position;  \n"
"layout (location = 1) in vec3 color;   \n"
"layout (location = 2) in vec2 texCoord; \n "
"out vec3 ourColor;  \n"  
"out vec2 TexCoord;  \n "
"uniform mat4 transform;  \n"
"void main() \n "
"{   gl_Position = transform* vec4(position, 1.0f); \n "
   " ourColor = color;   \n  "
  "  TexCoord = vec2(texCoord.x,1.0f-texCoord.y ) ; } \n" ;
char *fragmentShaderTexrureSource = (char*)
"#version 300 es   \n "
"in vec3 ourColor;  \n "
"in vec2 TexCoord; \n  "
"out vec4 color;  \n  "
"uniform sampler2D ourTexture1;  \n "
"uniform sampler2D ourTexture2;  \n "
"void main()  \n "
"{    color = mix (texture(ourTexture1, TexCoord),texture(ourTexture2,TexCoord),0.2 ); \n"
"  } \n ";

GLuint shaderProgram;

glm::mat4 trans ;
void createshader()
{
	  // 顶点 shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderTextureSource, NULL);
	glCompileShader(vertexShader);
	
	 // 片段 shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderTexrureSource, NULL);
	glCompileShader(fragmentShader);
	
	 //链接 shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// 用图片创建贴图
GLuint Textureload(char *filename)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface == NULL) { //加载失败退出
		return 0;
	}

	int mode;
	// work out what format to tell glTexImage2D to use...
	if (surface->format->BytesPerPixel == 3)
    { // RGB 24bit
		mode = GL_RGB;
	} 
	else if (surface->format->BytesPerPixel == 4) 
	{ // RGBA 32bit
		mode = GL_RGBA;
	} 
	else
    {
		SDL_FreeSurface(surface);
		return 0;
	}
	
	GLuint textureId;
	glGenTextures(1, &textureId);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ); 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w,surface->h, 0, mode,GL_UNSIGNED_BYTE,surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	//***

	//***
	if(surface) SDL_FreeSurface(surface);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}




int mIsAlive = 1;



GLuint texture1,texture2;// 贴图

GLfloat vertices[] = 
{
        // Positions          // Colors           // Texture Coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
};
GLuint indices[] = 
{  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
};


void render()
{
	trans = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	
//	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans,(GLfloat)SDL_GetTicks() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));


	glUseProgram(shaderProgram);
	
	GLuint VBO, VAO, EBO;
    glGenVertexArraysOES(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArrayOES(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

	GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1,GL_FALSE, glm::value_ptr(trans));
   

    glBindVertexArrayOES(0); // Unbind VAO	

    
   
   // 贴图 把纹理数据传进去
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture1);
	glUniform1i(glGetUniformLocation(shaderProgram,"ourTexture1"),0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,texture2);
	glUniform1i(glGetUniformLocation(shaderProgram,"ourTexture2"),1);
			
	//*** 绘制三角形
    glBindVertexArrayOES(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArrayOES(0);
	




}

void setViewport()
{
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0,&mode);
	glViewport(0,0,mode.w,mode.w);
	//设置视图

}

// 程序从 main 函数开始
int main(int args,char*argv[])
{
	
/*	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;*/
	/*
	trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	*/
	//*** SDL 初始化
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1)
	{
		SDL_LogError(0,"%s",SDL_GetError());
		return-1;
	}
	
	
	//*** opengles 使用版本 3.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
	//*** opengles 配置	
	//*** 窗口创建
	SDL_Window*win = SDL_CreateWindow("Hello World!",0,0,1920,1080,SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	if(win==NULL){
		SDL_LogError(0,"%s",SDL_GetError());
		return-1;
	}
	//***  opengles 创建上下文 
	SDL_GL_CreateContext(win);
	//*** 设置 清除背景颜色 白色
	glClearColor(1,1,1,1);
	
	//*** 设置窗口矩阵
	setViewport();
	//*** 初始化 shader 
	createshader();
	//***  
	int isload = 1;
	if(isload)
	{
	//*** 加载图片创建贴图
	texture1 = Textureload("box.jpg");
	texture2 = Textureload("face.jpg");
	}
	else
	{
		
		return 0;
	}
	
	mIsAlive = 1;
	while(mIsAlive)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type){
			case SDL_QUIT:mIsAlive = 0;
				break;
				
			case SDL_WINDOWEVENT:
				switch(event.window.event){
				case SDL_WINDOWEVENT_RESIZED:
					//窗口改变 打开手机的旋转屏幕试一下吧！
					setViewport();
					break;
				}
				break;
			}
			
		}
		
		//*** 清除颜色缓冲 *** 深度缓冲
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//***  画
		render();
		//*** opengles 更新窗口 
		SDL_GL_SwapWindow(win);
		SDL_Delay(100);
	}
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
