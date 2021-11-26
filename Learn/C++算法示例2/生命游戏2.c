/**
  >> 生命游戏 v2.0 @qaiu.cn <<
  version log:
  > 地图改为 无界模式，即将地图左右，上下相连，类似于我的世界循环地图
  实测生命会更持久
  > 分辨率改为自适应
  > 更多提示信息: 当前细胞数，地图最大容量，演变细胞数峰值，迭代次数等等
  计划中: 地图编辑器，优化性能
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
// 初始生命数 (最大)
#define START_NUM 1500
// 繁衍代数
#define GENERATION_NUM 10000

// 间隔，毫秒
#define T 50

// 1无限地图，2死区和活区

int main(int argc, char** argv) { 
    srand(time(NULL));
    sleep(1); 
    struct winsize info;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &info);
    printf("\e[?25l\e[2J");
    int h=info.ws_row-1,w=info.ws_col/2;

    int map[h-2][w-2]; 
    int max=(h - 2) * (w - 2);
    for (int i = 0; i < START_NUM; i++) {
        int n = rand() % max;
        int i0 = n / (w-2);
        int j0 = n % (w-2);
        map[i0][j0] = 1;
    } 

    int last_map[h-2][w-2]; 
    int next_map[h-2][w-2]; 
    int over=0;
    for (int n = 0; n < GENERATION_NUM; n++) {
        int living_num = 0;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (i == 0 || j == 0 || i == h-1 || j == w-1) {
                    printf("\e[48;5;240m  \e[0m");
                } else {
                    int living = 0;
                    for (int k = 0; k < 9; k++) {
                        if (k == 4) continue;
                        int i0=i - 1 + k / 3;
                        int j0=j - 1 + k % 3;
                        if (i0==0) i0=h-2;
                        else if (i0==h-1) i0=1;
                        if (j0==0) j0=w-2;
                        else if (j0==w-1) j0=1;
                        living += (map[i0-1][j0-1] == 1);
                    }
                    // 存活 =3 繁殖
                    if (living == 3) next_map[i-1][j-1] = 1;
                    // 存活 >3或<2 死亡
                    else if (living > 3 || living < 2) next_map[i-1][j-1] = 0;
                    printf(map[i-1][j-1] ? (living_num++, "\e[47m  \e[0m") : "  ");

                }
            }
            printf("\n"); 
        }

        printf("living/size: %06d/%06d(%02dx%02d); gen/max: %06d/%06d;\e[1;1H",living_num, max, h - 2, w - 2, n,over>0?over-2:0); // 清屏
        //      getch();
        usleep(T * 1000);
        if (!memcmp(last_map,next_map, max * sizeof (int))&&!over){
            over=n;
        }
        memcpy(last_map, map, max * sizeof (int));

        memcpy(map, next_map, max * sizeof (int));
    }

}
