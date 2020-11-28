#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *decrypt(const unsigned char *buff, size_t size) {
    unsigned char *buff1 = (unsigned char *) malloc(size);
    buff1[0] = 27;
    int t = 0;
    for (int i = 1; i < size; i++) {
        t += size;
        buff1[i] = buff[i] ^ (t
                              + ((unsigned int) (((unsigned long) (-2139062143LL * t) >> 32) + t) >> 7)
                              + ((signed int) (((unsigned long) (-2139062143LL * t) >> 32) + t) < 0));
    }
    return buff1;
}

unsigned char buff[20480];
int main(int argc, char *argv[]) {
    char filename[20] = "main.lua";
    if (argc == 2)
        strcpy(filename,argv[1]);
    printf("File name: %s \n", filename);

    FILE *fp = fopen(filename, "rb");
    size_t size = 0;
    size = fread(buff, sizeof(unsigned char), 20480, fp);
    printf("File size: %ld \n", size);

    unsigned char *res = decrypt(buff, size);

    strcat(filename, "c");
    FILE *fp1 = fopen(filename, "wb");
    fwrite(res, sizeof(unsigned char), size, fp1);
    printf("Output: %s", filename);

    return 0;
}