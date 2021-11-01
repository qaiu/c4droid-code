/**
 * C语言控制台俄罗斯方块 
 * 使用gcc或者clang编译，安卓系统推荐c4droid和termux
 * 操作说明: 使用方向键或者--46或ad键左右移动 2或w旋转 8或s下落 回车或空格硬降 Q或者ESC+ESC退出
 * blog: https://blog.qaiu.top
 * 
 * @Date: 2019.6
 * @Author QAIU
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#define H 21 //地图行大小
#define W 16 //列

typedef struct { //方块结构体
    int x[4][4], y[4][4], type;
} TETROMINO;
TETROMINO tetromino_first, tetromino_next;

//方块x轴的坐标，用全局变量保存，获取对应的方块时通过下标获取
const int coord_x[7][2][4] = {
    {0, 5, 0, 0, 4, 5, 6, 0}, //T
    {0, 0, 0, 0, 4, 5, 6, 7}, //I
    {4, 5, 0, 0, 4, 5, 0, 0}, //O
    {4, 5, 0, 0, 0, 5, 6, 0}, //Z
    {0, 5, 6, 0, 4, 5, 0, 0}, //S
    {4, 0, 0, 0, 4, 5, 6, 0}, //J
    {0, 0, 6, 0, 4, 5, 6, 0}, //L
};
//y轴坐标
const int coord_y[7][2][4] = {
    {0, 1, 0, 0, 2, 2, 2, 0},
    {0, 0, 0, 0, 2, 2, 2, 2},
    {1, 1, 0, 0, 2, 2, 0, 0},
    {1, 1, 0, 0, 0, 2, 2, 0},
    {0, 1, 1, 0, 2, 2, 0, 0},
    {1, 0, 0, 0, 2, 2, 2, 0},
    {0, 0, 1, 0, 2, 2, 2, 0}, 

};

volatile int height = 0, down_bottom_flag = 0, line = 0, score = 0, level = 0;
int map[H + 1][W + 1] = { };
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER; //同步锁

//读取单字符 https://my.oschina.net/yougui/blog/111345
char getch() 
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char c=getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}


//判断输入 https://www.cnblogs.com/xiayong123/archive/2011/07/19/3717262.html
static int kbhit(void)
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return (ch != EOF)? ungetc(ch, stdin),1:0;
}

//初始化地图(边框)
void init_map(void)
{
    for (int i = 0; i <= H; ++i) {
        for (int j = 0; j < W; ++j) {
            if (j < 12 && (i == H || i == 0 || j == 0 || j == 11))
                map[i][j] = 2;
            else if (map[i][j] < 4)
                map[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetromino_first.y[i][j] != 0)
                map[tetromino_first.y[i][j]][tetromino_first.x[i][j]] = 1;
            if (tetromino_next.y[i][j] != 0)
                map[tetromino_next.y[i][j]][tetromino_next.x[i][j] + 8] = 3;
        }
    }
}

//打印地图
void draw_map()
{
    pthread_mutex_lock(&mut); //线程同步锁，防止过度刷新
    init_map(); //初始化地图数据(map数组)
    printf("\e[1;1f\n\n\t\t"); //首先定位到屏幕中间 内层循环打印行，外层打印列
    //主循环打印游戏区域，根据地图数组的值打印空白或者色块
    for (int i = 0; i <= H; i++) {
        for (int j = 0; j < W; j++) {
            switch (map[i][j]) {
            case 0: //0空白区域 颜色转义来输出彩色
                printf("  ");
                break;
            case 1: //主方块的色块
                printf("\e[37;4%dm  \e[0m", tetromino_first.type+1);
                break;
            case 2: //边框
                printf("\e[37m%s\e[0m",((i == 0 || i == H) && (j == 0 || j == 11)) ? "＋" : ((i == 0 || i == H) ? "――" : "｜"));
                break;
            case 3: //提示的下一个方块的色块
                printf("\e[4%dm  \e[0m", tetromino_next.type+1);
                break; 
            default: //落地的方块色块
                printf("\e[4%dm  \e[0m", map[i][j] - 3);
                break;
            }
        }
        printf("\n\t\t"); //内层循环打印一行完事，换行并使用水平制表符Tab(\t)定位到屏幕中间
    }
    //关卡，得分信息
    printf("\e[5;9f\e[32mscore:\e[6;9f%5d\e[8;9fline:\e[9;9f%5d\e[11;9flevel:\e[12;9f%5d\n", score, line, level);
    pthread_mutex_unlock(&mut); //同步锁结束
}

//获取随机方块
TETROMINO get_tetromino()
{
    //下一个的方块类型: 获取一个0-7之间的一个随机数
    TETROMINO tetromino_rand = { };
    tetromino_rand.type = (double)rand() / RAND_MAX * 7; 
    //根据类型作为下标复制对应方块的x轴坐标到下一个方块里
    memcpy(tetromino_rand.x, coord_x[tetromino_rand.type], 32);
    //复制到x轴
    memcpy(tetromino_rand.y, coord_y[tetromino_rand.type], 32); //y轴
    return tetromino_rand;
}

//判定函数 
void judge(int high)
{   //遍历每一行判断是否有连成一条线的行
    int sum = 0, flag = 1;
    for (int i = high - 3; i <= high; i++, flag = 1) {
        for (int j = 1; j < 11; j++) {
            if (map[i][j] < 4) {
                flag = 0;
                break;
            }
        }
        if (flag) {
            for (int j = i; j > 1; --j) {
                for (int i = 1; i < 11; ++i) {
                    map[j][i] = map[j - 1][i];
                }
            }
            sum++, line++;
        }
    }
    score += (sum * sum * 100 / 2), level = level < 10 ? line / 10 : 0;  //得分
}

//判断方块是否落到底部
int can_down_move(TETROMINO tet)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tet.y[i][j] != 0 && map[tet.y[i][j] + 1][tet.x[i][j]] > 1) {
                return 1;
            }
        }
    }
    return 0;
}

//平移 参数-1左移，1右移，通过x轴加上参数值实现
void horizontal_move(int direction)
{
    TETROMINO tetromino_temp = tetromino_first;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetromino_temp.y[i][j] != 0) {
                if (map[tetromino_temp.y[i][j]][tetromino_temp.x[i][j] + direction] > 1)
                    return;
                tetromino_temp.x[i][j] += direction;
            }
        }
    }
    memcpy(&tetromino_first, &tetromino_temp, sizeof(tetromino_temp));
    //每次平移后判断是否还可以继续下落
    down_bottom_flag = can_down_move(tetromino_first);
}

//旋转，可以参考二维数组的转置(旋转)相关算法
void rotate()
{
    if (tetromino_first.type == 2) //田字方块不旋转
        return;
    int flag = 0, size = tetromino_first.type == 1 ? 4 : 4 - 1;
    TETROMINO tetromino_temp = {.type = tetromino_first.type };
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (tetromino_first.x[size - 1 - j][i]) {
                tetromino_temp.y[i][j] = tetromino_first.y[size - 1 - j][i] - size + j + i + 1;
                tetromino_temp.x[i][j] = tetromino_first.x[size - 1 - j][i] - i + j;
                if (map[tetromino_temp.y[i][j]][tetromino_temp.x[i][j]] > 1) flag = 1;
            }
        }
    }
    if (!flag)
        memcpy(&tetromino_first, &tetromino_temp, sizeof(tetromino_temp));
    down_bottom_flag = can_down_move(tetromino_first);
}

//下落
void down_move()
{
    //如果可以下落，让方块y轴依次加一
    if (!down_bottom_flag) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (map[tetromino_first.y[i][j]][tetromino_first.x[i][j]] < 2)
                    tetromino_first.y[i][j]++;
            }
        }
        down_bottom_flag = can_down_move(tetromino_first);
        return;
    }
    //遍历获取下落后垒起来的高度，判断游戏最多能消几行
    int high;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetromino_first.y[i][j] != 0) {
                map[tetromino_first.y[i][j]][tetromino_first.x[i][j]] = tetromino_first.type + 4;
                high = tetromino_first.y[i][j];
            }
        }
    }
    // 首先看是否到达最大高度(接触屏幕上方的坐标，表示游戏结束)
    if (map[2][4] > 3 || map[2][5] > 3 || map[2][6] > 3 || map[2][7] > 3) {
        draw_map();
        memset(map, 0, sizeof(map));
        init_map();
        down_bottom_flag = score = line = level = 0;
        printf("\e[10;23f\e[47;30mGame Over!\n\e[0m");
        getch();
    } else {//没有结束的话执行消行(如果可以)
        score += 10; //基础分
        judge(high);
        //降落完成，获取下一个方块
        tetromino_first = tetromino_next, tetromino_next = get_tetromino();
        down_bottom_flag = can_down_move(tetromino_first);
    }
}

void exit_clear() 
{
    puts("\e[2J\e[1;1H\t\tBye~ @Author QAIU\e[?25h");
    exit(0);
}

// 事件函数，使用多线程所以需要一个void指针
void *event(void *p)
{
    printf("\n\e[0m任意键开始游戏\nq退出...\n");

    if (getch()=='q') exit_clear();
    *(int *)p=0;

    puts("\e[2J");
    while (1) {
        int key = getch();
        switch (key) {
        case 'a':case '4':				// 左移
            horizontal_move(-1);
            break;
        case 'd':case '6':				// 右移
            horizontal_move(1);
            break;
        case 'w':case '2':				// 旋转
            rotate();
            break;
        case 's':case '5':				// 下移
            down_move();
            break;
        case ' ':case 10:				// 硬降
            while (!down_bottom_flag)
                down_move();
            down_move();
            break;
        case 'Q':case 'q':
            exit_clear();
            break;
        case 27:
            if (kbhit()) {
                if((key = getch()) == '[') {
                    switch(getch()) {
                    case 'A':
                        rotate();
                        break;
                    case 'B':
                        down_move();
                        break;
                    case 'D':
                        horizontal_move(-1);
                        break;
                    case 'C':
                        horizontal_move(1);
                        break;
                    }
                }
                break;
            }
            printf("\e[0m暂停\n任意键继续...\n按q退出\n");
            *(int *)p=1;
            if (getch()=='q') exit_clear();
            *(int *)p=0;
            puts("\e[2J");
        }
        draw_map();
    }
    return NULL;
}

//主线程   初始地图->绘制->(休眠一段时间)->执行下落->循环...
//子线程   按键监听->循环监听对应的指令(下落，平移等)
int main()
{
    srand(time(NULL)); //随机数发生器初始化
    pthread_t pid1; //线程id
    int8_t pause=1;
    tetromino_next = get_tetromino(); //生成当前方块
    tetromino_first = get_tetromino(); //生成下个方块
    printf("\e[?25l\e[2J"); //隐藏光标，清屏
    draw_map();
    usleep(1000);
    pthread_create(&pid1, NULL, event, &pause); //启动按键事件线程监听按键
    while (1) {
        if (pause) continue;
        draw_map();
        usleep(500000 - 80000 * level);
        down_move();
    }
}
