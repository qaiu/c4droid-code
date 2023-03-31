// TODO 多平台适配，错误提示
// 通用头文件/库文件的安装程序

/* 
  1.运行即可安装成功
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// 安装配置
#define SRC_DIR_INC_PREFIX "glm" // 源路径前缀 指当前安装头文件所在的目录

#define DEST_DIR_INC_PREFIX "" // 目标路径前缀 以include目录为根目录的相对路径


// 返回子串位置
int strpos(const char *haystack,const char *needle)  
{
    register unsigned char c, needc;
    unsigned char const *from, *end;
    int len = strlen(haystack);
    int needlen = strlen(needle);
    from = (unsigned char *)haystack;
    end = (unsigned char *)haystack + len;
    const char *findreset = needle;
    for (int i = 0; from < end; ++i) {
        c = *from++;
        needc = *needle;
        if(c == needc) {
            ++needle;
            if(*needle == '\0') {
                if (len == needlen) 
                    return 0;
                else
                    return i - needlen+1;
            }
        }  
        else {  
            if(*needle == '\0' && needlen > 0)
                return i - needlen +1;
            needle = findreset;  
        }
    }  
    return  -1;  
} 

void if_root()
{

    if (getuid() == 0)
    {
        puts("请在非root环境下运行: 软件右上角>设置>以root身份运行>取消勾选");
        exit(1);
    }
    
}

// see https://blog.csdn.net/cheyo/article/details/6595955  
int call_system(const char* shell)
{
    int status = system(shell);
 
    if (-1 == status)
    {
        printf("system error!");
    }
    else
    {
        printf("exit status value = [0x%x]\n", status);
 
        if (WIFEXITED(status))// //正确退出
        {
            if (0 == WEXITSTATUS(status))//操作成功
            {
                printf("run shell script successfully.\n");
            }
            else
            {
                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
            }
        }
        else
        {
            printf("exit status = [%d]\n", WEXITSTATUS(status));
        }
    }
 
    return status;
}

// aarch64-linux-android
// arm-linux-androideabi 

int main()
{
    if_root();
    char path[64]={};
    char path_inc[256]={};
    char path_lib[256]={};
    
    const char*env=getenv("PATH");    
    int n=strpos(env,"/bintools");    
    strncpy(path,env,n);
    strcpy(path_inc,"cp -r ./");
    strcat(path_inc, SRC_DIR_INC_PREFIX);
    
    strcat(path_inc, " ");   
    strcat(path_inc, path);
    
    #if defined (__arm64__)|| defined (__aarch64__)
    printf("你的系统是arm64位: 64-bit [__arm64__ arch]\n");
    strcat(path_inc,"/gcc/aarch64-linux-android/include/");
    /*
    strcpy(path_lib,"cp lib64/* ");
    strcat(path_lib,path);
    strcat(path_lib,"/gcc/aarch64-linux-android/lib/");
    */
    #elif defined(__arm__)
    printf("你的系统是arm32位: [__arm__ arch]\n");
    strcat(path_inc,"/gcc/arm-linux-androideabi/include/");
    /*
    strcpy(path_lib,"cp lib32/*");
    strcat(path_lib,path);
    strcat(path_lib,"/gcc/arm-linux-androideabi/lib/");
    */
    #else
    printf("暂不支持\n");
    return 0;
    #endif
    
    
    puts("开始安装");   
    strcat(path_inc, DEST_DIR_INC_PREFIX);
    puts(path_inc);
    call_system(path_inc);
    return 0;
}