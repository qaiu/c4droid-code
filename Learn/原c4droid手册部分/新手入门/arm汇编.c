#include <stdio.h>
int main(){
    int y = 0;
    int x = 11;
    __asm__ volatile("add %w0, %w1, 11" : "=r" (y) : "r" (x) :);	
    printf("%d,%d\n",x,y);	
}