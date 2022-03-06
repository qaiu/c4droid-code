#ifndef KRC2LRC_H
#define KRC2LRC_H
#include <string.h>

#define PUT_ERROR(e) printf("%s:%s\n",strerror(errno),(char*)e)

typedef unsigned char byte;

#define KRC_END ".krc"
#define LRC_END ".lrc"

int krc2lrc(const char *file_name, const char *krc_name, const char *decode_path, const char *convert_path);
int file_size(const char*filename);

#endif