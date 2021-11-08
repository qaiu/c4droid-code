/**
  >> 生命游戏 @qaiu.cn <<
  ==================

  规则：
  (1)当前细胞为死亡状态时，当周围有3个存活细胞时，
  则迭代后该细胞变成存活状态(模拟繁殖)；若原先为生，
  则保持不变。
  (2)当前细胞为存活状态时，当周围的邻居细胞低于两个
  (不包含两个)存活时，该细胞变成死亡状态(模拟生命数量稀少)。
  (3)当前细胞为存活状态时，当周围有两个或3个存活细胞时，
  该细胞保持原样。
  (4)当前细胞为存活状态时，当周围有3个以上的存活细胞时，
  该细胞变成死亡状态(模拟生命数量过多)。

  可以把最初的细胞结构定义为种子，当所有种子细胞按以上
  规则处理后，可以得到第1代细胞图。按规则继续处理当前的
  细胞图，可以得到下一代的细胞图，
  周而复始。

  总结
  1.周围有3个存活则当前细胞存活
  2.周围超过3个或者少于2个存活则当前细胞死亡
  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

// 地图长宽
#define H 132
#define W 100
// 初始生命数 (最大)
#define START_NUM 2000
// 繁衍代数
#define GENERATION_NUM 10000

// 间隔，毫秒
#define T 50

int main() {   
	srand(time(NULL));
	sleep(1);    
	// 判断 终端尺寸
	struct winsize info;
	while (1) {
		ioctl(STDIN_FILENO, TIOCGWINSZ, &info);
		if (H > info.ws_row - 1 || W > info.ws_col / 2 - 1) { 
			printf("\e[2J\e[1;1H");
			printf("Please choose a smaller font, size(w*h):%d*%d;request size:%d*%d\n"
				   "Maybe you can put away the input method\n", info.ws_col/2, info.ws_row-2,W,H);
			usleep(1000 * 1000);
			continue;
		} else {    	
			printf("\e[?25l\e[2J"); //隐藏光标，清屏
			break;
		}
	}

	// 初始化随机地图(生成细胞种子)
	int map[H][W] = {}; 
	int max=(H - 1) * (W - 1);
	for (int i = 0; i < START_NUM; i++) {
		int n = rand() % max;
		int h = n / (W-1);
		int w = n % (W-1);
		map[h + 1][w + 1] = 1;
	} 

	// 细胞繁殖 迭代 死亡
	int next_map[H][W] = {}; 
	memcpy(next_map, map, H * W * sizeof (int));
	for (int n = 0; n < GENERATION_NUM; n++) {
		int living_num = 0;
		for (int i = 0; i < info.ws_row - 1; i++) {
			for (int j = 0; j < info.ws_col / 2; j++) {
				if (i > H || j > W) continue;
				if (i == 0 || j == 0 || i == H || j == W) {
					printf("\e[41m  \e[0m", j);
				} else {
					// 获取周围(3*3并去掉中间的自己)存活数
					int living = 0;
					for (int k = 0; k < 9; k++) {
						if (k == 4) continue;
						living += (map[i - 1 + k / 3][j - 1 + k % 3] == 1);
					}
					// 存活 =3 繁殖
					if (living == 3) next_map[i][j] = 1;
					// 存活 >3或<2 死亡
					else if (living > 3 || living < 2) next_map[i][j] = 0;
					printf(map[i][j] ? (living_num++, "\e[47m  \e[0m") : "  ");
				}
			}
			printf("\n");
		}
		printf("living: %06d/%06d\e[1;1H",living_num, max); // 清屏
		usleep(T * 1000);
		memcpy(map, next_map, H * W * sizeof (int));
	}
}
