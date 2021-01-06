/**
 * HttpClient v1.0
 * QAIU
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>

#define ERROR_EXIT 0
//#include "errmacros.h"
#define ERROR_PRINT(...) 							                     \
		do {					  					                     \
		  printf("\e[31;1m%s\n已退出,错误码%d\e[0m",__VA_ARGS__);                           \
		  exit(EXIT_FAILURE);							\
		} while(0)


#define DEFAULT_BUFFER_SIZE 4096

#define HTTP_POST_HEADER \
    "POST %s HTTP/1.1\r\n" \
    "HOST: %s:%d\r\n" \
    "Accept: */*\r\n" \
    "Content-Type: application/x-www-form-urlencoded\r\n" \
    "Content-Length: %d\r\n" \
    "\r\n" \
    "%s" 

#define HTTP_GET_HEADER \
    "GET %s HTTP/1.1\r\n" \
    "HOST: %s:%d\r\n" \
    "Accept: */*\r\n" \
    "User-Agent: Mozilla/4.0(compatible)\r\n" \
    "connection:Keep-Alive\r\n" \
    "\r\n"

//#define HTTP_ERROR(msg)

enum {
	GET=1,
	POST,
	PUT,
	DELETE
};

struct http_request_info {
	char* host;
	char* path;
	int port;
	char *params;
};

struct http_response {
	char *header;
	char *body;
};

typedef struct http_request_info REQ_INFO;
typedef struct http_request_info *PREQ_INFO;

typedef struct http_response RESP;
typedef struct http_response *PRESP;

//创建基于socket的tcp连接
int create_connect(const char *host,int port) {
	int socket_fd;
	struct sockaddr_in pin={};
	struct hostent *remoteHost;
	if ((remoteHost = gethostbyname(host)) == 0)
	{
		ERROR_PRINT("Error resolving host\n");
	}
	// inet_ntop(*remoteHost->h_addr_list);
	pin.sin_family = AF_INET;	// 设置为ipv4
	pin.sin_port = htons(port);
	pin.sin_addr.s_addr = ((struct in_addr *)(remoteHost->h_addr))->s_addr;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		ERROR_PRINT("Error opening socket!\n");
	}
	if (connect(socket_fd, (const sockaddr *)&pin, sizeof(pin)) == -1)
	{
		ERROR_PRINT("Error connecting to socket\n");
	}
	return socket_fd;
}

PREQ_INFO parser_url(const char*url)
{
	char* host=(char*)malloc(DEFAULT_BUFFER_SIZE/8);
	char* path=(char*)malloc(DEFAULT_BUFFER_SIZE);
	int port=0;
	//解析url格式
	if (!strncmp(url,"http://",7)) url+=7;
	if (!strncmp(url,"https://",8)) url+=8;
	const char *url_path=strchr(url,'/');
	if (url_path==NULL)
	{
		strcpy(host,url);
		path[0] = '/';
	} else {
		strncpy(host, url,url_path-url);
		strcpy(path, url_path);
	}
	char *url_port = strchr(host,':');
	if (url_port != NULL)
	{
		sscanf(url_port+1, "%d", &port);
		*url_port = 0;
	} else {
		port = 80;	
	}
	PREQ_INFO info = (PREQ_INFO)malloc(sizeof(REQ_INFO));
	info -> host=host;
	info -> path=path;
	info -> port=port;

	return info;
}

int http_send(int socket_fd, PREQ_INFO req, const char*post_params, int type)
{
	//封装请求头
	char *message=(char*)malloc(8000);
	switch (type)
	{
	case GET:
		sprintf(message,HTTP_GET_HEADER,req->path,req->host,req->port);
		break;		
	case POST:
	case PUT:
	case DELETE:	
		sprintf(message,HTTP_GET_HEADER,req->path,req->host,req->port,strlen(post_params),post_params);
		break;				
	}
	
	//发送http请求
	return send(socket_fd, message, strlen(message), 0);
}

char *http_receive_response_header(int socket_fd)
{
	// 设置超时时间1秒，0代表秒后面的微秒数，左边这个就是1秒0微秒	
	struct timeval timeout = { 15, 0 };
	// 设置接收超时
	setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	int done = 0, chars = 0, l = 0;	
	//接受响应头
	//Content-Length
	char *http_header=(char*)calloc(1,DEFAULT_BUFFER_SIZE);
	char *buffer=(char*)calloc(1,DEFAULT_BUFFER_SIZE*64);
	//默认缓冲区1mb，手动释放
	char *buffer_body=(char*)calloc(1,DEFAULT_BUFFER_SIZE*256);
	char *temp=http_header;
	while (done == 0)
	{
		l = recv(socket_fd, http_header, 1, 0);
		if (l < 0)
			done = 1;
		switch (*http_header)
		{
		case '\r':
			break;
		case '\n':
			if (chars == 0)
				done = 1;
			chars = 0;
			break;
		default:
			chars++;
			break;
		}
//		printf("%c", *http_header);
		http_header++;
	}
	return temp;
}

