/**
 * 需要libcurl库 
 * 
 * 封装libcurl http 请求，支持get，post
 * @QAIU
 */

#ifndef HTTP_CLIENT_CURL_H
#define HTTP_CLIENT_CURL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

struct MemoryStruct {
	char *memory;
	size_t size;
};
static size_t WriteMemoryCallback(void *contents, size_t size,
				  size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *) userp;
	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/*      out     of      memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

/*
curl -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' 'http://'
Request URL:
*/
char* curl_http_client(int http_method,const char*url,const char*params)
{
	int ret = 0;
	int info = 0;
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;
	struct MemoryStruct chunk;
	chunk.memory = (char*)malloc(1);
	/* will be grown as needed by the realloc above */
	chunk.size = 0;
	/*no data at this point */

	char *Request_URL = (char *) malloc(strlen(url) + 1);
	strcpy(Request_URL, url);

	curl = curl_easy_init();
	if (!curl)
		return NULL;
		
	if (http_method&&params) {
    	headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded; charset=UTF-8");	
	    headers = curl_slist_append(headers, "Accept: application/json");
	} else{
    	headers = curl_slist_append(headers, "Accept: */*");
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);	//设置HTTP头
	curl_easy_setopt(curl, CURLOPT_URL, Request_URL);	//设置post请求的url地址
	//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);//设置发送超时时间
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
	
	if (http_method&&params) {
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);
    	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(params));
    	curl_easy_setopt(curl, CURLOPT_POST, 1);
	} else{
	   // curl_easy_setopt(curl, CURLOPT_GET, 1);
	}
	//curl_easy_setopt(curl,CURLOPT_VERBOSE,1);//打印调试信息

	res = curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &info);

	if (res != CURLE_OK || info != 200) {
		printf
		    ("!crul failed to perform url='%s' res=[%d] rcode=%d\n",
		     Request_URL, res, info);
	}

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	free(Request_URL);
	return chunk.memory;
}




char *http_get(const char *url)
{
	return curl_http_client(0,url,NULL);
}

char *http_post(const char *url,const char*params)
{
	return curl_http_client(1,url,params);
}

#endif // HTTP_CLIENT_CURL_H

//* test pass
int main()
{
    
	const char *url = "http://httpbin.org/post?test=OK";
	const char *url2 = "http://httpbin.org/get?test=OK";
	const char *post_param = "aaa=你好&test=OK";

	char *msg=http_post(url,post_param);
	puts(msg);
	free(msg);
	
	msg=http_get(url2);
	puts(msg);
	free(msg);
	
	return 0;
}
//*/