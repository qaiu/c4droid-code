#include <string.h>
#include <errno.h>     
#include "iconv.h"

//说明: 使用G++编译时请手动删除include/g++/iconv.h文件
//编译参数加入 -liconv
//iInLen的长度不包括\0，应该用strlen。返回值是处理后的sOut长度     
static int Utf8ToGb2312(char *sOut, int iMaxOutLen, const char *sIn, int iInLen)     
{     
    char *pIn = (char *)sIn;     
    char *pOut = sOut;     
    size_t ret;     
    size_t iLeftLen=iMaxOutLen;     
    iconv_t cd;   
  
    cd = iconv_open("gb2312", "utf-8");     
    if (cd == (iconv_t) - 1)     
    {     
        return -1;     
    }     
    size_t iSrcLen=iInLen;     
    ret = iconv(cd, &pIn,&iSrcLen, &pOut,&iLeftLen);     
    if (ret == (size_t) - 1)     
    {     
        iconv_close(cd);     
        return -1;     
    }     
  
    iconv_close(cd);     
  
    return (iMaxOutLen - iLeftLen);     
}    
  
//iInLen的长度不包括\0，应该用strlen。返回值是处理后的sOut长度     
static int Gb2312ToUtf8(char *sOut, int iMaxOutLen, const char *sIn, int iInLen)     
{     
    char *pIn = (char *)sIn;     
    char *pOut = sOut;     
    size_t ret;     
    size_t iLeftLen=iMaxOutLen;     
    iconv_t cd;     
  
    cd = iconv_open("utf-8", "gb2312");     
    if (cd == (iconv_t) - 1)     
    {     
        return -1;     
    }     
    size_t iSrcLen=iInLen;     
    ret = iconv(cd, &pIn,&iSrcLen, &pOut,&iLeftLen);     
    if (ret == (size_t) - 1)     
    {     
        iconv_close(cd);     
        return -1;     
    }     
  
    iconv_close(cd);     
  
    return (iMaxOutLen - iLeftLen);     
}    
  
int main(void)   
{
	 
    char pszOri[] = {0xC4, 0xE3, 0xBA, 0xC3,0x0}; //GB2312编码的"你好"
    printf("strlen:%d\n", strlen(pszOri));   
     
    char pszDst[50] = "";     
    int iLen = Gb2312ToUtf8(pszDst, 50, pszOri, strlen(pszOri)); // Gb2312ToUtf8            
    printf("iLen:%d,pszDst=%s\n", iLen, pszDst);   //命令行打印utf8字符
      
    printf("------------------!\n");   
   
    char pszGbDst[50] = "";       
    int iNewLen = Utf8ToGb2312(pszGbDst, 50, pszDst, strlen(pszDst)); // Utf8ToGb2312  
    FILE *fp=fopen("编码转换.txt","w+");
    //命令行显示GB2312乱码，所以输出到文件
    fprintf(fp,"iNewLen:%d,pszGbDst=%s\n", iNewLen, pszGbDst);
    fclose(fp);
    return 0;     
}