char *http_receive_response_body(int socket_fd, const char *http_header)
{	
	char *content_length_str = NULL;
	char *buffer=(char*)malloc(DEFAULT_BUFFER_SIZE*16);
	char *buffer_body=(char*)malloc(DEFAULT_BUFFER_SIZE*512);
	char *temp=NULL;
	int content_length = 0, result_length = 0;
	if ((content_length_str=strstr(http_header,"\nContent-Length")) != NULL)
	{
		//Content-Length:定长响应接收
		sscanf(content_length_str,"\nContent-Length: %d",&content_length);
		
		result_length = recv(socket_fd, buffer, content_length, 0);
		strcat(buffer_body,buffer);
	}
	else if (strstr(http_header,"\nTransfer-Encoding: chunked") != NULL)
	{
		//Transfer-Encoding: chunked分块
		char* chunk_length_str=(char*)calloc(1,8);
		temp=chunk_length_str;
		strcpy(chunk_length_str,"0x");
		chunk_length_str+=2;
		int chunk_length=0;
		for(;;) 
		{
			recv(socket_fd, chunk_length_str, 1, 0);//读取chunked大小
			if (chunk_length_str[0] == '\r')
			{
				recv(socket_fd, chunk_length_str, 1, 0);
				if (chunk_length_str[0] == '\n') 
				{
					sscanf(temp, "%x\n", &chunk_length); //格式化chunk length字符串为int
					if (chunk_length == 0) break;
					while (chunk_length > 0)
					{
						result_length = recv(socket_fd, buffer, chunk_length, 0); //根据length长度循环读取
						chunk_length -= result_length;
						strcat(buffer_body, buffer);
						memset(buffer, 0, strlen(buffer));
					}	
					recv(socket_fd, chunk_length_str, 2, 0);
					chunk_length = 0;
					chunk_length_str = temp + 2;
					memset(chunk_length_str, 0, 6);
				} 
				else 
				{
					ERROR_PRINT("chunked size format error");	
				}
			}
			else if (isxdigit(chunk_length_str[0]))
			{	
				chunk_length_str++;
			} 
			else 
			{
				ERROR_PRINT("chunked size format error");
			}
		}
		free(chunk_length_str);
	}
	else
	{ //默认处理方式: 固定每次512字节读取
		do
		{
			result_length = recv(socket_fd, buffer, 512, 0);		
			if (result_length < 0) break;
			*(buffer + result_length) = 0;
			strcat(buffer_body, buffer);
		}
		while (result_length > 0);
	}
	free(buffer);
	return buffer_body;
}

PRESP http_client(const char* url,const char* params, int type, int flag)
{
	//1.解析url
	PREQ_INFO req = parser_url(url);
	//2.创建连接
	int socket_fd=create_connect(req->host, req->port);
	//3.发送请求
	if (http_send(socket_fd, req, params, type) == -1)
	{
		ERROR_PRINT("Error in send\n");
	}
	//4.接收响应头
	char *resp_header=http_receive_response_header(socket_fd);
	//5.接收响应实体
	char *resp_body=http_receive_response_body(socket_fd,resp_header);
	//6.关闭连接，清理内存
	close(socket_fd);
	free(req->host);
	free(req->path);
	free(req);
	
	//7.封装并返回
	PRESP resp=(PRESP)malloc(sizeof (http_response));
	resp->body=resp_body;
	if (flag == 1) 
		free (resp_header);
	else
		resp->header=resp_header;
	return resp;
}

//url格式 协议(可选)://地址:端口(不写默认80)/请求路径
//如www.baidu.com:8080/image/demo.html
//http://qaiu.top
//https协议默认返回301跳转信息，不支持https协议解析
char *http_get(const char *url)
{
	return http_client(url,NULL,GET,0)->body;
}

char *http_post(const char *url,const char*params)
{
	return http_client(url,params,POST,0)->body;
}

char *http_put(const char *url,const char*params)
{
	return http_client(url,params,PUT,0)->body;
}

char *http_delete(const char *url,const char*params)
{
	return http_client(url,params,DELETE,0)->body;
}

int main(int argc, char **argv)
{
	char* page = http_get("www.baidu.com");
	FILE *fp = fopen("/sdcard/index.html","w+");
	
	fprintf(fp,"%s",page);
	puts(page);
	free(page);
	fclose(fp);
	return 0;
}
