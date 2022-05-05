/**
  >> 兰顿蚂蚁 v1.2 blog.qaiu.top <<
  ==================

  兰顿蚂蚁和康威生命游戏类似，也是一种元胞自动机

  在平面上的正方形格被填上黑色或白色。
  在其中一格正方形有一只“蚂蚁”。
  它的头部朝向上下左右其中一方。

  1. 若蚂蚁在白格，右转90度，将该格改为黑格，向前移一步；
  2. 若蚂蚁在黑格，左转90度，将该格改为白格，向前移一步。
  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

// 间隔，微秒
#define T 100

int main(int argc, char** argv) {
	sleep(1);
	// 获取屏幕尺寸（终端下指的是字符最大的行列）
	struct winsize info;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &info);
	printf("\e[?25l\e[2J");
	int h=info.ws_row-1,w=info.ws_col/2;
	// 生成随机地图
	uint8_t map[h+1][w+1]; 
	memset(map,0,sizeof(map));
	// 定义蚂蚁的出生位置 这里默认地图中央
	int ant[2] = {w / 2, h / 2};
	// 定义蚂蚁初始方向，0123分别表示顺序上右下左
	int turn = 3, last_v;
	for (;;) {
		map[ant[1]][ant[0]] = (last_v = map[ant[1]][ant[0]]) ^ 1;    // 当前格颜色反转
		printf("\e[%d;%dH\e[48;5;%dm  \e[0m", ant[1], ant[0] * 2 + 1, last_v ? 254 : 232);    // 输出当前格
		ant[turn % 2] += ((turn >> 1 << 1) - 1) * ((last_v << 1) - 1);    // 基于当前方向前移
		turn = (turn == last_v * 3) ? 3 - last_v * 3 : turn + ((last_v << 1) - 1);    // 转向
		ant[1] = ant[1] == -1 ? h - 1 : ant[1] == h ? 0 : ant[1];    // 越界reset
		ant[0] = ant[0] == -1 ? w - 1 : ant[0] == w ? 0 : ant[0];
		printf("\e[%d;%dH\e[41m  \e[0m", ant[1], ant[0] * 2 + 1);    // 打印蚂蚁位置
		fflush(NULL);
		usleep(T);
	}

}
