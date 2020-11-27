#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>

#define host   "www.baidu.com"
#define port    443

char sendbuff[1024*50];
char recvbuff[1024*50];

int main() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if(ctx == NULL) {
        return 0;
    }
    int fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    /* 如果host是IP，直接使用下面方式
    sin.sin_addr.s_addr = inet_addr(host);
    */
    struct hostent *ip = gethostbyname(host); // 域名dns解析
    if(ip == NULL) {
        printf("gethostbyname error");
        return 0;
    }
    sin.sin_addr = *(struct in_addr*)ip->h_addr_list[0];
    if(connect(fd,(struct sockaddr*)&sin, sizeof(sin)) < 0) {
        printf("connect error");
        return 0;
    }
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);
    if(SSL_connect(ssl) == -1) {
        printf("ssl connect error");
        return 0;
    }

    strcpy(sendbuff,"GET /index.html HTTP/1.1\r\n"
                    "Host: "host"\r\n"
                    "User-Agent: www.worldflying.cn client\r\n"
                    "Accept: */*\r\n"
                    "Cache-Control: no-cache\r\n"
                    "Accept-Encoding: gzip, deflate, br\r\n"
                    "Connection: keep-alive\r\n\r\n");
    SSL_write(ssl,sendbuff, strlen(sendbuff));

    while (1) {
        memset(recvbuff, 0, sizeof(recvbuff));
        if (SSL_read(ssl, recvbuff, sizeof(recvbuff)-1) <= 0) {
            break;
        }
        printf("%s\n", recvbuff);
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(fd);
    SSL_CTX_free(ctx);
}