/*c4droid代码手册
 *控制台坦克大战
 *亅小夕ve编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include<time.h>
#include <conio.h>
#include<stdlib.h>

#define H 15
#define W 25
#define Left '4'
#define Right '6'
#define Up '2'
#define Down '8'
#define inAttack1 '5'

#define inAttack2 '9'
#define Back '7'

char map[H + 1][W + 1], s[100];
int eSum = 0, eSumMax = 2;
int point = 0, gameAble = 1, kills, myHpMax = 10, gameTime = 80;
int Ac, Ad;
int enemyHp = 1;
int getHpTime = 5;
int level = 1;

struct Time {
  clock_t S;
  clock_t F;
  double D;
} t[20], tt[10];


struct Tank {
  int a;                        /* 纵坐标 */
  int b;                        /* 横坐标 */
  int c;                        /* 方向 */
  int Aa;                       /* 攻击纵坐标 */
  int Ab;                       /* 攻击横坐标 */
  int Ac;                       /* 攻击方向 */
  int Ad;                       /* 判断攻击 */
  int hp;                       /* 血量 */
  char h;                       /* 血量存储字符 */
  char s;                       /* 自身存储字符 */
  char *ss;                     /* 自身存储字符表达 */
  char p;
  /* 炮弹存储字符 */
  char *pp;                     /* 炮弹存储字符表达 */
  char in;
} T[20];

void mapMake();
void mapDraw();
void out(int a, int b, char *c);
void out(int a, int b, int c);
void tankMake(int u, int e);
void inTank();
void gameStar();

int moveAble(int u, int a, int b);
int turnAble(int u, char c);
int DieAble(int u, char d);

void move(int u, char d);
void turnBack(int u);
int die(int u);
void attack(int u);
void mineAttack();
void attack_();
void getHp();
void enemyMake();
void enemy();
void emove(int u);

void tips(int t);
void youWin();
void youDie();
int choose();
void help();
void about();

void game1();
#include"aboutMove6.h"
#include"aboutDraw6.h"
#include"aboutGame1.h"
int main() {
 
  gameStar();
  return 0;
}

void gameStar() {

  inTank();
  srand(time(0));
  printf("\033[?25l");
    
  while (1) {
    switch (choose()) {
    case (1):
      game1();
      break;
    case (2):
      help();
      getch();
      break;
    case (3):
      about();
      getch();
      break;
    case (4):
      system("input keyevent 4");
    }
  }
}


int choose() {
  static int cho = 1;
  char in;
  clrscr();
  while (1) {
    if (cho == 1)
      out(10, 3, "\E[46;37m开始\E[40;37m");
    else
      out(10, 3, "\E[47;36m开始\E[40;37m");
    if (cho == 2)
      out(10, 5, "\E[46;37m帮助\E[40;37m");
    else
      out(10, 5, "\E[47;36m帮助\E[40;37m");
    if (cho == 3)
      out(10, 7, "\E[46;37m关于\E[40;37m");
    else
      out(10, 7, "\E[47;36m关于\E[40;37m");
    if (cho == 4)
      out(10, 9, "\E[46;37m退出\E[40;37m");
    else
      out(10, 9, "\E[47;36m退出\E[40;37m");
    in = getch();
    if (in == '2' && cho > 1)
      --cho;
    if (in == '8' && cho < 4)
      ++cho;
    if (in == '5')
      break;
  }
  clrscr();
  return cho;
}
void help() {
  out(8, 1, "按2,4,6,8");
  out(8, 3, "上下左右移动");
  out(8, 5, "按5攻击1");
  out(8, 7, "按7后退");
  out(8, 9, "按9攻击2➨击中\33[42;31m❤ \33[40;37m时回血");

  out(10, 14, "任意键返回");
}

void about() {
  out(10, 4, "㆑º㆑º㆑º㆑º");
  out(10, 14, "任意键返回");
}


