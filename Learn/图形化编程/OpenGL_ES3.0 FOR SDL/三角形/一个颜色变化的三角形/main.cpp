#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengles.h"
#include "SDL2/SDL_opengles2.h"
#include "SDL2/SDL_opengles2_gl2.h"
#include "SDL2/SDL_opengles2_gl2ext.h"
//使用opengles3.0版本请用 #version 300 es
/*
Uniform
Uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，但uniform和顶点属性有些不同。
首先，uniform是全局的(Global)。全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，
而且它可以被着色器程序的任意着色器在任意阶段访问。第二，无论你把uniform值设置成什么，uniform会一直保存它们的数据，
直到它们被重置或更新。
我们可以在一个着色器中添加uniform关键字至类型和变量名前来声明一个GLSL的uniform。
从此处开始我们就可以在着色器中使用新声明的uniform了。
我们来看看这次是否能通过uniform设置三角形的颜色：
*/

/*我们在片段着色器中声明了一个uniform vec4的ourColor，并把片段着色器的输出颜色设置为uniform值的内容。
因为uniform是全局变量，我们可以在任何着色器中定义它们，而无需通过顶点着色器作为中介。顶点着色器中不需要这个uniform，
所以我们不用在那里定义它。
如果你声明了一个uniform却在GLSL代码中没用过，编译器会静默移除这个变量，
导致最后编译出的版本中并不会包含它，这可能导致几个非常麻烦的错误，记住这点！
*/

char* vertexShaderSource =( char*)
"#version 300 es    \n"
"layout (location = 0) in vec3 position;\n"
"void main() \n"
"{   gl_Position = vec4(position.x, position.y, position.z, 1.0);}   \n";

char* fragmentShaderSource = (char*)
"#version 300 es \n"
"out vec4 color; \n"
"uniform vec4 ourColor;  \n "
"void main()   \n"
"{   color = ourColor ; }  \n" ;


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
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	// 4. 解绑VAO（不是EBO！）
	glBindVertexArrayOES(0);
	
	//这里实现颜色变化
	GLfloat timeValue = SDL_GetTicks();
	GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
	GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	/*
	我们用glGetUniformLocation查询uniform ourColor的位置值。
	我们为查询函数提供着色器程序和uniform的名字（这是我们希望获得的位置值的来源）。
	如果glGetUniformLocation返回-1就代表没有找到这个位置值。
	最后，我们可以通过glUniform4f函数设置uniform值。
	注意，查询uniform地址不要求你之前使用过着色器程序，
	但是更新一个unform之前你必须先使用程序（调用glUseProgram)，
	因为它是在当前激活的着色器程序中设置unform的。	
	*/
	
	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation,0.0f, greenValue, 0.0f, 1.0f);
		
	glBindVertexArrayOES(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArrayOES(0);

	/*
	glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取索引。
	这意味着我们必须在每次要用索引渲染一个物体时绑定相应的EBO，这还是有点麻烦。
	不过顶点数组对象同样可以保存索引缓冲对象的绑定状态。VAO绑定时正在绑定的索引缓冲对象会被保存为VAO的元素缓冲对象。
	绑定VAO的同时也会自动绑定EBO。	
	*/
	/*
	当目标是GL_ELEMENT_ARRAY_BUFFER的时候，VAO会储存glBindBuffer的函数调用。
	这也意味着它也会储存解绑调用，所以确保你没有在解绑VAO之前解绑索引数组缓冲，否则它就没有这个EBO配置了。	
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
		SDL_Delay(100);
	}
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

/*
我们可以看到这个矩形的颜色在发生变化，接下来我们就来设置顶点色

*/